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

int readCities(Distance distances[], DistanceTable tables[])
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
    printf("Es wurden %d Staedte gefunden\n", i); // Bestaetigung, dass alle Staedte gefunden wurden.
    //int k=0;
    int n=0;
    int j=0;
    i=0;
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
                    tables[i].distances = &distances[n];
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
            tables[i].distances = &distances[n];
            n++;
            j++;
            i++;
    }
    fclose(fptr);
    return n;
}


int main(void)
{
    char cities[15][128];
    Distance dist[256];
    DistanceTable tables[15];
    for (int j=0; j<15;j++)
    {
        tables[j].cities = &cities[j];
    }
    printf("\"Traveling Salesman\"-Problem\n");
    quest: printf("Wollen sie die Tablelle laden? y/n: ");
    char ans;
    scanf("%c", &ans);
    int max;
    switch(ans)
    {
        case 'y': max = readCities(dist, tables); break;
        case 'n': printf("understandable have a great day\n"); return 0; break;
        default: printf("Bitte nur mit y oder n antworten\n");goto quest; break;
    }
    for (int i=0; i<tables[0].n;i++)
    {
        printf("\n %d %s ", tables[i].n+1,tables[i].cities);
        for(int j=0; j<5;j++)
        {
            printf("%d ", tables[i].distances[j-4].dist);
        }
    }
    return 0;
}
