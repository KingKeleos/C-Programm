#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wchar.h>

typedef struct // Forgegebene Struktur
{
    int from; // Index der Anfangsstadt
    int to;   // Index der Zielstadt
    int dist; // Entfernung der Städte
    } Distance;

typedef struct // Forgegebene Struktur
{
    int n; // Anzahl der Städte
    char **cities; // Pointer auf Feld mit Städtenamen
    Distance *distances; //Pointer auf Feld mit Entfernungen
    } DistanceTable;

int main(void)
{
    printf("\"Traveling Salesman\"-Problem\n");
    DistanceTable Table;
    char c;
    int i=0; // Zaehlervriable
    int j=0;
    FILE *fptr;
    fptr=fopen("../Staedte.txt","r"); //Hardcoded Datei Städte wird im darüber liegenden Ornder gelesen
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden"); //Fehlerwurf, bei nicht existenter Datei
        exit(1);
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    while ( (c=fgetc(fptr)) != EOF) //Jeden Character Lesen, bis zum Ende der Datei
    {
        while (c!='\n')
        {
            if (c==' ') // Lehrzeichen filtern
            {
                i=i+1; // Staedte zaehlen
            }
            else
            {
                strcat(Table.cities, c);
                printf("%s\n", Table.cities);
            }
        }
        i=0;
        j=j+1;
    }
    printf("Es wurden %d Staedte gefunden", Table.n); // Bestaetigung, dass alle Staedte gefunden wurden.
    Table.cities="";
    int fclose(fptr);
    return 0;
}

int readTable(char* filename)
{
}
