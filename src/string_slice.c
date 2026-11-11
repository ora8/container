#include "string_slice.h"
#include <string.h> // Für strncpy und strncmp

// Funktion zur Erstellung eines Slices
struct string_slice string_slice_create(char *begin, char *end) {
    struct string_slice slice;
    slice.begin = begin;
    slice.end = end;
    return slice;
}

// Funktion zur Berechnung der Länge eines Slices
size_t string_slice_length(struct string_slice slice) {
    return slice.end - slice.begin;
}

// Funktion zum Kopieren eines Slices in einen Puffer
void string_slice_copy(struct string_slice slice, char *dest) {
    size_t len = string_slice_length(slice);
    strncpy(dest, slice.begin, len);
    dest[len] = '\0'; // Nullterminierung sicherstellen
}

// Funktion zum Vergleichen zweier Slices
int string_slice_equals(struct string_slice a, struct string_slice b) {
    if (string_slice_length(a) != string_slice_length(b)) {
        return 0;
    }
    return memcmp(a.begin, b.begin, string_slice_length(a)) == 0;
}

// Funktion zur Erstellung eines Teil-Slices
struct string_slice string_slice_substring(struct string_slice slice, size_t offset, size_t len) {
    if (offset + len > string_slice_length(slice)) {
        // Bereichsfehler: Slice ist zu kurz
        return string_slice_create(NULL, NULL);
    }
    return string_slice_create(slice.begin + offset, slice.begin + offset + len);
}