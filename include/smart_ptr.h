#ifndef SMART_PTR_H
#define SMART_PTR_H

#include <stddef.h>  // Für size_t
#include <stdbool.h> // Für bool

// Struktur für einen einfachen Smart-Pointer (ähnlich unique_ptr)
typedef struct {
    void* data;      // Zeiger auf die Daten
    void (*destructor)(void*); // Funktion zum Freigeben der Daten
} smart_ptr;

// Erstellt einen neuen Smart-Pointer (übernimmt Eigentum)
smart_ptr smart_ptr_create(void* data, void (*destructor)(void*));

// Gibt den Smart-Pointer frei (ruft den Destruktor auf)
void smart_ptr_destroy(smart_ptr* ptr);

// Gibt den Zeiger zurück (ohne Eigentum zu übertragen)
void* smart_ptr_get(const smart_ptr* ptr);

// Setzt einen neuen Zeiger (alter Zeiger wird freigegeben)
void smart_ptr_reset(smart_ptr* ptr, void* new_data, void (*new_destructor)(void*));

// Prüft, ob der Smart-Pointer gültig ist
bool smart_ptr_is_valid(const smart_ptr* ptr);

#endif // SMART_PTR_H
