#ifndef BTREE_FILE_H
#define BTREE_FILE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Maximale Anzahl von Schlüsseln in einem Knoten
#define ORDER 5

// Struktur für einen B-Tree Knoten (Block in der Datei)
typedef struct {
    int keys[ORDER];                 // Array der Schlüssel
    int children[ORDER + 1];         // Blocknummern der Kinder (für innere Knoten)
    int num_keys;                    // Anzahl der aktuellen Schlüssel
    bool is_leaf;                    // true, wenn es sich um einen Blattknoten handelt
    int block_num;                   // Blocknummer in der Datei
} btree_node;

// Struktur für die Metadaten des B-Trees
typedef struct {
    int root_block;                  // Blocknummer des Wurzelknotens
    int height;                      // Höhe des Baumes
    int num_nodes;                   // Anzahl der Knoten
    int free_list_head;              // Kopf der Freiliste
} btree_meta_data;

// Struktur für den B-Tree auf Datei
typedef struct {
    FILE* file;                      // Dateizeiger
    btree_meta_data metadata;          // Metadaten
} btree_file;

// Öffnet einen B-Tree in einer Datei (erstellt eine neue Datei, falls sie nicht existiert)
btree_file* btree_file_open(const char* filename);

// Schließt den B-Tree und gibt die Datei frei
void btree_file_close(btree_file* tree);

// Fügt einen Schlüssel in den B-Tree ein
void btree_file_insert(btree_file* tree, int key);

// Sucht einen Schlüssel im B-Tree
bool btree_file_search(const btree_file* tree, int key);

// Löscht einen Schlüssel aus dem B-Tree
void btree_file_delete(btree_file* tree, int key);

// Gibt alle Schlüssel in aufsteigender Reihenfolge aus (Traversierung)
void btree_file_traverse(const btree_file* tree);

#endif // BTREE_FILE_H
