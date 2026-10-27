#include <stdio.h>

#include "btree_file.h"

int main() {
    const char* filename = "btree.dat";

    // Öffne den B-Tree
    btree_file* tree = btree_file_open(filename);
    if (tree == NULL) {
        printf("Fehler: Baum konnte nicht geöffnet werden.\n");
        return 1;
    }

    // Schlüssel einfügen
    printf("Einfügen von Schlüsseln: 10, 20, 5, 15, 30, 25, 40\n");
    btree_file_insert(tree, 10);
    btree_file_insert(tree, 20);
    btree_file_insert(tree, 5);
    btree_file_insert(tree, 15);
    btree_file_insert(tree, 30);
    btree_file_insert(tree, 25);
    btree_file_insert(tree, 40);

    // Traversierung
    printf("Traversierung: ");
    btree_file_traverse(tree);

    // Suche
    printf("Suche nach 15: %s\n", btree_file_search(tree, 15) ? "Gefunden" : "Nicht gefunden");
    printf("Suche nach 27: %s\n", btree_file_search(tree, 27) ? "Gefunden" : "Nicht gefunden");

    // Löschen
    printf("\nLöschen von 20\n");
    btree_file_delete(tree, 20);
    printf("Traversierung nach Löschen: ");
    btree_file_traverse(tree);

    // Baum schließen
    btree_file_close(tree);

    return 0;
}
