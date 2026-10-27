#include "bplus_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hilfsfunktion: Erstellt einen neuen Knoten
static bplus_tree_node* create_node(bool is_leaf) {
    bplus_tree_node* node = (bplus_tree_node*)malloc(sizeof(bplus_tree_node));
    if (node == NULL) {
        return NULL;
    }
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->next = NULL;
    for (int i = 0; i < ORDER + 1; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Hilfsfunktion: Teilt einen vollen Blattknoten
static void split_leaf_node(bplus_tree_node* leaf, int* key, bplus_tree_node** new_leaf) {
    *new_leaf = create_node(true);
    if (*new_leaf == NULL) {
        return;
    }

    int split_pos = ORDER / 2;
    *key = leaf->keys[split_pos];

    // Kopiere die zweite Hälfte der Schlüssel in den neuen Knoten
    for (int i = split_pos; i < ORDER; i++) {
        (*new_leaf)->keys[i - split_pos] = leaf->keys[i];
        (*new_leaf)->num_keys++;
    }

    // Aktualisiere die Anzahl der Schlüssel im alten Knoten
    leaf->num_keys = split_pos;

    // Verknüpfe die Blattknoten
    (*new_leaf)->next = leaf->next;
    leaf->next = *new_leaf;
}

// Hilfsfunktion: Teilt einen vollen inneren Knoten
static void split_inner_node(bplus_tree_node* inner, int* key, bplus_tree_node** new_inner) {
    *new_inner = create_node(false);
    if (*new_inner == NULL) {
        return;
    }

    int split_pos = (ORDER + 1) / 2;
    *key = inner->keys[split_pos - 1];

    // Kopiere die zweite Hälfte der Schlüssel und Kinder in den neuen Knoten
    for (int i = split_pos; i < ORDER; i++) {
        (*new_inner)->keys[i - split_pos] = inner->keys[i];
        (*new_inner)->children[i - split_pos] = inner->children[i];
        (*new_inner)->num_keys++;
    }
    (*new_inner)->children[ORDER - split_pos] = inner->children[ORDER];

    // Aktualisiere die Anzahl der Schlüssel im alten Knoten
    inner->num_keys = split_pos - 1;
}

// Hilfsfunktion: Fügt einen Schlüssel in einen Blattknoten ein (sortiert)
static void insert_into_leaf(bplus_tree_node* leaf, int key) {
    int i = leaf->num_keys - 1;
    while (i >= 0 && key < leaf->keys[i]) {
        leaf->keys[i + 1] = leaf->keys[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->num_keys++;
}

// Hilfsfunktion: Fügt einen Schlüssel in einen inneren Knoten ein (sortiert)
static void insert_into_inner(bplus_tree_node* inner, int key, bplus_tree_node* child) {
    int i = inner->num_keys - 1;
    while (i >= 0 && key < inner->keys[i]) {
        inner->keys[i + 1] = inner->keys[i];
        inner->children[i + 2] = inner->children[i + 1];
        i--;
    }
    inner->keys[i + 1] = key;
    inner->children[i + 2] = child;
    inner->num_keys++;
}

// Hilfsfunktion: Fügt einen Schlüssel in den B+ Tree ein (rekursiv)
static void insert_recursive(bplus_tree_node* node, int key, int* promoted_key, bplus_tree_node** promoted_node) {
    if (node->is_leaf) {
        insert_into_leaf(node, key);
        if (node->num_keys == ORDER) {
            split_leaf_node(node, promoted_key, promoted_node);
        }
    } else {
        int i = 0;
        while (i < node->num_keys && key >= node->keys[i]) {
            i++;
        }
        insert_recursive(node->children[i], key, promoted_key, promoted_node);
        if (*promoted_key != -1) {
            insert_into_inner(node, *promoted_key, *promoted_node);
            *promoted_key = -1;
            *promoted_node = NULL;
            if (node->num_keys == ORDER) {
                split_inner_node(node, promoted_key, promoted_node);
            }
        }
    }
}

// Erstellt einen neuen B+ Tree
bplus_tree* bplus_tree_create() {
    bplus_tree* tree = (bplus_tree*)malloc(sizeof(bplus_tree));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = create_node(true);
    if (tree->root == NULL) {
        free(tree);
        return NULL;
    }
    tree->height = 1;
    return tree;
}

// Zerstört einen B+ Tree und gibt alle Knoten frei (rekursiv)
static void destroy_node(bplus_tree_node* node) {
    if (node == NULL) {
        return;
    }
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            destroy_node(node->children[i]);
        }
    }
    free(node);
}

// Zerstört einen B+ Tree und gibt alle Knoten frei
void bplus_tree_destroy(bplus_tree* tree) {
    if (tree != NULL) {
        destroy_node(tree->root);
        free(tree);
    }
}

// Fügt einen Schlüssel in den B+ Tree ein
void bplus_tree_insert(bplus_tree* tree, int key) {
    if (tree == NULL || tree->root == NULL) {
        return;
    }

    int promoted_key = -1;
    bplus_tree_node* promoted_node = NULL;

    insert_recursive(tree->root, key, &promoted_key, &promoted_node);

    if (promoted_key != -1) {
        // Der Wurzelknoten muss geteilt werden
        bplus_tree_node* new_root = create_node(false);
        if (new_root == NULL) {
            return;
        }
        new_root->keys[0] = promoted_key;
        new_root->children[0] = tree->root;
        new_root->children[1] = promoted_node;
        new_root->num_keys = 1;
        tree->root = new_root;
        tree->height++;
    }
}

// Sucht einen Schlüssel im B+ Tree (rekursiv)
static bool search_recursive(const bplus_tree_node* node, int key) {
    if (node == NULL) {
        return false;
    }

    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }

    if (i < node->num_keys && key == node->keys[i]) {
        return true;
    }

    if (node->is_leaf) {
        return false;
    }

    return search_recursive(node->children[i], key);
}

// Sucht einen Schlüssel im B+ Tree
bool bplus_tree_search(const bplus_tree* tree, int key) {
    if (tree == NULL || tree->root == NULL) {
        return false;
    }
    return search_recursive(tree->root, key);
}

// Hilfsfunktion: Merges two nodes (for deletion)
static void merge_nodes(bplus_tree_node* parent, int index, bplus_tree_node* left, bplus_tree_node* right) {
    if (left->is_leaf) {
        // Merge leaf nodes
        for (int i = 0; i < right->num_keys; i++) {
            left->keys[left->num_keys + i] = right->keys[i];
        }
        left->num_keys += right->num_keys;
        left->next = right->next;
    } else {
        // Merge inner nodes
        left->keys[left->num_keys] = parent->keys[index];
        left->num_keys++;
        for (int i = 0; i <= right->num_keys; i++) {
            left->children[left->num_keys + i] = right->children[i];
        }
        left->num_keys += right->num_keys;
    }
    free(right);

    // Shift keys and children in parent
    for (int i = index; i < parent->num_keys - 1; i++) {
        parent->keys[i] = parent->keys[i + 1];
        parent->children[i + 1] = parent->children[i + 2];
    }
    parent->num_keys--;
}

// Hilfsfunktion: Redistributes keys between two nodes (for deletion)
static void redistribute_nodes(bplus_tree_node* parent, int index, bplus_tree_node* left, bplus_tree_node* right) {
    if (left->is_leaf) {
        // Redistribute leaf nodes
        for (int i = right->num_keys - 1; i >= 0; i--) {
            right->keys[i + 1] = right->keys[i];
        }
        right->keys[0] = left->keys[left->num_keys - 1];
        left->num_keys--;
        parent->keys[index] = right->keys[0];
    } else {
        // Redistribute inner nodes
        right->children[right->num_keys + 1] = right->children[right->num_keys];
        for (int i = right->num_keys; i > 0; i--) {
            right->keys[i] = right->keys[i - 1];
            right->children[i] = right->children[i - 1];
        }
        right->keys[0] = parent->keys[index];
        right->children[0] = left->children[left->num_keys];
        left->num_keys--;
        parent->keys[index] = left->keys[left->num_keys];
    }
}

// Hilfsfunktion: Deletes a key from a node (recursively)
static bool delete_recursive(bplus_tree_node* node, int key) {
    if (node == NULL) {
        return false;
    }

    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }

    if (node->is_leaf) {
        if (i < node->num_keys && key == node->keys[i]) {
            // Key found in leaf node
            for (int j = i; j < node->num_keys - 1; j++) {
                node->keys[j] = node->keys[j + 1];
            }
            node->num_keys--;
            return true;
        }
        return false;
    }

    bool deleted = delete_recursive(node->children[i], key);
    if (!deleted) {
        return false;
    }

    // Check if the child needs merging or redistribution
    if (node->children[i]->num_keys < ORDER / 2) {
        if (i > 0 && node->children[i - 1]->num_keys > ORDER / 2) {
            // Borrow from left sibling
            redistribute_nodes(node, i - 1, node->children[i - 1], node->children[i]);
        } else if (i < node->num_keys && node->children[i + 1]->num_keys > ORDER / 2) {
            // Borrow from right sibling
            redistribute_nodes(node, i, node->children[i], node->children[i + 1]);
        } else {
            // Merge with sibling
            if (i < node->num_keys) {
                merge_nodes(node, i, node->children[i], node->children[i + 1]);
            } else {
                merge_nodes(node, i - 1, node->children[i - 1], node->children[i]);
            }
        }
    }

    return true;
}

// Löscht einen Schlüssel aus dem B+ Tree
void bplus_tree_delete(bplus_tree* tree, int key) {
    if (tree == NULL || tree->root == NULL) {
        return;
    }

    delete_recursive(tree->root, key);

    // Check if the root has only one child and is not a leaf
    if (!tree->root->is_leaf && tree->root->num_keys == 0) {
        bplus_tree_node* old_root = tree->root;
        tree->root = tree->root->children[0];
        free(old_root);
        tree->height--;
    }
}

// Hilfsfunktion: Traversiert den B+ Tree (rekursiv)
static void traverse_recursive(const bplus_tree_node* node) {
    if (node == NULL) {
        return;
    }

    if (node->is_leaf) {
        for (int i = 0; i < node->num_keys; i++) {
            printf("%d ", node->keys[i]);
        }
    } else {
        for (int i = 0; i <= node->num_keys; i++) {
            traverse_recursive(node->children[i]);
        }
    }
}

// Gibt alle Schlüssel in aufsteigender Reihenfolge aus (Traversierung)
void bplus_tree_traverse(const bplus_tree* tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("(leer)\n");
        return;
    }
    traverse_recursive(tree->root);
    printf("\n");
}
