#ifndef _HASH_H_
#define _HASH_H_

#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#define INITIAL_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.7  // Schwellenwert für dynamische Anpassung

typedef struct hash_node {
    char* key;
    void *value;
    struct hash_node* next;
} hash_node;

typedef struct {
    hash_node** buckets;
    size_t size;           // Aktuelle Größe der Hashtabelle
    size_t count;          // Anzahl der gespeicherten Elemente
    float load_factor_threshold;  // Schwellenwert für Load Factor
} hash_table;

hash_table *hash_table_create(int initial_size);
int hash_table_free(hash_table *table);
void hash_table_delete(hash_table* table, const char* key);
void *hash_table_get(hash_table *table, const char *key);
int hash_table_insert(hash_table *table, const char *key, void *value);

#define INT_NO_VALUE INT_MIN

// Struktur für einen Eintrag in der Hash-Tabelle
typedef struct int_hash_node {
    const char* key;  // Schlüssel (const char*)
    int value;        // Wert (int)
    struct int_hash_node* next; // Zeiger auf den nächsten Eintrag (für Chaining)
} int_hash_node;

// Struktur für die Hash-Tabelle
typedef struct {
    int_hash_node** buckets;  // Array von Buckets (Listen)
    size_t size;      // Größe der Hash-Tabelle
    size_t count;     // Anzahl der Elemente
} int_hash_table;


// Initialisiert eine neue Hash-Tabelle
int_hash_table* int_hash_table_create(size_t size);
// Gibt die Hash-Tabelle frei
void int_hash_table_free(int_hash_table* table);
// Fügt einen Schlüssel-Wert-Paar in die Hash-Tabelle ein
void int_hash_table_insert(int_hash_table* table, const char* key, int value);
// Gibt den Wert für einen Schlüssel zurück (oder -1, wenn der Schlüssel nicht existiert)
int int_hash_table_get(int_hash_table* table, const char* key);
// Prüft, ob ein Schlüssel in der Hash-Tabelle existiert
bool int_hash_table_contains(int_hash_table* table, const char* key);
// Entfernt einen Schlüssel-Wert-Paar aus der Hash-Tabelle
void int_hash_table_remove(int_hash_table* table, const char* key);
// Gibt die Anzahl der Elemente in der Hash-Tabelle zurück
size_t int_hash_table_count(int_hash_table* table);
// Gibt die Größe der Hash-Tabelle (Anzahl der Buckets) zurück
size_t int_hash_table_size(int_hash_table* table);

#define LONG_NO_VALUE LONG_MIN

// Struktur für einen Eintrag in der Hash-Tabelle
typedef struct long_hash_node {
    const char* key;  // Schlüssel (const char*)
    int value;        // Wert (int)
    struct long_hash_node* next; // Zeiger auf den nächsten Eintrag (für Chaining)
} long_hash_node;

// Struktur für die Hash-Tabelle
typedef struct {
    long_hash_node** buckets;  // Array von Buckets (Listen)
    size_t size;      // Größe der Hash-Tabelle
    size_t count;     // Anzahl der Elemente
} long_hash_table;

// Initialisiert eine neue Hash-Tabelle
long_hash_table* long_hash_table_create(size_t size);
// Gibt die Hash-Tabelle frei
void long_hash_table_free(long_hash_table* table);
// Fügt einen Schlüssel-Wert-Paar in die Hash-Tabelle ein
void long_hash_table_insert(long_hash_table* table, const char* key, long value);
// Gibt den Wert für einen Schlüssel zurück (oder -1, wenn der Schlüssel nicht existiert)
long long_hash_table_get(long_hash_table* table, const char* key);
// Prüft, ob ein Schlüssel in der Hash-Tabelle existiert
bool long_hash_table_contains(long_hash_table* table, const char* key);
// Entfernt einen Schlüssel-Wert-Paar aus der Hash-Tabelle
void long_hash_table_remove(long_hash_table* table, const char* key);
// Gibt die Anzahl der Elemente in der Hash-Tabelle zurück
size_t long_hash_table_count(long_hash_table* table);
// Gibt die Größe der Hash-Tabelle (Anzahl der Buckets) zurück
size_t long_hash_table_size(long_hash_table* table);

#define DOUBLE_NO_VALUE DBL_MIN

// Struktur für einen Eintrag in der Hash-Tabelle
typedef struct double_hash_node {
    const char* key;  // Schlüssel (const char*)
    double value;        // Wert (int)
    struct double_hash_node* next; // Zeiger auf den nächsten Eintrag (für Chaining)
} double_hash_node;

// Struktur für die Hash-Tabelle
typedef struct {
    double_hash_node** buckets;  // Array von Buckets (Listen)
    size_t size;      // Größe der Hash-Tabelle
    size_t count;     // Anzahl der Elemente
} double_hash_table;

// Initialisiert eine neue Hash-Tabelle
double_hash_table* double_hash_table_create(size_t size);
// Gibt die Hash-Tabelle frei
void double_hash_table_free(double_hash_table* table);
// Fügt einen Schlüssel-Wert-Paar in die Hash-Tabelle ein
void double_hash_table_insert(double_hash_table* table, const char* key, double value);
// Gibt den Wert für einen Schlüssel zurück (oder -1, wenn der Schlüssel nicht existiert)
double double_hash_table_get(double_hash_table* table, const char* key);
// Prüft, ob ein Schlüssel in der Hash-Tabelle existiert
bool double_hash_table_contains(double_hash_table* table, const char* key);
// Entfernt einen Schlüssel-Wert-Paar aus der Hash-Tabelle
void double_hash_table_remove(double_hash_table* table, const char* key);
// Gibt die Anzahl der Elemente in der Hash-Tabelle zurück
size_t double_hash_table_count(double_hash_table* table);
// Gibt die Größe der Hash-Tabelle (Anzahl der Buckets) zurück
size_t doulbe_hash_table_size(double_hash_table* table);

#endif // _HASH_H_
