#include "smart_ptr.h"
#include <stdio.h>
#include <stdlib.h>

// Destruktor für int
void destroy_int(void* data) {
    printf("Freigabe von int: %d\n", *(int*)data);
    free(data);
}

// Destruktor für char[]
void destroy_string(void* data) {
    printf("Freigabe von String: %s\n", (char*)data);
    free(data);
}

int main() {
    // Smart-Pointer für einen int erstellen
    int* x = malloc(sizeof(int));
    *x = 42;
    smart_ptr ptr1 = smart_ptr_create(x, destroy_int);

    if (smart_ptr_is_valid(&ptr1)) {
        printf("Wert von ptr1: %d\n", *(int*)smart_ptr_get(&ptr1));
    }

    // Smart-Pointer für einen String erstellen
    char* str = malloc(20);
    snprintf(str, 20, "Hallo, Welt!");
    smart_ptr ptr2 = smart_ptr_create(str, destroy_string);

    if (smart_ptr_is_valid(&ptr2)) {
        printf("Wert von ptr2: %s\n", (char*)smart_ptr_get(&ptr2));
    }

    // ptr1 freigeben
    smart_ptr_destroy(&ptr1);

    // ptr2 zurücksetzen (neuer String)
    char* new_str = malloc(30);
    snprintf(new_str, 30, "Neuer String");
    smart_ptr_reset(&ptr2, new_str, destroy_string);

    if (smart_ptr_is_valid(&ptr2)) {
        printf("Neuer Wert von ptr2: %s\n", (char*)smart_ptr_get(&ptr2));
    }

    // ptr2 freigeben
    smart_ptr_destroy(&ptr2);

    return 0;
}
