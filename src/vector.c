#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

// Standard-Startkapazität, falls keine angegeben wird
#define DEFAULT_CAPACITY 16

// Erstellt einen neuen vector mit einer Anfangskapazität
vector *vector_create(size_t initial_capacity) {
  vector *vec = (vector *)malloc(sizeof(vector));
  if (vec == NULL) {
    return NULL;
  }

  vec->capacity = (initial_capacity > 0) ? initial_capacity : DEFAULT_CAPACITY;
  vec->size = 0;
  vec->data = (void **)malloc(vec->capacity * sizeof(void *));
  if (vec->data == NULL) {
    free(vec);
    return NULL;
  }

  return vec;
}

// Zerstört den vector und gibt den belegten Speicher frei
int vector_free(vector *vec) {
  if (vec == NULL) {
    return -EINVAL;
  }

  free(vec->data);
  free(vec);

  return 0;
}

// Verdoppelt die Kapazität des vectors, falls nötig
static void vector_resize(vector *vec) {
  size_t new_capacity = vec->capacity * 2;
  void **new_data = (void **)realloc(vec->data, new_capacity * sizeof(void *));
  if (new_data == NULL) {
    return; // Fehlerbehandlung: Speicher konnte nicht alloziert werden
  }

  vec->data = new_data;
  vec->capacity = new_capacity;
}

// Fügt ein Element am Ende des vectors hinzu
void vector_push_back(vector *vec, void *element) {
  if (vec->size >= vec->capacity) {
    vector_resize(vec);
  }

  vec->data[vec->size++] = element;
}

// Entfernt das letzte Element des vectors
void vector_pop_back(vector *vec) {
  if (vec->size == 0) {
    return; // vector ist leer
  }

  vec->size--;
}

// Prüft ob der Vektor leer ist
bool vector_is_empty(const vector *vec)
{
    return vec->size == 0L;
}

// Gibt die Position des angebenenen Elements zurück oder -1 wenn nicht gefunden
// (Lineare Suche)
ssize_t vector_find(const vector *vec, void *element) { 
    for (size_t index = 0; index < vector_size(vec); index++) {
        if (vec->data[index] == element) {
            return index;
        }
    }
    return -1; 
}

// Gibt das Element an der angegebenen Position zurück
void *vector_get(const vector *vec, size_t index) {
  if (index >= vec->size) {
    return NULL; // Index ungültig
  }

  return vec->data[index];
}

// Gibt die Anzahl der Elemente im vector zurück
size_t vector_size(const vector *vec) { return vec->size; }

// Gibt die Kapazität des vectors zurück
size_t vector_capacity(const vector *vec) { return vec->capacity; }

// Löscht alle Elemente aus dem vector
void vector_clear(vector *vec) { vec->size = 0; }
