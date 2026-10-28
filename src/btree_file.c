#include "btree_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Größe eines Blocks in der Datei
#define BLOCK_SIZE sizeof(btree_node)

// Hilfsfunktion: Liest einen Block aus der Datei
static void read_block(FILE* file, int block_num, btree_node* node) {
    fseek(file, block_num * BLOCK_SIZE, SEEK_SET);
    size_t read = fread(node, BLOCK_SIZE, 1, file);
    (void)read;
}

// Hilfsfunktion: Schreibt einen Block in die Datei
static void write_block(FILE* file, int block_num, const btree_node* node) {
    fseek(file, block_num * BLOCK_SIZE, SEEK_SET);
    fwrite(node, BLOCK_SIZE, 1, file);
    fflush(file);
}

// Hilfsfunktion: Alloziert einen neuen Block in der Datei
static int allocate_block(FILE* file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    int new_block = file_size / BLOCK_SIZE;
    btree_node empty_node = {0};
    write_block(file, new_block, &empty_node);
    return new_block;
}

// Hilfsfunktion: Gibt einen Block frei (fügt ihn zur Freiliste hinzu)
static void free_block(FILE* file, int block_num, btree_meta_data* metadata) {
    btree_node free_node = {0};
    free_node.block_num = block_num;
    free_node.children[0] = metadata->free_list_head;
    write_block(file, block_num, &free_node);
    metadata->free_list_head = block_num;
}

// Hilfsfunktion: Gibt den nächsten freien Block zurück
static int get_free_block(FILE* file, btree_meta_data* metadata) {
    if (metadata->free_list_head != -1) {
        btree_node free_node;
        read_block(file, metadata->free_list_head, &free_node);
        int block_num = metadata->free_list_head;
        metadata->free_list_head = free_node.children[0];
        return block_num;
    } else {
        return allocate_block(file);
    }
}

// Hilfsfunktion: Liest die Metadaten aus der Datei
static void read_metadata(FILE* file, btree_meta_data* metadata) {
    fseek(file, 0, SEEK_SET);
    size_t read = fread(metadata, sizeof(btree_meta_data), 1, file);
    (void)read;
}

// Hilfsfunktion: Schreibt die Metadaten in die Datei
static void write_metadata(FILE* file, const btree_meta_data* metadata) {
    fseek(file, 0, SEEK_SET);
    fwrite(metadata, sizeof(btree_meta_data), 1, file);
    fflush(file);
}

// Hilfsfunktion: Erstellt einen neuen B-Tree (initialisiert die Datei)
static void create_new_tree(FILE* file) {
    btree_meta_data metadata = {0};
    metadata.root_block = -1;
    metadata.height = 0;
    metadata.num_nodes = 0;
    metadata.free_list_head = -1;
    write_metadata(file, &metadata);
}

// Hilfsfunktion: Erstellt einen neuen Knoten
static btree_node* create_node(bool is_leaf) {
    btree_node* node = (btree_node*)malloc(sizeof(btree_node));
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(btree_node));
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->block_num = -1;
    return node;
}

// Hilfsfunktion: Zerstört einen Knoten
static void destroy_node(btree_node* node) {
    free(node);
}

