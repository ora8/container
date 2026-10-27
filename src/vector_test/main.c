#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"


// Hilfsfunktion zum Ausgeben des vectors (für int-Werte)
void print_vector_int(const vector* vec) {
    printf("vector (Größe: %zu, Kapazität: %zu): [", vec->size, vec->capacity);
    for (size_t i = 0; i < vec->size; i++) {
        printf("%d", *(int*)vec->data[i]);
        if (i < vec->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Hilfsfunktion zum Ausgeben des vectors (für Strings)
void print_vector_string(const vector* vec) {
    printf("vector (Größe: %zu, Kapazität: %zu): [", vec->size, vec->capacity);
    for (size_t i = 0; i < vec->size; i++) {
        printf("\"%s\"", (char*)vec->data[i]);
        if (i < vec->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    // vector für Integer erstellen
    vector* int_vector = vector_create(5);
    if (int_vector == NULL) {
        fprintf(stderr, "Fehler: vector konnte nicht erstellt werden.\n");
        return 1;
    }

    // Integer zum vector hinzufügen
    int a = 10, b = 20, c = 30;     // Spezieller int_vector verwenden.
    vector_push_back(int_vector, &a);
    vector_push_back(int_vector, &b);
    vector_push_back(int_vector, &c);

    printf("Position von element %d ist %ld\n", b, vector_find(int_vector, &b));
    // vector ausgeben
    print_vector_int(int_vector);

    // Letztes Element entfernen
    vector_pop_back(int_vector);
    print_vector_int(int_vector);

    // Element an Index 1 abrufen
    int* element = (int*)vector_get(int_vector, 1);
    if (element != NULL) {
        printf("Element an Index 1: %d\n", *element);
    }

    // vector für Strings erstellen
    vector* string_vector = vector_create(3);
    if (string_vector == NULL) {
        fprintf(stderr, "Fehler: vector konnte nicht erstellt werden.\n");
        return 1;
    }

    // Strings zum vector hinzufügen
    char* str1 = strdup("Hallo");
    char* str2 = strdup("Welt");
    char* str3 = strdup("!");
    vector_push_back(string_vector, str1);
    vector_push_back(string_vector, str2);
    vector_push_back(string_vector, str3);

    // vector ausgeben
    print_vector_string(string_vector);

    // vector löschen
    vector_clear(string_vector);
    printf("vector nach clear (Größe: %zu)\n", vector_size(string_vector));

    // Speicher freigeben
    vector_free(int_vector);
    vector_free(string_vector);
    free(str1);
    free(str2);
    free(str3);

    return 0;
}
