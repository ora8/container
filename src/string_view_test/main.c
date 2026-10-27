#include "string_view.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // string_view aus einer C-Zeichenkette erstellen
    string_view sv1 = string_view_from_cstr("Hallo, Welt!");
    printf("string_view 1: '%.*s' (Länge: %zu)\n", (int)sv1.length, sv1.data, sv1.length);

    // string_view aus einem Zeichen-Array und Länge erstellen
    const char* str = "C ist toll!";
    string_view sv2 = string_view_from_chars(str, 9);
    printf("string_view 2: '%.*s' (Länge: %zu)\n", (int)sv2.length, sv2.data, sv2.length);

    // Länge abfragen
    printf("Länge von sv1: %zu\n", string_view_length(&sv1));

    // Substring erstellen
    string_view substr = string_view_substr(&sv1, 7, 5);
    printf("Substring: '%.*s' (Länge: %zu)\n", (int)substr.length, substr.data, substr.length);

    // Vergleich
    string_view sv3 = string_view_from_cstr("Hallo");
    string_view sv4 = string_view_from_cstr("Hallo");
    int cmp = string_view_compare(&sv3, &sv4);
    printf("Vergleich von sv3 und sv4: %d (0 = gleich)\n", cmp);

    // Umwandlung in C-String
    const char* cstr = string_view_to_cstr(&sv1);
    if (cstr != NULL) {
        printf("C-String: '%s'\n", cstr);
        free((void*)cstr);  // Speicher freigeben
    }

    // Überprüfen, ob string_view leer ist
    string_view empty_sv = string_view_from_cstr("");
    printf("Ist sv leer? %s\n", string_view_is_empty(&empty_sv) ? "Ja" : "Nein");

    return 0;
}
