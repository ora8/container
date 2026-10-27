#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include <stdbool.h>
#include <stddef.h>

// Maximale Anzahl von Schlüsseln in einem Knoten
#define ORDER 5

// Struktur für einen B+ Tree Knoten
typedef struct bplus_tree_node {
    bool is_leaf;                    // true, wenn es sich um einen Blattknoten handelt
    int keys[ORDER];                 // Array der Schlüssel
    struct bplus_tree_node* children[ORDER + 1]; // Zeiger auf Kindknoten (für innere Knoten)
    struct bplus_tree_node* next;      // Zeiger auf den nächsten Blattknoten (nur für Blattknoten)
    int num_keys;                    // Anzahl der aktuellen Schlüssel
} bplus_tree_node;

// Struktur für einen B+ Tree
typedef struct {
    bplus_tree_node* root;             // Wurzelknoten
    int height;                      // Höhe des Baumes
} bplus_tree;

// Erstellt einen neuen B+ Tree
bplus_tree* bplus_tree_create();

// Zerstört einen B+ Tree und gibt alle Knoten frei
void bplus_tree_destroy(bplus_tree* tree);

// Fügt einen Schlüssel in den B+ Tree ein
void bplus_tree_insert(bplus_tree* tree, int key);

// Sucht einen Schlüssel im B+ Tree
bool bplus_tree_search(const bplus_tree* tree, int key);

// Löscht einen Schlüssel aus dem B+ Tree
void bplus_tree_delete(bplus_tree* tree, int key);

// Gibt alle Schlüssel in aufsteigender Reihenfolge aus (Traversierung)
void bplus_tree_traverse(const bplus_tree* tree);

#endif // BPLUS_TREE_H
