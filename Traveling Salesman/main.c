#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int main()
{
    printf("Hello world!\n");
    return 0; //test
}

typedef struct // Forgegebene Struktur
{
    int from; // Index der Anfangsstadt
    int to;   // Index der Zielstadt
    int dist; // Entfernung der Städte
    } Distance;

typedef struct // Forgegebene Struktur
{int n; // Anzahl der Städte
char **cities; // Pointer auf Feld mit Städtenamen
Distance *distances; //Pointer auf Feld mit Entfernungen
} DistanceTable;