// Hilfsfunktion: Teilt einen vollen Knoten
static void split_node(btree_file* tree, btree_node* parent, int child_index, btree_node* child) {
    (void)child_index; // Unterdrückt die Warnung, falls der Parameter nicht benötigt wird

    btree_node* new_node = create_node(child->is_leaf);
    if (new_node == NULL) {
        return;
    }

    int split_pos = child->num_keys / 2;
    int promoted_key = child->keys[split_pos];

    // Kopiere die zweite Hälfte der Schlüssel und Kinder in den neuen Knoten
    for (int i = split_pos; i < ORDER; i++) {
        if (child->is_leaf) {
            new_node->keys[i - split_pos] = child->keys[i];
        } else {
            new_node->keys[i - split_pos] = child->keys[i];
            new_node->children[i - split_pos] = child->children[i];
        }
    }

    if (!child->is_leaf) {
        new_node->children[ORDER - split_pos] = child->children[ORDER];
    }

    // Aktualisiere die Anzahl der Schlüssel im alten Knoten
    child->num_keys = split_pos;
    new_node->num_keys = ORDER - split_pos - (child->is_leaf ? 0 : 1);

    // Alloziere Block für den neuen Knoten
    new_node->block_num = get_free_block(tree->file, &tree->metadata);
    tree->metadata.num_nodes++;

    // Schreibe die Knoten zurück in die Datei
    write_block(tree->file, child->block_num, child);
    write_block(tree->file, new_node->block_num, new_node);

    // Füge den neuen Schlüssel und den neuen Knoten in den Elternknoten ein
    if (parent == NULL) {
        // Erstelle einen neuen Wurzelknoten
        btree_node* new_root = create_node(false);
        if (new_root == NULL) {
            destroy_node(new_node);
            return;
        }

        new_root->keys[0] = promoted_key;
        new_root->children[0] = child->block_num;
        new_root->children[1] = new_node->block_num;
        new_root->num_keys = 1;

        new_root->block_num = get_free_block(tree->file, &tree->metadata);
        tree->metadata.root_block = new_root->block_num;
        tree->metadata.height++;

        write_block(tree->file, new_root->block_num, new_root);
        destroy_node(new_root);
    } else {
        // Füge den neuen Schlüssel und den neuen Knoten in den Elternknoten ein
        int i = parent->num_keys - 1;
        while (i >= 0 && promoted_key < parent->keys[i]) {
            parent->keys[i + 1] = parent->keys[i];
            parent->children[i + 2] = parent->children[i + 1];
            i--;
        }
        parent->keys[i + 1] = promoted_key;
        parent->children[i + 2] = new_node->block_num;
        parent->num_keys++;

        write_block(tree->file, parent->block_num, parent);
    }

    destroy_node(new_node);
}

// Hilfsfunktion: Fügt einen Schlüssel in einen Knoten ein (sortiert)
static void insert_into_node(btree_node* node, int key, int child_block) {
    int i = node->num_keys - 1;
    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->children[i + 2] = node->children[i + 1];
            i--;
        }
        node->keys[i + 1] = key;
        node->children[i + 2] = child_block;
    }
    node->num_keys++;
}

// Hilfsfunktion: Fügt einen Schlüssel in den B-Tree ein (rekursiv)
static void insert_recursive(btree_file* tree, int block_num, int key) {
    btree_node node;
    read_block(tree->file, block_num, &node);

    int i = 0;
    while (i < node.num_keys && key > node.keys[i]) {
        i++;
    }

    if (i < node.num_keys && key == node.keys[i]) {
        // Schlüssel existiert bereits
        return;
    }

    if (node.is_leaf) {
        insert_into_node(&node, key, -1);
        if (node.num_keys == ORDER) {
            split_node(tree, NULL, -1, &node);
        } else {
            write_block(tree->file, block_num, &node);
        }
    } else {
        insert_recursive(tree, node.children[i], key);
        read_block(tree->file, block_num, &node); // Knoten könnte sich durch Split geändert haben
        if (node.num_keys == ORDER) {
            split_node(tree, &node, i, NULL);
        } else {
            write_block(tree->file, block_num, &node);
        }
    }
}

// Hilfsfunktion: Sucht einen Schlüssel im B-Tree (rekursiv)
static bool search_recursive(const btree_file* tree, int block_num, int key) {
    btree_node node;
    read_block(tree->file, block_num, &node);

    int i = 0;
    while (i < node.num_keys && key > node.keys[i]) {
        i++;
    }

    if (i < node.num_keys && key == node.keys[i]) {
        return true;
    }

    if (node.is_leaf) {
        return false;
    }

    return search_recursive(tree, node.children[i], key);
}

// // Hilfsfunktion: Finds the minimum key in a subtree (for deletion)
// static int find_min_key(btree_file* tree, int block_num) {
//     btree_node node;
//     read_block(tree->file, block_num, &node);

