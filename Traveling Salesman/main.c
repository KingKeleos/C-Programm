#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wchar.h>


int main()
{
    printf("Travelingsalesman-Problem\n");
    return 0;
}

void readTable()
{
int c;
FILE *file;
file=fopen("Staedte.txt","r");
if(file){
    while ((c=getc(file))!=EOF)
        putchar(c);
    fclose(file);
}
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
