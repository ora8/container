#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

hash_table *hash_table_create(int initial_size) {
  hash_table *table = (hash_table *)malloc(sizeof(hash_table));
  if (table == NULL) {
    return NULL;
  }

  table->buckets = (hash_node **)calloc(initial_size, sizeof(hash_node *));
  if (table->buckets == NULL) {
    free(table);
    return NULL;
  }

  table->size = initial_size;
  table->count = 0;
  table->load_factor_threshold = LOAD_FACTOR_THRESHOLD;
  return table;
}

int hash_table_free(hash_table *table) {
  if (table) {
    return -EINVAL;
  }
  for (size_t i = 0; i < table->size; i++) {
    hash_node *current = table->buckets[i];
    while (current != NULL) {
      hash_node *temp = current;
      current = current->next;
      free(temp->key);
      free(temp);
    }
  }
  free(table->buckets);
  free(table);
  return 0;
}

static size_t hash(const char *key, int size) {
  size_t hash_value = 0;
  while (*key) {
    hash_value = (hash_value * 31) + *key++;
  }
  return hash_value % size;
}

void hash_table_delete(hash_table* table, const char* key) {
    unsigned int index = hash(key, table->size);
    hash_node* current = table->buckets[index];
    hash_node* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                table->buckets[index] = current->next;
            } else {
                previous->next = current->next;
            }

            free(current->key);
            free(current);
            table->count--;  // Anzahl der Elemente verringern
            return;
        }
        previous = current;
        current = current->next;
    }
}

void *hash_table_get(hash_table *table, const char *key) {
  unsigned int index = hash(key, table->size);
  hash_node *current = table->buckets[index];

  while (current != NULL) {
    if (!strcmp(current->key, key)) {
      return current->value;
    }
    current = current->next;
  }

  return NULL; // Schlüssel nicht gefunden
}

static int hash_table_resize(hash_table *table) {
  int new_size = table->size * 2;
  hash_node **new_buckets = (hash_node **)calloc(new_size, sizeof(hash_node *));
  if (new_buckets == NULL) {
    return -ENOMEM;
  }

  // Alte Elemente in die neue Tabelle einfügen
  for (size_t i = 0; i < table->size; i++) {
    hash_node *current = table->buckets[i];
    while (current != NULL) {
      hash_node *next = current->next;
      unsigned int new_index = hash(current->key, new_size);

      // Neuer Knoten wird am Anfang der neuen Liste eingefügt
      current->next = new_buckets[new_index];
      new_buckets[new_index] = current;

      current = next;
    }
  }

  // Alte Buckets freigeben und neue zuweisen
  free(table->buckets);
  table->buckets = new_buckets;
  table->size = new_size;

  return 0;
}

int hash_table_insert(hash_table *table, const char *key, void *value) {
  if (!key) {
    return -EINVAL;
  }
  // Überprüfe, ob die Tabelle neu dimensioniert werden muss
  if ((float)table->count / table->size >= table->load_factor_threshold) {
    if (hash_table_resize(table) < 0) {
      return -ENOMEM;
    }
  }

  unsigned int index = hash(key, table->size);

  // Überprüfe, ob der Schlüssel bereits existiert
  hash_node *current = table->buckets[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      current->value = value; // Schlüssel existiert, Wert aktualisieren
      return 0;
    }
    current = current->next;
  }

  // Schlüssel existiert nicht, neuen Knoten erstellen
  hash_node *new_node = (hash_node *)malloc(sizeof(hash_node));
  if (!new_node) {
    return -1;
  }

  new_node->key = strdup(key);
  if (!new_node->key) {
    return -2;
  }
  new_node->value = value;
  new_node->next = table->buckets[index]; // Neuer Knoten wird Kopf der Liste
  table->buckets[index] = new_node;
  table->count++; // Anzahl der Elemente erhöhen

  return 0;
}

