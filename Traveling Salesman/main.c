#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct                                                                                                     // Forgegebene Struktur
{
    int from;                                                                                                      // Index der Anfangsstadt
    int to;                                                                                                        // Index der Zielstadt
    int dist;                                                                                                      // Entfernung der Städte
    } Distance;

typedef struct                                                                                                     // Forgegebene Struktur
{
    int n;                                                                                                         // Anzahl der Städte
    char **cities;                                                                                                 // Pointer Feld der Staedtenamen
    Distance *distances;                                                                                           //Pointer auf Feld vom Typ Distance
    } DistanceTable;

bool checkReadability(FILE *fpointer, DistanceTable* ptrTables)                                                    //Funktion Testet, ob die zu lesende Datei konform ist
{
    int lines=0;                                                                                                   //Zeilen Zähler
    int i=0;                                                                                                       //Laufvariable
    char clipboard[128]={0};                                                                                       //Zwischenspeicher für die komplette Zeile
    fseek(fpointer, 0, SEEK_SET);                                                                                  // Datei von Anfang an durchsuchen

    while(fgets(clipboard, 128, fpointer)!=NULL)                                                                   //While Schleife zählt die Anzahl der Zeilen
    {
        lines++;
    }

    fseek(fpointer, 0, SEEK_SET);                                                                                  // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fpointer);                                                                               //Eine Zeile komplett in die Zwischenablage kopieren
    char *string=strtok(clipboard, " \t");                                                                         //Zeile

    while (string != NULL)                                                                                         //Solange nicht das Ende der Datei erreicht wurde...
    {
        if(string[strlen(string)-1]=='\n')                                                                         //sobald das Ende der Zeile erreicht wurde...
        {
            string[strlen(string)-1]=0;                                                                            //... Dem String das letzte Character entfernen -> Bug entfernen
        }
        i++;
        if (isdigit(*string))                                                                                      //Wenn Integer in Namen -> Fehler
        {
            return false;
        }
        string=strtok(NULL, " \t");                                                                                //String wieder leeren
    }

    if (lines != i+1)                                                                                              //Matrix in der falschen Dimension -> Fehler
    {
        return false;
    }

    int n=0;                                                                                                       // Zählervariable für die Zeilen
    int j=0;                                                                                                       // Zählervariable für die Spalten
    string=strtok(NULL, " \t");

    while(fgets(clipboard, 128, fpointer)!=0)                                                                      //Solange eine neue Zeile gelesen werden kann wird eine neue gelesen
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            if (n==j && strcmp(string,"0"))                                                                        //Abstand Stadt zu sich selbst ist ungleich 0 -> Fehler
            {
                return false;
            }
            if (n!=j && strcmp(string,"0")==0)                                                                     //Distanz von unterschiedlichen Staedten ist 0 -> Fehler
            {
                return false;
            }
            if (strcmp(string," ")==0)                                                                             //Wenn ein String leer ist -> Fehler
            {
                return false;
            }
            int negcomp = atoi(string);                                                                            //geladenen String in int convertieren
            if (negcomp<0)                                                                                         //Negative Zahl finden -> Fehler
            {
                return false;
            }
            string=strtok(NULL, " \t");                                                                            //String leeren
            j++;
        }
        if(j!=i)                                                                                                   //Tabelle nicht Vollständig -> Fehler
        {
            return false;
        }
        j=0;
        n++;
    }
    return true;
}

