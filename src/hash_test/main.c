
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

int main() {
    hash_table* table = hash_table_create(INITIAL_SIZE);

    // Füge viele Elemente ein, um die dynamische Anpassung zu testen
    for (int i = 0; i < 100; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        char* value = (char*) malloc(20 * sizeof(char));
        sprintf(value, "%d", i * 20);
        printf("insert: key %s, value %s\n", key, value);
        hash_table_insert(table, key, value);
    }

    // Überprüfe, ob die Tabelle dynamisch angepasst wurde
    printf("Größe der Hashtabelle: %lu\n", table->size);  // Sollte 160 sein (10 * 2^4)
    printf("Anzahl der Elemente: %lu\n", table->count);   // Sollte 100 sein

    // Suche nach einem Schlüssel
    printf("key50: %s\n", (char *) hash_table_get(table, "key50"));  // Ausgabe: 500

    // Lösche einen Schlüssel
    hash_table_delete(table, "key50");
    printf("key50 nach Löschen: %s\n", (char *)hash_table_get(table, "key50"));  // Ausgabe: -1

    // Speicher freigeben
    hash_table_free(table);

    return 0;
}

