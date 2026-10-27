#ifndef STR_H
#define STR_H

#include <stddef.h>  // Für size_t
#include <stdbool.h> // Für bool
#include <string.h>  // Für strlen, strcpy, etc.

// Maximale Anzahl von Zeichen, die auf dem Stack gespeichert werden können
#define str_SSO_MAX 23

// Struktur für einen C-String mit Small String Optimization (SSO)
typedef struct {
    union {
        char stack_data[str_SSO_MAX + 1]; // Platz für SSO (inkl. '\0')
        char* heap_data;                       // Zeiger auf Heap-Daten
    } data;
    bool is_heap;    // Flag: true = Heap, false = Stack
    size_t size;   // Länge der Zeichenkette (ohne '\0')
} str;

// Erstellt einen leeren str (verwendet SSO)
str str_create();

// Erstellt einen str aus einer C-Zeichenkette (verwendet SSO, falls möglich)
str str_create_from_cstr(const char* cstr);

// Erstellt einen str mit einer bestimmten Kapazität (erzwingt Heap-Nutzung)
str str_create_with_capacity(size_t capacity);

// Kopiert einen str (verwendet SSO, falls möglich)
str str_copy(const str* str);

// Zerstört einen str und gibt den Speicher frei (falls Heap genutzt wurde)
void str_destroy(str* str);

// Gibt die Zeichenkette als C-String zurück (null-terminiert)
const char* str_cstr(const str* str);

// Gibt die Länge der Zeichenkette zurück
size_t str_size(const str* str);

// Prüft, ob der String leer ist
bool str_empty(const str* str);

// Fügt eine C-Zeichenkette an den String an (verwendet SSO oder Heap)
void str_append_cstr(str* str, const char* cstr);

// Fügt einen einzelnen Buchstaben an den String an
void str_append_char(str* str, char c);

// Vergleicht zwei Strings (wie strcmp)
int str_compare(const str* str1, const str* str2);

// Gibt einen Substring zurück (neuer str, verwendet SSO oder Heap)
str str_substr(const str* str, size_t start, size_t length);

// Löscht einen Teil des Strings
void str_erase(str* str, size_t start, size_t length);

#endif // str_H
