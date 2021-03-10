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

void readCities(Distance distances[], DistanceTable tables[])
{
    char c;
    char string[128]=""; //String erstellen aus chars
    char filename[128];
    printf("Datei laden:\n\n");
    retry: printf("Wie heisst die Datei: ");
    scanf("%s", filename); //Nutzer kann seine Datei aussuchen
    int i=0; // Zaehlervriable
    FILE *fptr=fopen(filename,"r"); //Ausgewählte datei wird nur gelese;
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden!\n"); //Fehlerwurf, bei nicht existenter Zeile
        goto retry;
        exit(1);
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    while ( (c=fgetc(fptr)) != '\n') //Jeden Character Lesen, bis zum Ende der ersten Reihe, um nur Namen der Städte zu nehmen
        {
        if (c==' ') // Lehrzeichen filtern
        {
            strcpy(tables[i].cities, string);
            tables[i].n=i;
            strcpy(string,"");
            i=i+1; // Staedte zaehlen
        }
        else
        {
            strncat(string, &c,1);
        }
    }
    strcpy(tables[i].cities, string);
    tables[i].n=i;
    strcpy(string,"");
    i=i+1;
    printf("Staedte:\n");
    for (int j=0; j<i;j++)
    {
        printf("%d. %s\n",tables[j].n+1 ,tables[j].cities);
    }
    //int k=0;
    int n=0;
    int j=0;
    while ( c!= EOF)
    {
        int k=0;
           while ( (c=fgetc(fptr)) != '\n'&& c !=EOF) //Jeden Character Lesen, bis zum Ende der Datei, um nur die Zahlen zu lesen
            {
                if (c==' ') // Lehrzeichen filtern
                {
                    distances[n].from=j;
                    distances[n].to=k;
                    distances[n].dist = atoi(string); //String in Int convertieren
                    strcpy(string,""); //String wieder leeren
                    printf("%d ",distances[n].dist);
                    k++; // Staedte zaehlen
                    n++;
                }
                else
                {
                    strncat(string, &c,1);
                }
           }
            distances[n].from=j;
            distances[n].to=k;
            distances[n].dist = atoi(string);
            strcpy(string,""); //String wieder leeren
            printf("%d \n",distances[n].dist);
            n++;
            j++;
    }
    printf("Es wurden %d Staedte gefunden\n", i); // Bestaetigung, dass alle Staedte gefunden wurden.
    fclose(fptr);
}


int main(void)
{
    char cities[15][128];
    Distance dist[256];
    DistanceTable tables[15];
    for (int j=0; j<15;j++)
    {
        tables[j].distances = &dist[j];
        tables[j].cities = &cities[j];
    }
    printf("\"Traveling Salesman\"-Problem\n");
    readCities(dist, tables);
    for(int i=0; i<sizeof(dist);i++)
    {
        printf("%d %d %d\n",dist[i].from+1, dist[i].to+1, dist[i].dist);
    }
    return 0;
}
