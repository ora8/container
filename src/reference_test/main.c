#include <stdio.h>
#include "reference.h"

int main() {
    // Neue Referenz erstellen
    reference ref = reference_create();
    printf("Initialisiert: %s\n", reference_is_valid(&ref) ? "Ja" : "Nein");
    printf("Adresse: %p\n", reference_get_address(&ref));

    // Referenz initialisieren
    int x = 42;
    reference_init(&ref, &x);
    printf("Nach Initialisierung:\n");
    printf("Gültig: %s\n", reference_is_valid(&ref) ? "Ja" : "Nein");
    printf("Adresse: %p\n", reference_get_address(&ref));
    printf("Wert an der Adresse: %d\n", *(int *)reference_get_address(&ref));

    // Adresse ändern
    double y = 3.14;
    reference_change(&ref, &y);
    printf("Nach Adressänderung:\n");
    printf("Gültig: %s\n", reference_is_valid(&ref) ? "Ja" : "Nein");
    printf("Adresse: %p\n", reference_get_address(&ref));
    printf("Wert an der Adresse: %f\n", *(double *)reference_get_address(&ref));

    // Referenz auf NULL setzen
    reference_change(&ref, NULL);
    printf("Nach Setzen auf NULL:\n");
    printf("Gültig: %s\n", reference_is_valid(&ref) ? "Ja" : "Nein");
    printf("Adresse: %p\n", reference_get_address(&ref));

    return 0;
}
