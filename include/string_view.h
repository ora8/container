#ifndef STRING_VIEW
#define STRING_VIEW

#include <stddef.h>  // Für size_t
#include <stdbool.h> // Für bool

// string_view-Struktur
typedef struct {
    const char* data;  // Zeiger auf die Zeichenkette
    size_t size;     // Länge der Zeichenkette
} string_view;

// Erstellt einen string_view aus einer null-terminierten Zeichenkette
string_view string_view_from_cstr(const char cstr[]);

// Erstellt einen string_view aus einem Zeichen und einer Länge
string_view string_view_from_chars(const char* chars, size_t size);

// Gibt die Länge des string_views zurück
size_t string_view_size(const string_view* sv);

// Vergleicht zwei string_views
int string_view_compare(const string_view* sv1, const string_view* sv2);

// Gibt einen Substring als neuen string_view zurück
string_view string_view_substr(const string_view* sv, size_t start, size_t size);

// Returns a substring of a C-string as a new string_view
string_view string_view_cstr_substr(const char *str, size_t start, size_t size);

// Überprüft, ob ein string_view leer ist
bool string_view_is_empty(const string_view* sv);

#endif // STRING_VIEW_H