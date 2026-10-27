#ifndef REFERENCE_H
#define REFERENCE_H

#include <stdbool.h>

typedef struct {
    void *address;
    bool valid;
} reference;

typedef struct {
    int *address;
    bool valid;
} int_reference;

typedef struct {
    char *address;
    bool valid;
} char_reference;

typedef struct {
    short *address;
    bool valid;
} short_reference;

typedef struct {
    long *address;
    bool valid;
} long_reference;

typedef struct {
    double *address;
    bool valid;
} double_reference;

// Erstellt eine neue, nicht initialisierte Referenz
reference reference_create();

// Initialisiert die Referenz
void reference_init(reference *ref, void *address);

// Ändert eine Referenz mit einer Adresse
void reference_change(reference *ref, void *address);

// Gibt die Adresse der Referenz zurück (NULL, wenn nicht initialisiert)
void *reference_get_address(const reference *ref);

// Prüft, ob die Referenz initialisiert ist
bool reference_is_valid(const reference *ref);

// Erstellt eine neue, nicht initialisierte Referenz
int_reference int_reference_create();

// Initialisiert die Referenz
void int_reference_init(int_reference *ref, int *address);

// Ändert eine Referenz mit einer Adresse
void int_reference_change(int_reference *ref, int *address);

// Gibt die Adresse der Referenz zurück (NULL, wenn nicht initialisiert)
int *int_reference_get_address(const int_reference *ref);

// Liefert den Wert der Reference und prüft auf die Gültikeit
bool int_reference_get_value(int_reference *ref, int *value);

// Prüft, ob die Referenz initialisiert ist
bool int_reference_is_valid(const int_reference *ref);

// Erstellt eine neue, nicht initialisierte Referenz
double_reference double_reference_create();

// Initialisiert die Referenz
void double_reference_init(double_reference *ref, double *address);

// Ändert eine Referenz mit einer Adresse
void double_reference_change(double_reference *ref, double *address);

// Gibt die Adresse der Referenz zurück (NULL, wenn nicht initialisiert)
double *double_reference_get_address(const double_reference *ref);

// Liefert den Wert der Reference und prüft auf die Gültikeit
bool double_reference_get_value(double_reference *ref, double *value);

// Prüft, ob die Referenz initialisiert ist
bool double_reference_is_valid(const double_reference *ref);

#endif // REFERENCE_H
