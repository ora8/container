#include "string_view.h"
#include <stdlib.h>
#include <string.h>

// Erstellt einen string_view aus einer null-terminierten Zeichenkette
string_view string_view_from_cstr(const char cstr[]) {
  if (cstr == NULL) {
    return (string_view){NULL, 0};
  }
  return (string_view){cstr, strlen(cstr)};
}

// Erstellt einen string_view aus einem Zeichen und einer Länge
string_view string_view_from_chars(const char *chars, size_t size) {
  if (chars == NULL) {
    return (string_view){NULL, 0};
  }
  return (string_view){chars, size};
}

// Gibt die Länge des string_views zurück
size_t string_view_size(const string_view *sv) { return sv->size; }

// Vergleicht zwei string_views
int string_view_compare(const string_view *sv1, const string_view *sv2) {
  if (sv1->size != sv2->size) {
    return (int)(sv1->size - sv2->size);
  }
  return memcmp(sv1->data, sv2->data, sv1->size);
}

// Gibt einen Substring als neuen string_view zurück
string_view string_view_substr(const string_view *sv, size_t start,
                               size_t size) {
  if (sv->data == NULL || start >= sv->size) {
    return (string_view){NULL, 0};
  }
  if (start + size > sv->size) {
    size = sv->size - start;
  }
  return (string_view){sv->data + start, size};
}

// Returns a substring of a C-string as a new string_view
string_view string_view_cstr_substr(const char *str, size_t start,
                                    size_t size) {
  size_t str_size = strlen(str);
  if (str == NULL || start >= str_size) {
    return (string_view){NULL, 0};
  }
  if (start + size > str_size) {
    size = str_size - start;
  }
  return (string_view){str + start, size};
}

// Überprüft, ob ein string_view leer ist
bool string_view_is_empty(const string_view *sv) {
  return (sv->size == 0 || sv->data == NULL);
}