// Initialisiert eine neue Hash-Tabelle
int_hash_table* int_hash_table_create(size_t size) {
    int_hash_table* table = malloc(sizeof(int_hash_table));
    if (!table) {
        return NULL;
    }

    table->buckets = calloc(size, sizeof(int_hash_node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = size;
    table->count = 0;
    return table;
}

// Gibt die Hash-Tabelle frei
void int_hash_table_free(int_hash_table* table) {
    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->size; ++i) {
        int_hash_node* node = table->buckets[i];
        while (node) {
            int_hash_node* next = node->next;
            free(node);
            node = next;
        }
    }

    free(table->buckets);
    free(table);
}

// Fügt einen Schlüssel-Wert-Paar in die Hash-Tabelle ein
void int_hash_table_insert(int_hash_table* table, const char* key, int value) {
    if (!key) {
        return;
    }

    size_t index = hash(key, table->size);

    // Prüfe, ob der Schlüssel bereits existiert
    int_hash_node* node = table->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            // Schlüssel existiert: Wert aktualisieren
            node->value = value;
            return;
        }
        node = node->next;
    }

    // Schlüssel existiert nicht: neuen Eintrag erstellen
    int_hash_node* new_node = malloc(sizeof(int_hash_node));
    if (!new_node) {
        return;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;
}

// Gibt den Wert für einen Schlüssel zurück (oder -1, wenn der Schlüssel nicht existiert)
int int_hash_table_get(int_hash_table* table, const char* key) {
    if (!key) {
        return INT_NO_VALUE;
    }

    size_t index = hash(key, table->size);
    int_hash_node* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }

    return INT_NO_VALUE; // Schlüssel nicht gefunden
}

