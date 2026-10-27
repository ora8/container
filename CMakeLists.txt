cmake_minimum_required(VERSION 3.10)
project(Container C)

# Setze die C-Standardversion (z. B. C11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Optionale Warnungen aktivieren (empfohlen für Entwicklung)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Optionale Optimierungen für Release-Builds
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)  # Standardmäßig Release
endif()

# Setze das include-Verzeichnis für Header-Dateien
include_directories(${CMAKE_SOURCE_DIR}/include)

# Gemeinsame Quelldateien
set(CONTAINER_SOURCES
    src/hash.c
    src/vector.c
    src/string_view.c
    src/reference.c
    src/str.c
)

# Quelldateien für die beiden Programme definieren
set(HASH_TEST_SOURCES
    src/hash_test/main.c
)

set(VECTOR_TEST_SOURCES
    src/vector_test/main.c
)

set(STRING_VIEW_TEST_SOURCES
    src/string_view_test/main.c
)

set(REFERNCE_TEST_SOURCES
    src/reference_test/main.c
)

set(STRING_TEST_SOURCES
    src/str_test/main.c
)

# Erstes ausführbares Programm erstellen
add_executable(hash_test ${CONTAINER_SOURCES} ${HASH_TEST_SOURCES})

# Zweites ausführbares Programm erstellen
add_executable(vector_test ${CONTAINER_SOURCES} ${VECTOR_TEST_SOURCES})

# Drittes ausführbares Programm erstellen
add_executable(string_view_test ${CONTAINER_SOURCES} ${STRING_VIEW_TEST_SOURCES})

# Viertes ausführbares Programm erstellen
add_executable(reference_test ${CONTAINER_SOURCES} ${REFERNCE_TEST_SOURCES})

# Fünftes ausführbares Programm erstellen
add_executable(string_test ${CONTAINER_SOURCES} ${STRING_TEST_SOURCES})

# Debug- und Release-Konfigurationen
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    add_compile_options(-g)  # Debug-Symbole
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    add_compile_options(-O3)  # Optimierungen
endif()
