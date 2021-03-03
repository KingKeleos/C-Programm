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
    int dist; // Entfernung der St�dte
    } Distance;

typedef struct // Forgegebene Struktur
{int n; // Anzahl der St�dte
char **cities; // Pointer auf Feld mit St�dtenamen
Distance *distances; //Pointer auf Feld mit Entfernungen
} DistanceTable;
