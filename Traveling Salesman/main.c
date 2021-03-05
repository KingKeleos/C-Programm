#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

double kuerzesteRoute ()
{
    char start [57]; //Der Längste Ortsname auf der Welt hat 58 Zeichen
    printf("Nennen Sie mir die Startstadt:\n");
    scanf ("%s", &start[0]);

}

int main()
{
    printf("Hello world!\n");
    kuerzesteRoute();
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
