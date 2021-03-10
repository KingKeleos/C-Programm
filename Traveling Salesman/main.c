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

void readCities(DistanceTable* tables)
{
    char filename[128];
    int lines=0;
    char *string; //String erstellen aus chars
    char clipboard[128]={0};
    printf("Datei laden:\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename); //Nutzer kann seine Datei aussuchen
    FILE *fptr=fopen(filename,"r"); //Ausgewählte datei wird nur gelese;
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden!\n"); //Fehlerwurf, bei nicht existenter Zeile
        fflush(fptr);
        return NULL;
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    int i=0;
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while(string!=NULL)
    {
        lines++;
        string=strtok(NULL," \t");
    }
    tables->cities = malloc(lines * sizeof(char*));
    tables->n=lines;
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while (string != NULL)
    {
        if(string[strlen(string)-1]=='\n')
        {
            string[strlen(string)-1]=0;
        }
        tables->cities[i]= malloc((strlen(string)+1)*sizeof(char));
        tables->cities[i]= strcpy(tables->cities[i],string);
        i++;
        string=strtok(NULL, " \t");
    }
    for (int j=0; j<i;j++)
    {
        printf("%d %s\n",j+1, tables->cities[j]);
    }
    printf("Es wurden %d Staedte gefunden\n", tables->n); // Bestaetigung, dass alle Staedte gefunden wurden.
    //int k=0;
    int n=0;
    int j=0;
    int k=0;
    tables->distances = malloc(tables->n*tables->n*sizeof(Distance));
    string=strtok(NULL, " \t");
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            tables->distances[k].from=n;
            tables->distances[k].to=j;
            tables->distances[k].dist=atoi(string);
            string=strtok(NULL, " \t");
            k++;
            j++;
        }
        j=0;
        n++;
    }
    fclose(fptr);
    free(tables);
}


int main()
{
    printf("\"Traveling Salesman\"-Problem\n");
    DistanceTable tables;
    quest: printf("Wollen sie die Tablelle laden? y/n: ");
    char ans;
    scanf("%c", &ans);
    int max;
    switch(ans)
    {
        case 'y': readCities(&tables); break;
        case 'n': printf("understandable have a great day\n"); return 0; break;
        default: printf("Bitte nur mit y oder n antworten\n");goto quest; break;
    }
    /*for (int i=0; i<(tables.n);i++)
    {
        printf("\n %d %s ", tables.n+1,tables.cities);
        for(int j=0; j<5;j++)
        {
            printf("%d ", tables.distances[j-4].dist);
        }
    }*/
    return 0;
}
