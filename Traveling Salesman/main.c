#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct // Forgegebene Struktur
{
    int from; // Index der Anfangsstadt
    int to;   // Index der Zielstadt
    int dist; // Entfernung der St�dte
    } Distance;

typedef struct // Forgegebene Struktur
{
    int n; // Anzahl der St�dte
    char **cities; // Pointer Feld der Staedtenamen
    Distance *distances; //Pointer auf Feld vom Typ Distance
    } DistanceTable;

bool checkReadability(FILE *fpointer, DistanceTable* ptrTables)
{
    int lines=0; //Zeilen Z�hler
    int i=0;
    char clipboard[128]={0}; //Zwischenspeicher f�r die komplette Zeile
    fseek(fpointer, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    while(fgets(clipboard, 128, fpointer)!=NULL)
    {
        lines++;
    }
    fseek(fpointer, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fpointer);
    char *string=strtok(clipboard, " \t");
    while (string != NULL)
    {
        if(string[strlen(string)-1]=='\n')
        {
            string[strlen(string)-1]=0;
        }
        i++;
        if (isdigit(*string)) //Wenn Integer in Namen -> Fehler
        {
            return false;
        }
        string=strtok(NULL, " \t");
    }
    if (lines != i+1) //Matrix in der falschen Dimension -> Fehler
    {
        return false;
    }
    int n=0; // Z�hlervariable f�r die Zeilen
    int j=0; // Z�hlervariable f�r die Spalten
    string=strtok(NULL, " \t");
    while(fgets(clipboard, 128, fpointer)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            if (n==j && strcmp(string,"0")) //Abstand Stadt zu sich selbst ist ungleich 0 -> Fehler
            {
                return false;
            }
            if (n!=j && strcmp(string,"0")==0) //Distanz von unterschiedlichen Staedten ist 0 -> Fehler
            {
                return false;
            }
            if (strcmp(string," ")==0)
            {
                return false;
            }
            int negcomp = atoi(string);
            if (negcomp<0) //Negative Zahl finden -> Fehler
            {
                return false;
            }
            string=strtok(NULL, " \t");
            j++;
        }
        if(j!=i) //Tabelle nicht Vollst�ndig -> Fehler
        {
            return false;
        }
        j=0;
        n++;
    }
    return true;
}

DistanceTable* readCities(DistanceTable* tables)
{
    char filename[128];
    int lines=0;
    char *string; //String erstellen aus chars
    char clipboard[128]={0};
    printf("Datei laden:\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename); //Nutzer kann seine Datei aussuchen
    FILE *fptr=fopen(filename,"r"); //Ausgew�hlte datei wird nur gelesen
    bool readable = checkReadability(fptr, tables);
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden!\n"); //Fehlerwurf, bei nicht existenter Zeile
        fflush(fptr);
        tables =NULL;
        return tables;
    }
    if (readable==false)
    {
        printf("Die Datei ist nicht zulaessig!");//Fehlerausgabe
        exit(1);
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
    return tables;
}

void writeCities(DistanceTable* tables)
{
    printf("%d\n", tables->n);
    for (int i=0; i<(tables->n);i++)
    {
        printf("\n %s ",tables->cities[i]);
        for(int j=0; j<(tables->n*tables->n);j++)
        {
            printf("%d ",tables->distances[j].dist);
        }
    }
}

int main()
{
    printf("\"Traveling Salesman\"-Problem\n");
    DistanceTable tables;
    DistanceTable *ptrTables=NULL;
    char ans;
    while (ans!='a'||ans!='b'||ans!='c'||ans!='d'||ans!='e'||ans!='f')
    {
        printf("\nWaehlen Sie ihre Aktion: \n(a) Entfernungstabelle laden\n(b) Entfernungstabellespeichern\n(c) Entfernungstabelle anzeigen\n(d) Entfernung wischen zwei St�dten �ndern\n(e) Kuerzeste Route berechnen\n(f) Programm beenden\n");
        printf("\nIhre Auswahl: ");
        scanf("%s", &ans);
        switch(ans)
        {
            case 'a': ptrTables = readCities(&tables); break; //Function zum laden der Datei �ffnen
            case 'b': writeCities(&tables); break; //Funktion zum schreiben der Daten �ffnen
            case 'c': break;
            case 'd': break;
            case 'e': break;
            case 'f': printf("understandable have a great day\n"); return 0; break;
            default: printf("Bitte nur mit a,b,c,d,e oder f antworten\n"); ans=NULL; break;
        }
    }
    for (int i=0; i<(tables.n);i++)
    {
        printf("\n%d %s ",tables.n,tables.cities[i]);
        for(int j=0; j<(tables.n*tables.n);j++)
        {
            printf("%d ",tables.distances[j].dist);
        }}
    return 0;
}
