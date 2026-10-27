#include "shared_ptr.h"
#include <stdio.h>
#include <stdlib.h>

// Destruktor für int
void destroy_int(void* data) {
    printf("Freigabe von int: %d\n", *(int*)data);
    free(data);
}

int main() {
    // Shared-Pointer für einen int erstellen
    int* x = malloc(sizeof(int));
    *x = 42;
    shared_ptr ptr1 = shared_ptr_create(x, destroy_int);

    if (shared_ptr_is_valid(&ptr1)) {
        printf("Wert von ptr1: %d\n", *(int*)shared_ptr_get(&ptr1));
    }

    // ptr2 ist eine Kopie von ptr1 (Referenzzähler wird erhöht)
    shared_ptr ptr2 = shared_ptr_copy(&ptr1);

    if (shared_ptr_is_valid(&ptr2)) {
        printf("Wert von ptr2: %d\n", *(int*)shared_ptr_get(&ptr2));
    }

    // ptr1 freigeben (Referenzzähler wird verringert, aber Objekt bleibt, weil ptr2 noch existiert)
    shared_ptr_release(&ptr1);

    // ptr2 freigeben (Referenzzähler wird verringert, Objekt wird freigegeben, weil kein anderer Zeiger mehr existiert)
    shared_ptr_release(&ptr2);

    return 0;
}
