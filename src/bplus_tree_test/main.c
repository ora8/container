#include "bplus_tree.h"
#include <stdio.h>

int main() {
    bplus_tree* tree = bplus_tree_create();
    if (tree == NULL) {
        printf("Fehler: Baum konnte nicht erstellt werden.\n");
        return 1;
    }

    // Schlüssel einfügen
    printf("Einfügen von Schlüsseln: 10, 20, 5, 15, 30, 25, 40\n");
    bplus_tree_insert(tree, 10);
    bplus_tree_insert(tree, 20);
    bplus_tree_insert(tree, 5);
    bplus_tree_insert(tree, 15);
    bplus_tree_insert(tree, 30);
    bplus_tree_insert(tree, 25);
    bplus_tree_insert(tree, 40);

    // Traversierung
    printf("Traversierung: ");
    bplus_tree_traverse(tree);

    // Suche
    printf("Suche nach 15: %s\n", bplus_tree_search(tree, 15) ? "Gefunden" : "Nicht gefunden");
    printf("Suche nach 27: %s\n", bplus_tree_search(tree, 27) ? "Gefunden" : "Nicht gefunden");

    // Löschen
    printf("\nLöschen von 20\n");
    bplus_tree_delete(tree, 20);
    printf("Traversierung nach Löschen: ");
    bplus_tree_traverse(tree);

    // Zerstören des Baumes
    bplus_tree_destroy(tree);

    return 0;
}
