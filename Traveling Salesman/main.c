#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wchar.h>


int main(void)
{
    printf("\"Traveling Salesman\"-Problem\n");
    char cities[90][90];
    char c;
    char string[15][256];
    char pstring[15] = &string;
    int i=0;
    int j=0;
    int k=0;
    FILE *fptr;
    fptr=fopen("../Staedte.txt","r");
    if(fptr == NULL)
    {
        printf("Fehler, Datei nicht gefunden");
        exit(1);
    }
    fseek(fptr, 0, SEEK_SET);
    char table[15][15][256];
    while ( (c=fgetc(fptr)) !=EOF) //Jeden Character Lesen, bis zum Ende der Datei
    {
        printf("Gelesener Charater: %c\n",c);
        if (c==' ') //Wenn gelesener Character ein Leerzeichen -> vollen String in Tabelle eintragen.
        {
            i=i+1; //Eine Position weiter
        }
        if (c=='\n')
        {
            i=0; //wieder an den Anfang der Tabelle...
            j=j+1; // ...aber eine Zeile weiter
            printf("New Line\n");

        }
        else
        {
            table[i][j][k]=c;
            k=k+1;
        }
    }

    for(int i=0; i<sizeof(table);i=i+1)
    {
        for(int j=0; j<sizeof(table[i]);j+=1)
            {
                for (int k=0; k<sizeof(table[i][j]); k=k+1)
                {
                    printf("%s", table[i][j][k]);
                }
            }
    }

    int fclose(fptr);
    return 0;
}

int readTable(char* filename)
{
    int c;
    char cities[15][15];
    FILE *file;
    file=fopen(filename,"r");
    int fgetc (file);
    int fclose(file);
    printf("Datei wurde gelesen");
    return c;
}


//typedef struct // Forgegebene Struktur
//{
//    int from; // Index der Anfangsstadt
//    int to;   // Index der Zielstadt
//    int dist; // Entfernung der Städte
//    } Distance;

//typedef struct // Forgegebene Struktur
//{
//    int n; // Anzahl der Städte
//    char **cities; // Pointer auf Feld mit Städtenamen
//    Distance *distances; //Pointer auf Feld mit Entfernungen
//    } DistanceTable;