// Prüft, ob ein Schlüssel in der Hash-Tabelle existiert
bool int_hash_table_contains(int_hash_table* table, const char* key) {
    if (!key) {
        return false;
    }

    size_t index = hash(key, table->size);
    int_hash_node* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// Entfernt einen Schlüssel-Wert-Paar aus der Hash-Tabelle
void int_hash_table_remove(int_hash_table* table, const char* key) {
    if (!key) {
        return;
    }

    size_t index = hash(key, table->size);
    int_hash_node* node = table->buckets[index];
    int_hash_node* prev = NULL;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next;
            }
            free(node);
            table->count--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

// Gibt die Anzahl der Elemente in der Hash-Tabelle zurück
size_t int_hash_table_count(int_hash_table* table) {
    return table ? table->count : 0;
}

// Gibt die Größe der Hash-Tabelle (Anzahl der Buckets) zurück
size_t int_hash_table_size(int_hash_table* table) {
    return table ? table->size : 0;
}

// Initialisiert eine neue Hash-Tabelle
long_hash_table* long_hash_table_create(size_t size) {
    long_hash_table* table = malloc(sizeof(long_hash_table));
    if (!table) {
        return NULL;
    }

    table->buckets = calloc(size, sizeof(long_hash_node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = size;
    table->count = 0;
    return table;
}

// Gibt die Hash-Tabelle frei
void long_hash_table_free(long_hash_table* table) {
    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->size; ++i) {
        long_hash_node* node = table->buckets[i];
        while (node) {
            long_hash_node* next = node->next;
            free(node);
            node = next;
        }
    }

    free(table->buckets);
    free(table);
}

// Fügt einen Schlüssel-Wert-Paar in die Hash-Tabelle ein
void long_hash_table_insert(long_hash_table* table, const char* key, long value) {
    if (!key) {
        return;
    }

    size_t index = hash(key, table->size);

    // Prüfe, ob der Schlüssel bereits existiert
    long_hash_node* node = table->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            // Schlüssel existiert: Wert aktualisieren
            node->value = value;
            return;
        }
        node = node->next;
    }

    // Schlüssel existiert nicht: neuen Eintrag erstellen
    long_hash_node* new_node = malloc(sizeof(long_hash_node));
    if (!new_node) {
        return;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;
}

// Gibt den Wert für einen Schlüssel zurück (oder -1, wenn der Schlüssel nicht existiert)
long long_hash_table_get(long_hash_table* table, const char* key) {
    if (!key) {
        return LONG_NO_VALUE;
    }

    size_t index = hash(key, table->size);
    long_hash_node* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return LONG_NO_VALUE; // Schlüssel nicht gefunden
}

// Prüft, ob ein Schlüssel in der Hash-Tabelle existiert
bool long_hash_table_contains(long_hash_table* table, const char* key) {
    if (!key) {
        return false;
    }

    size_t index = hash(key, table->size);
    long_hash_node* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// Entfernt einen Schlüssel-Wert-Paar aus der Hash-Tabelle
void long_hash_table_remove(long_hash_table* table, const char* key) {
    if (!key) {
        return;
    }

    size_t index = hash(key, table->size);
    long_hash_node* node = table->buckets[index];
    long_hash_node* prev = NULL;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next;
            }
            free(node);
            table->count--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

// Gibt die Anzahl der Elemente in der Hash-Tabelle zurück
size_t long_hash_table_count(long_hash_table* table) {
    return table ? table->count : 0;
}

// Gibt die Größe der Hash-Tabelle (Anzahl der Buckets) zurück
size_t long_hash_table_size(long_hash_table* table) {
    return table ? table->size : 0;
}

// Initialisiert eine neue Hash-Tabelle
double_hash_table* double_hash_table_create(size_t size) {
    double_hash_table* table = malloc(sizeof(double_hash_table));
    if (!table) {
        return NULL;
    }

    table->buckets = calloc(size, sizeof(double_hash_node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = size;
    table->count = 0;
    return table;
}

// Gibt die Hash-Tabelle frei
void double_hash_table_free(double_hash_table* table) {
    if (!table) {
        return;
    }

    for (size_t i = 0; i < table->size; ++i) {
        double_hash_node* node = table->buckets[i];
        while (node) {
            double_hash_node* next = node->next;
            free(node);
            node = next;
        }
    }

    free(table->buckets);
    free(table);
}

// Fügt einen Schlüssel-Wert-Paar in die Hash-Tabelle ein
void double_hash_table_insert(double_hash_table* table, const char* key, double value) {
    if (!key) {
        return;
    }

    size_t index = hash(key, table->size);

    // Prüfe, ob der Schlüssel bereits existiert
    double_hash_node* node = table->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            // Schlüssel existiert: Wert aktualisieren
            node->value = value;
            return;
        }
        node = node->next;
    }

    // Schlüssel existiert nicht: neuen Eintrag erstellen
    double_hash_node* new_node = malloc(sizeof(double_hash_node));
    if (!new_node) {
        return;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;
}

// Gibt den Wert für einen Schlüssel zurück (oder -1, wenn der Schlüssel nicht existiert)
double double_hash_table_get(double_hash_table* table, const char* key) {
    if (!key) {
        return DOUBLE_NO_VALUE;
    }

    size_t index = hash(key, table->size);
    double_hash_node* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return DOUBLE_NO_VALUE; // Schlüssel nicht gefunden
}

// Prüft, ob ein Schlüssel in der Hash-Tabelle existiert
bool double_hash_table_contains(double_hash_table* table, const char* key) {
    if (!key) {
        return false;
    }

    size_t index = hash(key, table->size);
    double_hash_node* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            return true;
        }
        node = node->next;
    }

    return false;
}

// Entfernt einen Schlüssel-Wert-Paar aus der Hash-Tabelle
void double_hash_table_remove(double_hash_table* table, const char* key) {
    if (!key) {
        return;
    }

    size_t index = hash(key, table->size);
    double_hash_node* node = table->buckets[index];
    double_hash_node* prev = NULL;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next;
            }
            free(node);
            table->count--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

// Gibt die Anzahl der Elemente in der Hash-Tabelle zurück
size_t double_hash_table_count(double_hash_table* table) {
    return table ? table->count : 0;
}

// Gibt die Größe der Hash-Tabelle (Anzahl der Buckets) zurück
size_t double_hash_table_size(double_hash_table* table) {
    return table ? table->size : 0;
}