//     while (!node.is_leaf) {
//         read_block(tree->file, node.children[0], &node);
//     }
//     return node.keys[0];
// }

// Hilfsfunktion: Merges two nodes (for deletion)
static void merge_nodes(btree_file* tree, btree_node* parent, int index, btree_node* left, btree_node* right) {
    if (left->is_leaf) {
        // Merge leaf nodes
        for (int i = 0; i < right->num_keys; i++) {
            left->keys[left->num_keys + i] = right->keys[i];
        }
        left->num_keys += right->num_keys;
    } else {
        // Merge inner nodes
        left->keys[left->num_keys] = parent->keys[index];
        left->num_keys++;
        for (int i = 0; i <= right->num_keys; i++) {
            left->children[left->num_keys + i] = right->children[i];
        }
        left->num_keys += right->num_keys;
    }

    // Free the right node
    free_block(tree->file, right->block_num, &tree->metadata);
    tree->metadata.num_nodes--;

    // Shift keys and children in parent
    for (int i = index; i < parent->num_keys - 1; i++) {
        parent->keys[i] = parent->keys[i + 1];
        parent->children[i + 1] = parent->children[i + 2];
    }
    parent->num_keys--;
}

// Hilfsfunktion: Redistributes keys between two nodes (for deletion)
static void redistribute_nodes(btree_file* tree, btree_node* parent, int index, btree_node* left, btree_node* right) {
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

    write_block(tree->file, left->block_num, left);
    write_block(tree->file, right->block_num, right);
    write_block(tree->file, parent->block_num, parent);
}

// Hilfsfunktion: Deletes a key from a node (recursively)
static bool delete_recursive(btree_file* tree, int block_num, int key) {
    btree_node node;
    read_block(tree->file, block_num, &node);

    int i = 0;
    while (i < node.num_keys && key > node.keys[i]) {
        i++;
    }

    if (node.is_leaf) {
        if (i < node.num_keys && key == node.keys[i]) {
            // Key found in leaf node
            for (int j = i; j < node.num_keys - 1; j++) {
                node.keys[j] = node.keys[j + 1];
            }
            node.num_keys--;
            write_block(tree->file, block_num, &node);
            return true;
        }
        return false;
    }

    // Lese den Kindknoten
    btree_node child;
    read_block(tree->file, node.children[i], &child);

    bool deleted = delete_recursive(tree, node.children[i], key);
    if (!deleted) {
        return false;
    }

    // Lese den Elternknoten erneut (könnte sich durch Löschen geändert haben)
    read_block(tree->file, block_num, &node);

    // Prüfe, ob der Kindknoten zu leer ist und eine Neuverteilung oder Verschmelzung nötig ist
    if (node.children[i] != -1) {
        btree_node left_child, right_child;
        read_block(tree->file, node.children[i], &left_child);

        if (i + 1 <= node.num_keys && node.children[i + 1] != -1) {
            read_block(tree->file, node.children[i + 1], &right_child);

            if (left_child.num_keys < ORDER / 2) {
                if (i > 0 && node.children[i - 1] != -1) {
                    btree_node left_sibling;
                    read_block(tree->file, node.children[i - 1], &left_sibling);
                    if (left_sibling.num_keys > ORDER / 2) {
                        // Leihe vom linken Geschwisterknoten
                        redistribute_nodes(tree, &node, i - 1, &left_sibling, &left_child);
                        write_block(tree->file, node.children[i - 1], &left_sibling);
                        write_block(tree->file, node.children[i], &left_child);
                        write_block(tree->file, block_num, &node);
                    } else {
                        // Verschmelze mit dem linken Geschwisterknoten
                        merge_nodes(tree, &node, i - 1, &left_sibling, &left_child);
                        write_block(tree->file, node.children[i - 1], &left_sibling);
                        write_block(tree->file, block_num, &node);
                    }
                } else if (right_child.num_keys > ORDER / 2) {
                    // Leihe vom rechten Geschwisterknoten
                    redistribute_nodes(tree, &node, i, &left_child, &right_child);
                    write_block(tree->file, node.children[i], &left_child);
                    write_block(tree->file, node.children[i + 1], &right_child);
                    write_block(tree->file, block_num, &node);
                } else {
                    // Verschmelze mit dem rechten Geschwisterknoten
                    merge_nodes(tree, &node, i, &left_child, &right_child);
                    write_block(tree->file, node.children[i], &left_child);
                    write_block(tree->file, block_num, &node);
                }
            }
        }
    }

    return true;
}


