#ifndef _STRING_VIEW_
#define _STRING_VIEW_

#include <stddef.h>  // Für size_t
#include <stdbool.h> // Für bool

// string_view-Struktur
typedef struct {
    const char* data;  // Zeiger auf die Zeichenkette
    size_t length;     // Länge der Zeichenkette
} string_view;

// Erstellt einen string_view aus einer null-terminierten Zeichenkette
string_view string_view_from_cstr(const char* cstr);

// Erstellt einen string_view aus einem Zeichen und einer Länge
string_view string_view_from_chars(const char* chars, size_t length);

// Gibt die Länge des string_views zurück
size_t string_view_length(const string_view* sv);

// Gibt den C-String (null-terminiert) zurück
const char* string_view_to_cstr(const string_view* sv);

// Vergleicht zwei string_views
int string_view_compare(const string_view* sv1, const string_view* sv2);

// Gibt einen Substring als neuen string_view zurück
string_view string_view_substr(const string_view* sv, size_t start, size_t length);

// Überprüft, ob ein string_view leer ist
bool string_view_is_empty(const string_view* sv);

// Zerstört den string_view (in C nicht nötig, aber für Konsistenz)
void string_view_destroy(string_view* sv);

#endif // STRING_VIEW_H