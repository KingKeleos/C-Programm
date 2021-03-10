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
    char **cities; // Pointer Feld der Staedtenamen
    Distance *distances; //Pointer auf Feld vom Typ Distance
    } DistanceTable;

void readCities()
{
    char c;
    char cities[15][128];
    char string[128]=""; //String erstellen aus chars
    char filename[128];
    Distance distances[15];
    DistanceTable tables[15];
    for (int j=0; j<15;j++)
    {
        tables[j].cities = &cities[j];
    }
    printf("%s", tables[0].cities);
    printf("Datei laden:\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename); //Nutzer kann seine Datei aussuchen
    int i=0; // Zaehlervriable
    FILE *fptr=fopen(filename,"r"); //Ausgewählte datei wird nur gelese;
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden"); //Fehlerwurf, bei nicht existenter Zeile
        exit(1);
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    do
        {
        if (c==' ') // Lehrzeichen filtern
        {
            strcpy(cities[i], string);
            tables[i].n=i;
            strcpy(string,"");
            i=i+1; // Staedte zaehlen
        }
        else
        {
            strncat(string, &c,1);
        }
    }
    while ( (c=fgetc(fptr)) != '\n'); //Jeden Character Lesen, bis zum Ende der ersten Reihe

    strcpy(cities[i], string);
    tables[i].n=i;
    strcpy(string,"");
    i=i+1;
    //for (int k; k<i-1; k++)
    //{
    //       while ( (c=fgetc(fptr)) != EOF) //Jeden Character Lesen, bis zum Ende der Datei
    //        {
    //            if (c==' ') // Lehrzeichen filtern
    //            {
    //                strcpy(cities[i], string);
    //                strcpy(string,"");
    //                k=k+1; // Staedte zaehlen
    //            }
    //            else
    //            {
    //                strncat(string, &c,1);
    //            }
    //}
    //}
    printf("Staedte:\n");
    for (int j=0; j<i;j++)
    {
        printf("%d. %s\n",j+1,tables[j].cities);
    }
    printf("Es wurden %d Staedte gefunden", i); // Bestaetigung, dass alle Staedte gefunden wurden.
    fclose(fptr);
}


int main(void)
{
    Distance distances;
    DistanceTable Table;
    printf("\"Traveling Salesman\"-Problem\n");
    readCities();
    return 0;
}