DistanceTable* readCities(DistanceTable* tables, char filename[])                                                  //Funktion liest die komplette Datei, wenn sie passt
{
    int lines=0;                                                                                                   //ZeilenzählerVariable
    char *string;                                                                                                  //String erstellen aus chars
    char clipboard[128]={0};                                                                                       //zwischenspeicher
    printf("\n\n---------------| Datei laden |--------------\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename);                                                                                         //Nutzer kann seine Datei aussuchen
    FILE *fptr=fopen(filename,"r");                                                                                //Ausgewählte datei wird nur gelesen
    bool readable = checkReadability(fptr, tables);                                                                //Test, ob die Datei im richtigen "Format" ist
    if(fptr == NULL)
    {
        printf("\n !-------| Fehler, Datei nicht gefunden |------!\n");                                            //Fehlerwurf, bei nicht existenter Zeile
        tables =NULL;
        return tables;                                                                                             //Null Pointer ausgeben bei Fehler
    }
    if (readable==false)
    {
        printf("!------| Die Datei ist nicht zulaessig! |-----!\n");                                               //Fehlerausgabe bei nicht passender Datei
        tables =NULL;
        return tables;                                                                                             //Null Pointer ausgeben bei Fehler
    }
    fseek(fptr, 0, SEEK_SET);                                                                                      // Datei von Anfang an durchsuchen
    int i=0;
    fgets(clipboard, 128, fptr);                                                                                   //Zeile wird geladen
    string=strtok(clipboard, " \t");
    while(string!=NULL)
    {
        lines++;                                                                                                   //Zählrvariable hochzähhlen
        string=strtok(NULL," \t");
    }
    tables->cities = malloc(lines * sizeof(char*));                                                                //Speicher für die Städte wird angelegt
    tables->n=lines;                                                                                               //Anzahl der Städte der Struktur über den Pointer zuweisen
    fseek(fptr, 0, SEEK_SET);                                                                                      // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while (string != NULL)
    {
        if(string[strlen(string)-1]=='\n')
        {
            string[strlen(string)-1]=0;
        }
        tables->cities[i]= malloc((strlen(string)+1)*sizeof(char));                                                 //Dem Namen der Städte passenden Speicher anlegen
        tables->cities[i]= strcpy(tables->cities[i],string);                                                        //Über dem Pointer der Struktur den Namen der Stadt übergeben
        i++;
        string=strtok(NULL, " \t");
    }
    printf("_____________________________________________\n");
    printf("Es wurden %d Staedte gefunden\n", tables->n);                                                           // Bestaetigung, dass alle Staedte gefunden wurden.
    printf("_____________________________________________\n");
    for (int j=0; j<i;j++)
    {
        printf("%d %s\n",j+1, tables->cities[j]);                                                                   //Anzahl der Staedte wiedergeben
    }
    printf("_____________________________________________\n");
    int n=0;
    int j=0;
    int k=0;
    tables->distances = malloc(tables->n*tables->n*sizeof(Distance));                                               //Speicher für die Distanzen Anlegen mit (tables->n)^2 weil jede Stadt mit Jeder eine Distanz hat
    string=strtok(NULL, " \t");
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            tables->distances[k].from=n;                                                                            //Über den Tables Pointer dem Wert "from" von Distance den aktuellen Index zuweisen
            tables->distances[k].to=j;                                                                              //Über den Tables Pointer dem Wert "to" von Distance den aktuellen Index zuweisen
            tables->distances[k].dist=atoi(string);                                                                 //Gelesenen String als Int in über den Pointer in Distancectable der Distanz zuweisen.
            string=strtok(NULL, " \t");
            k++;
            j++;
        }
        j=0;
        n++;
    }
    fclose(fptr);                                                                                                   //Dateizugriff schließen, um Datei zu schreiben
    return tables;                                                                                                  //Pointer zurück geben
}

void writeCities(DistanceTable* tables)
{
    if (tables->n==0)                                                                                               //Null-Pointer -> kann nichts speichern.
    {
        printf("Keine Daten zum Speichern vorhanden.\n"); //
    }
    else
    {
        char saveFilename[128];                                                                                     //Speichername getrennt von Ladenamen
        char expoDist[15];
        printf("-----------| Daten Speichern |----------\n\n");
        printf("Wie soll die Datei heissen? -> ");
        scanf("%s", &saveFilename);                                                                                 //Dateinamen des Users auslesen
        FILE *fptr=fopen(saveFilename,"w");                                                                         //Ausgewählte datei wird nur geschrieben
        fseek(fptr, 0, SEEK_SET);                                                                                   // Datei von Anfang an durchsuchen
        for (int i=0; i<tables->n-1; i++)                                                                           //Bei jeder Stadt, bis auf der letzen..
        {
            char *citieform = tables->cities[i];                                                                    //citieform liest den Namen der Stadt aus
            strncat(citieform," ",1);                                                                               //Dem Namen der Stadt ein Leerzeichen hinzufügen um differenzieren zu können
            fputs(citieform,fptr);                                                                                  //Transformierten Namen der Stadt in die Datei schreiben
        }
        fputs(tables->cities[tables->n-1],fptr);                                                                    //Letzte Stadt ohne Leerzeichen speichern
        fputs("\n",fptr);                                                                                           //Zeilen Umbruch
        int k=0;
        for (int i =0; i<tables->n;i++)                                                                             //5=tables->n
        {
            for (int j=0; j<tables->n-1;j++)                                                                        //5tables->n
            {
                int distform = tables->distances[k].dist;                                                           //Distanz aus der Struktur lesen
                sprintf(expoDist, "%d",distform);
                strncat(expoDist," ",1);                                                                            //Leerzeichen anhängen
                k++;
                fputs(expoDist,fptr);                                                                               //In Datei ausgeben
            }
            int distform = tables->distances[k].dist;                                                               //Letzer Schritt der Zeile wird ohne Leerzeichen  ausgeführt
            sprintf(expoDist, "%d",distform);
            k++;
            fputs(expoDist,fptr);
            if (i<5-1)                                                                                              //letzten Zeilenummbruch abfangen, damit Matrix erhalten bleibt //5=tables->n
            {
                fputs("\n",fptr);                                                                                   //Zeilenumbruch am Ende der Zeile
            }
        }
        fclose(fptr);
        printf("Daten in %s gespeichert", saveFilename);
    }
}
bool checkChanges(DistanceTable* tables, char filename[])
{
    char *string;
    char clipboard[128]={0};
    int lines=0;
    FILE *fptr=fopen(filename,"r");                                                                                 //Zuletzt geladene Datei wird geöffnet
    if(fptr == NULL)                                                                                                //Wenn nichts geladen ist...
    {
        return false;                                                                                               //...kann nichts gespeichert werden.
    }
    fseek(fptr, 0, SEEK_SET);                                                                                       // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while(string!=NULL)
    {
        lines++;
        string=strtok(NULL," \t");
    }
    if (lines!=tables->n)                                                                                           //Wenn die Anzahl der Zeilen sich von der letzten Datei unterscheidet
    {
        return true;                                                                                                //Wahr ausgeben
    }
    fseek(fptr, 0, SEEK_SET);                                                                                       // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    int i=0;
    while (string != NULL)
    {
        if(string[strlen(string)-1]=='\n')
        {
            string[strlen(string)-1]=0;
        }
        i++;
        string=strtok(NULL, " \t");
    }
    int k=0;                                                                                                        //DistanzIndex
    int n=0;                                                                                                        //FromIndex
    int j=0;                                                                                                        //ToIndex
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            if(tables->distances[k].from!=n)                                                                        //Wenn sich die Position geändert hat...
            {
                return true;                                                                                        //...Wahr ausgeben
            }
            if(tables->distances[k].to!=j)                                                                          //Wenn sich die Position geändert hat...
            {
                return true;                                                                                        //...Wahr ausgeben
            }
            if(tables->distances[k].dist!=atoi(string))                                                             //Wenn sich der Wert in der aktuellen Position geändert hat
            {
                return true;                                                                                        //...Wahr ausgeben
            }
            string=strtok(NULL, " \t");
            k++;
            j++;
        }
        j=0;
        n++;
    }

    return false;
}


