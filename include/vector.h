#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>  // Für size_t

typedef struct {
    void** data;     // Array von void*-Zeigern
    size_t size;     // Anzahl der Elemente im vector
    size_t capacity; // Maximale Kapazität des vectors
} vector;

// Erstellt einen neuen vector mit einer Anfangskapazität
vector* vector_create(size_t initial_capacity);

// Zerstört den vector und gibt den belegten Speicher frei
int vector_free(vector* vec);

bool vector_is_empty(const vector *vec);

// Fügt ein Element am Ende des vectors hinzu
void vector_push_back(vector* vec, void* element);

// Entfernt das letzte Element des vectors
void vector_pop_back(vector* vec);

// Gibt die erste Position des angegebenen Element im Vektor zurück oder -1 wenn nicht gefunden.
ssize_t vector_find(const vector *vec, void *element);

// Gibt das Element an der angegebenen Position zurück
void* vector_get(const vector* vec, size_t index);

// Gibt die Anzahl der Elemente im vector zurück
size_t vector_size(const vector* vec);

// Gibt die Kapazität des vectors zurück
size_t vector_capacity(const vector* vec);

// Löscht alle Elemente aus dem vector
void vector_clear(vector* vec);

#endif