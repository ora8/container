#include "str.h"
#include <stdlib.h>
#include <stdio.h>

// Hilfsfunktion: Prüft, ob ein String auf dem Stack gespeichert werden kann
static bool str_can_use_sso(size_t size) {
    return size <= str_SSO_MAX;
}

// Hilfsfunktion: Alloziert Heap-Speicher und kopiert die Daten
static void str_allocate_heap(str* str, size_t size) {
    str->data.heap_data = (char*)malloc(size + 1);
    if (str->data.heap_data == NULL) {
        str->size = 0;
        str->is_heap = false;
        return;
    }
    str->is_heap = true;
}

// Hilfsfunktion: Kopiert Daten in den Stack oder Heap
static void str_copy_data(str* dest, const char* src, size_t size) {
    if (str_can_use_sso(size)) {
        memcpy(dest->data.stack_data, src, size);
        dest->data.stack_data[size] = '\0';
        dest->is_heap = false;
    } else {
        str_allocate_heap(dest, size);
        if (dest->is_heap) {
            memcpy(dest->data.heap_data, src, size);
            dest->data.heap_data[size] = '\0';
        }
    }
    dest->size = size;
}

// Erstellt einen leeren str (verwendet SSO)
str str_create() {
    str str;
    str.data.stack_data[0] = '\0';
    str.size = 0;
    str.is_heap = false;
    return str;
}

// Erstellt einen str aus einer C-Zeichenkette (verwendet SSO, falls möglich)
str str_create_from_cstr(const char* cstr) {
    if (cstr == NULL) {
        return str_create();
    }
    size_t size = strlen(cstr);
    str str;
    str_copy_data(&str, cstr, size);
    return str;
}

// Erstellt einen str mit einer bestimmten Kapazität (erzwingt Heap-Nutzung)
str str_create_with_capacity(size_t capacity) {
    str str = str_create();
    if (capacity > str_SSO_MAX) {
        str_allocate_heap(&str, capacity);
    }
    return str;
}

// Kopiert einen str (verwendet SSO, falls möglich)
str str_copy(const str* str_) {
    if (str_ == NULL) {
        return str_create();
    }
    str copy;
    str_copy_data(&copy, str_cstr(str_), str_->size);
    return copy;
}

// Zerstört einen str und gibt den Speicher frei (falls Heap genutzt wurde)
void str_destroy(str* str) {
    if (str == NULL) {
        return;
    }
    if (str->is_heap) {
        free(str->data.heap_data);
        str->data.heap_data = NULL;
    }
    str->size = 0;
    str->is_heap = false;
}

// Gibt die Zeichenkette als C-String zurück (null-terminiert)
const char* str_cstr(const str* str) {
    if (str == NULL) {
        return "";
    }
    return str->is_heap ? str->data.heap_data : str->data.stack_data;
}

// Gibt die Länge der Zeichenkette zurück
size_t str_size(const str* str) {
    if (str == NULL) {
        return 0;
    }
    return str->size;
}

// Prüft, ob der String leer ist
bool str_empty(const str* str) {
    if (str == NULL) {
        return true;
    }
    return str->size == 0;
}

// Fügt eine C-Zeichenkette an den String an (verwendet SSO oder Heap)
void str_append_cstr(str* str, const char* cstr) {
    if (str == NULL || cstr == NULL) {
        return;
    }
    size_t cstr_len = strlen(cstr);
    size_t new_size = str->size + cstr_len;

    // Prüfen, ob der neue String noch auf dem Stack gespeichert werden kann
    bool use_heap = !str_can_use_sso(new_size);

    // Falls der aktuelle String auf dem Heap gespeichert ist oder der neue String zu groß ist,
    // müssen wir auf den Heap wechseln oder die Kapazität anpassen
    if (str->is_heap || use_heap) {
        if (!str->is_heap) {
            // Aktuellen Stack-String auf den Heap kopieren
            char stack_copy[str_SSO_MAX + 1];
            memcpy(stack_copy, str->data.stack_data, str->size + 1);
            str_allocate_heap(str, new_size);
            if (!str->is_heap) {
                return; // Fehler: Speicher konnte nicht alloziert werden
            }
            memcpy(str->data.heap_data, stack_copy, str->size + 1);
        } else {
            // Heap-Speicher vergrößern
            str->data.heap_data = (char*)realloc(str->data.heap_data, new_size + 1);
            if (str->data.heap_data == NULL) {
                return; // Fehler: Speicher konnte nicht alloziert werden
            }
        }
        // Zeichenkette anhängen
        strcat(str->data.heap_data, cstr);
        str->size = new_size;
    } else {
        // Stack-Speicher verwenden
        memcpy(str->data.stack_data + str->size, cstr, cstr_len);
        str->data.stack_data[new_size] = '\0';
        str->size = new_size;
    }
}

// Fügt einen einzelnen Buchstaben an den String an
void str_append_char(str* str, char c) {
    if (str == NULL) {
        return;
    }
    size_t new_size = str->size + 1;

    // Prüfen, ob der neue String noch auf dem Stack gespeichert werden kann
    bool use_heap = !str_can_use_sso(new_size);

    if (str->is_heap || use_heap) {
        if (!str->is_heap) {
            // Aktuellen Stack-String auf den Heap kopieren
            char stack_copy[str_SSO_MAX + 1];
            memcpy(stack_copy, str->data.stack_data, str->size + 1);
            str_allocate_heap(str, new_size);
            if (!str->is_heap) {
                return; // Fehler: Speicher konnte nicht alloziert werden
            }
            memcpy(str->data.heap_data, stack_copy, str->size + 1);
        } else {
            // Heap-Speicher vergrößern
            str->data.heap_data = (char*)realloc(str->data.heap_data, new_size + 1);
            if (str->data.heap_data == NULL) {
                return; // Fehler: Speicher konnte nicht alloziert werden
            }
        }
        // Buchstaben anhängen
        str->data.heap_data[str->size] = c;
        str->data.heap_data[new_size] = '\0';
        str->size = new_size;
    } else {
        // Stack-Speicher verwenden
        str->data.stack_data[str->size] = c;
        str->data.stack_data[new_size] = '\0';
        str->size = new_size;
    }
}

// Vergleicht zwei Strings (wie strcmp)
int str_compare(const str* str1, const str* str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    return strcmp(str_cstr(str1), str_cstr(str2));
}

// Gibt einen Substring zurück (neuer str, verwendet SSO oder Heap)
str str_substr(const str* str_, size_t start, size_t size) {
    if (str_ == NULL || start >= str_->size) {
        return str_create();
    }
    if (start + size > str_->size) {
        size = str_->size - start;
    }
    str _str_substr;
    str_copy_data(&_str_substr, str_cstr(str_) + start, size);
    return _str_substr;
}

// Löscht einen Teil des Strings
void str_erase(str* str, size_t start, size_t size) {
    if (str == NULL || start >= str->size) {
        return;
    }
    if (start + size > str->size) {
        size = str->size - start;
    }

    size_t new_size = str->size - size;

    if (str->is_heap) {
        // Heap-Speicher anpassen
        memmove(str->data.heap_data + start, str->data.heap_data + start + size, new_size - start + 1);
        str->data.heap_data[new_size] = '\0';
    } else {
        // Stack-Speicher anpassen
        memmove(str->data.stack_data + start, str->data.stack_data + start + size, new_size - start + 1);
        str->data.stack_data[new_size] = '\0';
    }
    str->size = new_size;
}
