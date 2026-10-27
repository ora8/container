#include "smart_ptr.h"

// Erstellt einen neuen Smart-Pointer (übernimmt Eigentum)
smart_ptr smart_ptr_create(void* data, void (*destructor)(void*)) {
    smart_ptr ptr;
    ptr.data = data;
    ptr.destructor = destructor;
    return ptr;
}

// Gibt den Smart-Pointer frei (ruft den Destruktor auf)
void smart_ptr_destroy(smart_ptr* ptr) {
    if (ptr != NULL) {
        if (ptr->destructor != NULL && ptr->data != NULL) {
            ptr->destructor(ptr->data);
        }
        ptr->data = NULL;
        ptr->destructor = NULL;
    }
}

// Gibt den Zeiger zurück (ohne Eigentum zu übertragen)
void* smart_ptr_get(const smart_ptr* ptr) {
    if (ptr != NULL) {
        return ptr->data;
    }
    return NULL;
}

// Setzt einen neuen Zeiger (alter Zeiger wird freigegeben)
void smart_ptr_reset(smart_ptr* ptr, void* new_data, void (*new_destructor)(void*)) {
    if (ptr != NULL) {
        smart_ptr_destroy(ptr); // Alten Zeiger freigeben
        ptr->data = new_data;
        ptr->destructor = new_destructor;
    }
}

// Prüft, ob der Smart-Pointer gültig ist
bool smart_ptr_is_valid(const smart_ptr* ptr) {
    return ptr != NULL && ptr->data != NULL;
}