// Öffnet einen B-Tree in einer Datei (erstellt eine neue Datei, falls sie nicht existiert)
btree_file* btree_file_open(const char* filename) {
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        file = fopen(filename, "wb+");
        if (file == NULL) {
            return NULL;
        }
        create_new_tree(file);
    }

    btree_file* tree = (btree_file*)malloc(sizeof(btree_file));
    if (tree == NULL) {
        fclose(file);
        return NULL;
    }

    tree->file = file;
    read_metadata(file, &tree->metadata);

    return tree;
}

// Schließt den B-Tree und gibt die Datei frei
void btree_file_close(btree_file* tree) {
    if (tree != NULL) {
        write_metadata(tree->file, &tree->metadata);
        fclose(tree->file);
        free(tree);
    }
}

// Fügt einen Schlüssel in den B-Tree ein
void btree_file_insert(btree_file* tree, int key) {
    if (tree == NULL || tree->file == NULL) {
        return;
    }

    if (tree->metadata.root_block == -1) {
        // Leerer Baum: Erstelle einen neuen Wurzelknoten
        btree_node* root = create_node(true);
        if (root == NULL) {
            return;
        }

        root->keys[0] = key;
        root->num_keys = 1;

        root->block_num = get_free_block(tree->file, &tree->metadata);
        tree->metadata.root_block = root->block_num;
        tree->metadata.height = 1;
        tree->metadata.num_nodes = 1;

        write_block(tree->file, root->block_num, root);
        destroy_node(root);
    } else {
        insert_recursive(tree, tree->metadata.root_block, key);
    }
}

// Sucht einen Schlüssel im B-Tree
bool btree_file_search(const btree_file* tree, int key) {
    if (tree == NULL || tree->file == NULL || tree->metadata.root_block == -1) {
        return false;
    }
    return search_recursive(tree, tree->metadata.root_block, key);
}

// Löscht einen Schlüssel aus dem B-Tree
void btree_file_delete(btree_file* tree, int key) {
    if (tree == NULL || tree->file == NULL || tree->metadata.root_block == -1) {
        return;
    }

    delete_recursive(tree, tree->metadata.root_block, key);

    // Check if the root has only one child and is not a leaf
    btree_node root;
    read_block(tree->file, tree->metadata.root_block, &root);
    if (!root.is_leaf && root.num_keys == 0) {
        btree_node child;
        read_block(tree->file, root.children[0], &child);
        free_block(tree->file, tree->metadata.root_block, &tree->metadata);
        tree->metadata.root_block = child.block_num;
        tree->metadata.height--;
    }
}

// Hilfsfunktion: Traversiert den B-Tree (rekursiv)
static void traverse_recursive(const btree_file* tree, int block_num) {
    btree_node node;
    read_block(tree->file, block_num, &node);

    if (node.is_leaf) {
        for (int i = 0; i < node.num_keys; i++) {
            printf("%d ", node.keys[i]);
        }
    } else {
        for (int i = 0; i <= node.num_keys; i++) {
            traverse_recursive(tree, node.children[i]);
        }
    }
}

// Gibt alle Schlüssel in aufsteigender Reihenfolge aus (Traversierung)
void btree_file_traverse(const btree_file* tree) {
    if (tree == NULL || tree->file == NULL || tree->metadata.root_block == -1) {
        printf("(leer)\n");
        return;
    }
    traverse_recursive(tree, tree->metadata.root_block);
    printf("\n");
}
