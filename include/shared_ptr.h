#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <stddef.h>  // Für size_t
#include <stdbool.h> // Für bool

// Struktur für einen Shared-Pointer mit Referenzzählung
typedef struct {
    void* data;          // Zeiger auf die Daten
    size_t* ref_count;   // Zeiger auf den Referenzzähler
    void (*destructor)(void*); // Funktion zum Freigeben der Daten
} shared_ptr;

// Erstellt einen neuen Shared-Pointer
shared_ptr shared_ptr_create(void* data, void (*destructor)(void*));

// Kopiert einen Shared-Pointer (erhöht den Referenzzähler)
shared_ptr shared_ptr_copy(const shared_ptr* ptr);

// Gibt einen Shared-Pointer frei (verringert den Referenzzähler)
void shared_ptr_release(shared_ptr* ptr);

// Gibt den Zeiger zurück
void* shared_ptr_get(const shared_ptr* ptr);

// Prüft, ob der Shared-Pointer gültig ist
bool shared_ptr_is_valid(const shared_ptr* ptr);

#endif // SHARED_PTR_H
