#ifndef STRING_SLICE_H
#define STRING_SLICE_H

#include <stddef.h> // Für size_t

// Strukturdefinition
struct string_slice {
    char *begin;  // Zeiger auf den Anfang des Slices
    char *end;    // Zeiger auf das Ende des Slices (exklusiv)
};

// Funktion zur Erstellung eines Slices
struct string_slice string_slice_create(char *begin, char *end);

// Funktion zur Berechnung der Länge eines Slices
size_t string_slice_length(struct string_slice slice);

// Funktion zum Kopieren eines Slices in einen Puffer
void string_slice_copy(struct string_slice slice, char *dest);

// Funktion zum Vergleichen zweier Slices
int string_slice_equals(struct string_slice a, struct string_slice b);

// Funktion zur Erstellung eines Teil-Slices
struct string_slice string_slice_substring(struct string_slice slice, size_t offset, size_t len);


#endif // STRING_SLICE_H
