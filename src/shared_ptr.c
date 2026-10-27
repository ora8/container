#include <stdlib.h>
#include <stdio.h>

#include "shared_ptr.h"

// Erstellt einen neuen Shared-Pointer
shared_ptr shared_ptr_create(void* data, void (*destructor)(void*)) {
    if (data == NULL) {
        return (shared_ptr){NULL, NULL, NULL};
    }

    shared_ptr ptr;
    ptr.data = data;
    ptr.ref_count = malloc(sizeof(size_t));
    if (ptr.ref_count == NULL) {
        free(data);
        return (shared_ptr){NULL, NULL, NULL};
    }
    *ptr.ref_count = 1;
    ptr.destructor = destructor;
    return ptr;
}

// Kopiert einen Shared-Pointer (erhöht den Referenzzähler)
shared_ptr shared_ptr_copy(const shared_ptr* ptr) {
    if (ptr == NULL || ptr->data == NULL || ptr->ref_count == NULL) {
        return (shared_ptr){NULL, NULL, NULL};
    }

    shared_ptr copy = *ptr;
    (*copy.ref_count)++;
    return copy;
}

// Gibt einen Shared-Pointer frei (verringert den Referenzzähler)
void shared_ptr_release(shared_ptr* ptr) {
    if (ptr == NULL || ptr->ref_count == NULL) {
        return;
    }

    (*ptr->ref_count)--;

    if (*ptr->ref_count == 0) {
        if (ptr->destructor != NULL) {
            ptr->destructor(ptr->data);
        }
        free(ptr->ref_count);
    }

    ptr->data = NULL;
    ptr->ref_count = NULL;
    ptr->destructor = NULL;
}

// Gibt den Zeiger zurück
void* shared_ptr_get(const shared_ptr* ptr) {
    if (ptr != NULL) {
        return ptr->data;
    }
    return NULL;
}

// Prüft, ob der Shared-Pointer gültig ist
bool shared_ptr_is_valid(const shared_ptr* ptr) {
    return ptr != NULL && ptr->data != NULL && ptr->ref_count != NULL;
}
