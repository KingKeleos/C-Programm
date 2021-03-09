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
    char **cities; // Pointer auf Staedtenamen
    Distance *distances; //Pointer auf Entfernungen vom Typ Distance
    } DistanceTable;



void readCities()
{
    char c;
    char cities[15][128];
    char string[128]=""; //String erstellen aus chars
    char filename[128];
    printf("Datei laden:\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename);
    int i=0; // Zaehlervriable
    FILE *fptr;
    fptr=fopen(filename,"r"); //Hardcoded Datei Städte wird im darüber liegenden Ornder gelesen
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden"); //Fehlerwurf, bei nicht existenter Datei
        exit(1);
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    while ( (c=fgetc(fptr)) != '\n') //Jeden Character Lesen, bis zum Ende der Datei
    {
        if (c==' ') // Lehrzeichen filtern
        {
            strcpy(cities[i], string);
            strcpy(string,"");
            i=i+1; // Staedte zaehlen
        }
        else
        {
            strncat(string, &c,1);
        }
    }
    printf("Staedte:\n");
    for (int j=0; j<i;j++)
    {
        printf("%d. %s\n",j+1,cities[j]);
    }
    printf("Es wurden %d Staedte gefunden", i); // Bestaetigung, dass alle Staedte gefunden wurden.
    fclose(fptr);
}

int main(void)
{
    Distance distances;
    DistanceTable Table;
    char cities[15][128];
    char* citptr;
    printf("\"Traveling Salesman\"-Problem\n");
    printf("Tabelle aus Datei lesen? y/n:\n");
    char ans;
    do
    {
        scanf("%c", ans);
        if (ans=='y')
        {
            readCities();
        }
        if (ans=='n')
        {
            printf("Hier endet das Programm.");
            return 0;
        }
    } while  (ans != 'y' || ans !='n');
    return 0;
}
