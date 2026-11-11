#include "string_view.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funktion zum Generieren eines zufälligen Strings
void generate_random_string(char buffer[], size_t length) {
  // Prüfen, ob die Länge gültig ist
  if (length == 0) {
    return;
  }

  // Mögliche Zeichen für den String (z. B. Großbuchstaben, Kleinbuchstaben,
  // Ziffern, Sonderzeichen)
  const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "abcdefghijklmnopqrstuvwxyz"
                         "0123456789"
                         "!@#$%^&*()_+-=[]{}|;:,.<>?\0";

  // Zufällige Zeichen auswählen und in den String schreiben
  for (size_t i = 0; i < length; i++) {
    size_t index = (size_t)(rand() % (sizeof(charset) - 1));
    buffer[i] = charset[index];
    if (!buffer[i]) {
      break;
    }
  }

  // Null-Byte am Ende hinzufügen
  buffer[length] = '\0';
}

size_t compare_test() {

  size_t ret = 0;
  for (size_t i = 0; i < 1000000; i++) {
    // Beispiel: Zufälligen String mit 16 Zeichen generieren
    const size_t length = 16;
    char buffer1[length + 1];
    char buffer2[length + 2];

    srand((unsigned int)time(NULL));
    generate_random_string(buffer1, length);
    generate_random_string(buffer2, length);

    string_view sv1 = string_view_from_cstr("buffer1");
    string_view sv2 = string_view_from_cstr(buffer2);

    if (!string_view_compare(&sv1, &sv2)) {
      ret++;
    }
  }

  return ret;
}

int main() {
  // string_view aus einer C-Zeichenkette erstellen
  string_view sv1 = string_view_from_cstr("Hallo, Welt!");
  printf("string_view 1: '%.*s' (Länge: %zu)\n", (int)sv1.size, sv1.data,
         sv1.size);

  // string_view aus einem Zeichen-Array und Länge erstellen
  const char *str = "C ist toll!";
  string_view sv2 = string_view_from_chars(str, 9);
  printf("string_view 2: '%.*s' (Länge: %zu)\n", (int)sv2.size, sv2.data,
         sv2.size);

  // Länge abfragen
  printf("Länge von sv1: %zu\n", string_view_size(&sv1));

  // Substring erstellen
  string_view substr = string_view_substr(&sv1, 7, 5);
  printf("Substring: '%.*s' (Länge: %zu)\n", (int)substr.size, substr.data,
         substr.size);

  // Vergleich
  string_view sv3 = string_view_from_cstr("Hallo");
  string_view sv4 = string_view_from_cstr("Hello");
  int cmp = string_view_compare(&sv3, &sv4);
  printf("Vergleich von sv3 und sv4: %d (0 = gleich)\n", cmp);

  // Überprüfen, ob string_view leer ist
  string_view empty_sv = string_view_from_cstr("");
  printf("Ist sv leer? %s\n", string_view_is_empty(&empty_sv) ? "Ja" : "Nein");

  clock_t start_time = clock();

  size_t num_equal = compare_test();

  // Zeitmessung beenden
  clock_t end_time = clock();

  // Berechne die benötigte Zeit in Sekunden
  double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("Benötigte Zeit: %.6f Sekunden\n", time_spent);

  printf("Anzahl gleicher string_views: %zu\n", num_equal);

  return 0;
}