int main()
{
    char readFile[128];
    printf("\"Traveling Salesman\"-Problem\n");
    DistanceTable tables;                                                                                           //Struktur DistanceTable als Teil der main definieren, damit sie drauf zugreigen kann
    DistanceTable *ptrTables=NULL;
    char ans;                                                                                                       //Character, der die EIngaben des Nutzers liest
    while (ans!='a'||ans!='b'||ans!='c'||ans!='d'||ans!='e'||ans!='f')                                              //Schleife wieder holen, bis mindestens eins der Zeichen geschrieben wurde
    {
        printf("\n---------| Waehlen Sie ihre Aktion |---------\n\n");
        printf("(a) Entfernungstabelle laden\n");
        printf("(b) Entfernungstabellespeichern\n");
        printf("(c) Entfernungstabelle anzeigen\n");
        printf("(d) Entfernung wischen zwei Staedten aendern\n");
        printf("(e) Kuerzeste Route berechnen\n");
        printf("(f) Programm beenden\n");
        printf("\n--------------| Ihre Auswahl |---------------\n");
        printf("-> ");
        scanf("%s", &ans);                                                                                          //Antwort in den char ans lesen
        switch(ans)                                                                                                 //Abfrage aller Antwortmöglichkeiten
        {
            case 'a': ptrTables = readCities(&tables, &readFile);                                                   //Function zum laden der Datei öffnen
                      break;
            case 'b': writeCities(&tables);                                                                         //Funktion zum schreiben der Daten öffnen
                      break;
            case 'c':
                        break;
            case 'd': break;
            case 'e': break;
            case 'f': if ((ptrTables!=NULL) && checkChanges(&tables, &readFile))                                    //Erst schauen, ob  ein Pointer erstellt wurde UND die Daten ich geändert haben
                      {
                        char save;
                        while (save!='y'||save!='n')                                                                //Antwortmöglichkeiten auf y oder n reduzieren und wiederholen
                        {
                            printf("Sie haben veraenderte ungespeicherte Daten, wollen Sie sie speichern? y/n:\n");
                            scanf("%s",&save);
                            if (save == 'y')                                                                        //Wenn der Nutzer speichern möchte...
                            {
                                writeCities(&tables);                                                               //...Speicherfunktion aufrufen
                            }
                            else
                            {
                                printf("understandable have a great day\n");                                        //Nachricht, dass das Programm beendet wurde
                                return 0;
                            }
                        }
                      }
                      printf("understandable have a great day\n");                                                  //Nachricht, dass das Programm beendet wurde
                      return 0;
                      break;
            default: printf("Bitte nur mit a,b,c,d,e oder f antworten\n"); ans=NULL; break;                         //Abfang, wenn ein anderer Buchstabe gesendet wurde
        }
    }
}
