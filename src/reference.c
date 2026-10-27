#include <stddef.h>

#include "reference.h"

// Erstellt eine neue, nicht initialisierte Referenz
reference reference_create() {
    return (reference){NULL, false};
}

// Initialisiert die Referenz
void reference_init(reference *ref, void *address) {
    ref->address = address;
    ref->valid = address;
}

// Ändert eine Referenz mit einer Adresse
void reference_change(reference *ref, void *address)
{
    reference_init(ref, address);
}

// Gibt die Adresse der Referenz zurück (NULL, wenn nicht initialisiert)
void *reference_get_address(const reference *ref) {
    if (!ref->valid) {
        return NULL;
    }
    return ref->address;
}

// Prüft, ob die Referenz initialisiert ist
bool reference_is_valid(const reference *ref) {
    return ref->valid;
}

// Erstellt eine neue, nicht initialisierte Referenz
int_reference int_reference_create() {
    return (int_reference){NULL, false};
}

// Initialisiert die Referenz
void int_reference_init(int_reference *ref, int *address) {
    ref->address = address;
    ref->valid = address;
}

// Ändert eine Referenz mit einer Adresse
void int_reference_change(int_reference *ref, int *address)
{
    int_reference_init(ref, address);
}

// Gibt die Adresse der Referenz zurück (NULL, wenn nicht initialisiert)
int *int_reference_get_address(const int_reference *ref) {
    if (!ref->valid) {
        return NULL;
    }
    return ref->address;
}

// Liefert den Wert der Reference und prüft auf die Gültikeit
bool int_reference_get_value(int_reference *ref, int *value)
{
    if (!ref->valid) {
        return false;
    }
    *value = *ref->address;
    return true;
}

// Prüft, ob die Referenz initialisiert ist
bool int_reference_is_valid(const int_reference *ref) {
    return ref->valid;
}

// Erstellt eine neue, nicht initialisierte Referenz
double_reference double_reference_create() {
    return (double_reference){NULL, false};
}

// Initialisiert die Referenz
void double_reference_init(double_reference *ref, double *address) {
    ref->address = address;
    ref->valid = address;
}

// Ändert eine Referenz mit einer Adresse
void double_reference_change(double_reference *ref, double *address)
{
    double_reference_init(ref, address);
}

// Gibt die Adresse der Referenz zurück (NULL, wenn nicht initialisiert)
double *double_reference_get_address(const double_reference *ref) {
    if (!ref->valid) {
        return NULL;
    }
    return ref->address;
}

// Liefert den Wert der Reference und prüft auf die Gültikeit
bool double_reference_get_value(double_reference *ref, double *value)
{
    if (!ref->valid) {
        return false;
    }
    *value = *ref->address;
    return true;
}

// Prüft, ob die Referenz initialisiert ist
bool double_reference_is_valid(const double_reference *ref) {
    return ref->valid;
}