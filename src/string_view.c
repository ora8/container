#include "string_view.h"
#include <string.h>
#include <stdlib.h>

// Erstellt einen string_view aus einer null-terminierten Zeichenkette
string_view string_view_from_cstr(const char* cstr) {
    if (cstr == NULL) {
        return (string_view){NULL, 0};
    }
    return (string_view){cstr, strlen(cstr)};
}

// Erstellt einen string_view aus einem Zeichen und einer Länge
string_view string_view_from_chars(const char* chars, size_t length) {
    if (chars == NULL) {
        return (string_view){NULL, 0};
    }
    return (string_view){chars, length};
}

// Gibt die Länge des string_views zurück
size_t string_view_length(const string_view* sv) {
    if (sv == NULL) {
        return 0;
    }
    return sv->length;
}

// Gibt den C-String (null-terminiert) zurück
const char* string_view_to_cstr(const string_view* sv) {
    if (sv == NULL || sv->data == NULL) {
        return "";
    }
    // Erstelle eine null-terminierte Kopie (Achtung: Speicherverwaltung!)
    char* cstr = (char*)malloc(sv->length + 1);
    if (cstr == NULL) {
        return NULL;
    }
    memcpy(cstr, sv->data, sv->length);
    cstr[sv->length] = '\0';
    return cstr;
}

// Vergleicht zwei string_views
int string_view_compare(const string_view* sv1, const string_view* sv2) {
    if (sv1 == NULL || sv2 == NULL) {
        return -1;
    }
    if (sv1->length != sv2->length) {
        return (int)(sv1->length - sv2->length);
    }
    return strncmp(sv1->data, sv2->data, sv1->length);
}

// Gibt einen Substring als neuen string_view zurück
string_view string_view_substr(const string_view* sv, size_t start, size_t length) {
    if (sv == NULL || sv->data == NULL || start >= sv->length) {
        return (string_view){NULL, 0};
    }
    if (start + length > sv->length) {
        length = sv->length - start;
    }
    return (string_view){sv->data + start, length};
}

// Überprüft, ob ein string_view leer ist
bool string_view_is_empty(const string_view* sv) {
    return (sv == NULL || sv->length == 0 || sv->data == NULL);
}

// Zerstört den string_view (in C nicht nötig, aber für Konsistenz)
void string_view_destroy(string_view* sv) {
    // In C gibt es nichts zu zerstören, aber die Funktion bleibt für Konsistenz
    (void)sv;
}
