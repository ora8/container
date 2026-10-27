#include "str.h"
#include <stdio.h>

void print_string_info(const str* str, const char* name) {
    printf("%s: '%s' (Länge: %zu, SSO: %s, Heap: %s)\n",
           name,
           str_cstr(str),
           str_size(str),
           str_size(str) < 24 ? "Ja" : "Nein",
           str->is_heap ? "Ja" : "Nein");
}

int main() {
    // Leeren String erstellen (verwendet SSO)
    str str1 = str_create();
    print_string_info(&str1, "str1 (leer)");

    // String aus C-Zeichenkette erstellen (verwendet SSO, da Länge <= 15)
    str str2 = str_create_from_cstr("Hallo, Welt!");
    print_string_info(&str2, "str2 ('Hallo, Welt!')");

    // String aus einer längeren C-Zeichenkette erstellen (verwendet Heap)
    str str3 = str_create_from_cstr("Dies ist ein sehr langer String, der die SSO-Grenze überschreitet!");
    print_string_info(&str3, "str3 (langer String)");

    // String mit Kapazität erstellen (erzwingt Heap-Nutzung)
    str str4 = str_create_with_capacity(100);
    print_string_info(&str4, "str4 (mit Kapazität)");

    // Buchstaben anhängen (str1 bleibt auf dem Stack)
    str_append_char(&str1, 'A');
    str_append_char(&str1, 'B');
    str_append_char(&str1, 'C');
    print_string_info(&str1, "str1 nach Anhängen von 'ABC'");

    // C-Zeichenkette anhängen (str1 bleibt auf dem Stack)
    str_append_cstr(&str1, "DEF");
    print_string_info(&str1, "str1 nach Anhängen von 'DEF'");

    // Substring erstellen (verwendet SSO, da Länge <= 15)
    str substr = str_substr(&str2, 7, 5);
    print_string_info(&substr, "substr ('Welt!')");

    // String vergleichen
    printf("Vergleich str2 und str2: %d (0 = gleich)\n", str_compare(&str2, &str2));

    // Teil des Strings löschen (str2 bleibt auf dem Stack)
    str_erase(&str2, 5, 6);
    print_string_info(&str2, "str2 nach Löschen ('Hallo!')");

    // Speicher freigeben
    str_destroy(&str1);
    str_destroy(&str2);
    str_destroy(&str3);
    str_destroy(&str4);
    str_destroy(&substr);

    printf("Sizeof str %lu\n", sizeof(str));

    return 0;
}
