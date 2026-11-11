#include <stdio.h>
#include "string_slice.h"

int main() {
    char text[] = "Hallo Welt";

    // Slice erstellen
    struct string_slice slice = string_slice_create(text, text + 5); // "Hallo"

    // Länge des Slices
    printf("Länge: %zu\n", string_slice_length(slice)); // 5

    // Slice kopieren
    char buffer[10];
    string_slice_copy(slice, buffer);
    printf("Kopie: %s\n", buffer); // "Hallo"

    // Teil-Slice erstellen
    struct string_slice sub_slice = string_slice_substring(slice, 1, 3); // "all"
    printf("Teil-Slice: ");
    for (char *p = sub_slice.begin; p < sub_slice.end; p++) {
        putchar(*p);
    }
    putchar('\n'); // "all"

    return 0;
}
