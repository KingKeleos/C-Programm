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

bool checkReadability(FILE *fpointer, DistanceTable* ptrTables) //Funktion Testet, ob die zu lesende Datei konform der Regeln ist
{
    int lines=0; //Zeilen Z�hler
    int i=0; //Laufvariable
    char clipboard[128]={0}; //Zwischenspeicher f�r die komplette Zeile
    fseek(fpointer, 0, SEEK_SET); // Datei von Anfang an durchsuchen

    while(fgets(clipboard, 128, fpointer)!=NULL) //While Schleife z�hlt die Anzahl der Zeilen
    {
        lines++;
    }

    fseek(fpointer, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fpointer); //Eine Zeile komplett in die Zwischenablage kopieren
    char *string=strtok(clipboard, " \t"); //Zeile

    while (string != NULL) //Solange nicht das Ende der Datei erreicht wurde...
    {
        if(string[strlen(string)-1]=='\n') //sobald das Ende der Zeile erreicht wurde...
        {
            string[strlen(string)-1]=0; //... Dem String das letzte Character entfernen -> Bug entfernen
        }
        i++;
        if (isdigit(*string)) //Wenn Integer in Namen -> Fehler
        {
            return false;
        }
        string=strtok(NULL, " \t"); //String wieder leeren
    }

    if (lines != i+1) //Matrix in der falschen Dimension -> Fehler
    {
        return false;
    }

    int n=0; // Z�hlervariable f�r die Zeilen
    int j=0; // Z�hlervariable f�r die Spalten
    string=strtok(NULL, " \t");

    while(fgets(clipboard, 128, fpointer)!=0) //Solange eine neue Zeile gelesen werden kann wird eine neue gelesen
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
            if (strcmp(string," ")==0) //Wenn ein String leer ist -> Fehler
            {
                return false;
            }
            int negcomp = atoi(string); //geladenen String in int convertieren
            if (negcomp<0) //Negative Zahl finden -> Fehler
            {
                return false;
            }
            string=strtok(NULL, " \t"); //String leeren
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

DistanceTable* readCities(DistanceTable* tables, char filename[]) //Funktion liest die komplette Dateim, wenn sie passt
{
    int lines=0; //Zeilenz�hlerVariable
    char *string; //String erstellen aus chars
    char clipboard[128]={0}; //zwischenspeicher
    printf("\n\n---------------| Datei laden |--------------\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename); //Nutzer kann seine Datei aussuchen
    FILE *fptr=fopen(filename,"r"); //Ausgew�hlte datei wird nur gelesen
    bool readable = checkReadability(fptr, tables); //Test, ob die Datei im richtigen "Format" ist
    if(fptr == NULL)
    {
        printf("\n !-------| Fehler, Datei nicht gefunden |------!\n"); //Fehlerwurf, bei nicht existenter Zeile
        tables =NULL;
        return tables;//Null Pointer ausgeben bei Fehler
    }
    if (readable==false)
    {
        printf("!------| Die Datei ist nicht zulaessig! |-----!\n");//Fehlerausgabe bei nicht passender Datei
        tables =NULL;
        return tables;//Null Pointer ausgeben bei Fehler
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    int i=0;
    fgets(clipboard, 128, fptr); //Zeile wird geladen
    string=strtok(clipboard, " \t");
    while(string!=NULL)
    {
        lines++; //Z�hlrvariable hochz�hhlen
        string=strtok(NULL," \t");
    }
    tables->cities = malloc(lines * sizeof(char*)); //Speicher f�r die St�dte wird angelegt
    tables->n=lines; //Anzahl der St�dte der Struktur �ber den Pointer zuweisen
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while (string != NULL)
    {
        if(string[strlen(string)-1]=='\n')
        {
            string[strlen(string)-1]=0;
        }
        tables->cities[i]= malloc((strlen(string)+1)*sizeof(char)); //Dem Namen der St�dte passenden Speicher anlegen
        tables->cities[i]= strcpy(tables->cities[i],string); //�ber dem Pointer der Struktur den Namen der Stadt �bergeben
        i++;
        string=strtok(NULL, " \t");
    }
    printf("_____________________________________________\n");
    printf("Es wurden %d Staedte gefunden\n", tables->n); // Bestaetigung, dass alle Staedte gefunden wurden.
    printf("_____________________________________________\n");
    for (int j=0; j<i;j++)
    {
        printf("%d %s\n",j+1, tables->cities[j]); //Anzahl der Staedte wiedergeben
    }
    printf("_____________________________________________\n");
    int n=0;
    int j=0;
    int k=0;
    tables->distances = malloc(tables->n*tables->n*sizeof(Distance)); //Speicher f�r die Distanzen Anlegen mit (tables->n)^2 weil jede Stadt mit Jeder eine Distanz hat
    string=strtok(NULL, " \t");
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            tables->distances[k].from=n;//�ber den Tables Pointer dem Wert "from" von Distance den aktuellen Index zuweisen
            tables->distances[k].to=j; //�ber den Tables Pointer dem Wert "to" von Distance den aktuellen Index zuweisen
            tables->distances[k].dist=atoi(string); //Gelesenen String als Int in �ber den Pointer in Distancectable der Distanz zuweisen.
            string=strtok(NULL, " \t");
            k++;
            j++;
        }
        j=0;
        n++;
    }
    fclose(fptr); //Dateizugriff schlie�en, um Datei zu schreiben
    return tables; //Pointer zur�ck geben
}

void writeCities(DistanceTable* tables)
{
    if (tables->n==0) //Null-Pointer -> kann nichts speichern.
    {
        printf("Keine Daten zum Speichern vorhanden.\n"); //
    }
    else
    {
        char saveFilename[128]; //Speichername getrennt von Ladenamen
        char expoDist[15];
        printf("-----------| Daten Speichern |----------\n\n");
        printf("Wie soll die Datei heissen? -> ");
        scanf("%s", &saveFilename); //Dateinamen des Users auslesen
        FILE *fptr=fopen(saveFilename,"w"); //Ausgew�hlte datei wird nur geschrieben
        fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
        for (int i=0; i<tables->n-1; i++) //Bei jeder Stadt, bis auf der letzen..
        {
            char *citieform = tables->cities[i]; //citieform liest den Namen der Stadt aus
            strncat(citieform," ",1); //Dem Namen der Stadt ein Leerzeichen hinzuf�gen um differenzieren zu k�nnen
            fputs(citieform,fptr); //Transformierten Namen der Stadt in die Datei schreiben
        }
        fputs(tables->cities[tables->n-1],fptr); //Letzte Stadt ohne Leerzeichen speichern
        fputs("\n",fptr); //Zeilen Umbruch
        int k=0;
        for (int i =0; i<tables->n;i++) //5=tables->n
        {
            for (int j=0; j<tables->n-1;j++) //5tables->n
            {
                int distform = tables->distances[k].dist; //Distanz aus der Struktur lesen
                sprintf(expoDist, "%d",distform);
                strncat(expoDist," ",1); //Leerzeichen anh�ngen
                k++;
                fputs(expoDist,fptr); //In Datei ausgeben
            }
            int distform = tables->distances[k].dist; //Letzer Schritt der Zeile wird ohne Leerzeichen  ausgef�hrt
            sprintf(expoDist, "%d",distform);
            k++;
            fputs(expoDist,fptr);
            if (i<tables->n-1) //letzten Zeilenummbruch abfangen, damit Matrix erhalten bleibt //5=tables->n
            {
                fputs("\n",fptr); //Zeilenumbruch am Ende der Zeile
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
    FILE *fptr=fopen(filename,"r"); //Zuletzt geladene Datei wird ge�ffnet
    if(fptr == NULL) //Wenn nichts geladen ist...
    {
        return false;//...kann nichts gespeichert werden.
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while(string!=NULL)
    {
        lines++;
        string=strtok(NULL," \t");
    }
    if (lines!=tables->n) //Wenn die Anzahl der Zeilen sich von der letzten Datei unterscheidet
    {
        return true; //Wahr ausgeben
    }
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
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
    if (i!=(sizeof(tables->cities))+1) //Wenn die Anzal der Eintr�ge der Zeile sich ge�ndert hat...
    {
        return true; //..Wahr ausgeben
    }
    int k=0; //DistanzIndex
    int n=0; //FromIndex
    int j=0; //ToIndex
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            if(tables->distances[k].from!=n) //Wenn sich die Position ge�ndert hat...
            {
                return true; //...Wahr ausgeben
            }
            if(tables->distances[k].to!=j) //Wenn sich die Position ge�ndert hat...
            {
                return true; //...Wahr ausgeben
            }
            if(tables->distances[k].dist!=atoi(string)) //Wenn sich der Wert in der aktuellen Position ge�ndert hat
            {
                return true; //...Wahr ausgeben
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

void freeSpace(DistanceTable* tables)
{
    for (int i=0; i>tables->n;i++)
    {
        free(tables->cities[i]); //St�dte aus den Speicher werfen
    }
    for (int i=0; i>(tables->n*tables->n);i++) // Die dazu geh�rigen Werte aus dem Speicher werfen
    {
        free(tables->distances[i].from);
        free(tables->distances[i].to);
        free(tables->distances[i].dist);
    }
    free(tables->n); //Zuletzt die Anzahl der St�dte freigeben
}

void swap(int *a, int *b)
{
   int t;

   t  = *b;
   *b = *a;
   *a = t;
}

void heuristic(DistanceTable* tables, int start)
{
int currentCity = start;
/*printf("%d\n",start);
printf("%d\n",currentCity);*/
int sumOfDistance=0;
int usedCity[tables->n];
int compareDist = 2147483647;
int compareCity;
int testtest;
/*for(int i=0; i<tables->n*tables->n; i++)
    printf("%d: %d\n", i, tables->distances[i].dist);*/
for(int i=0; i<tables->n; i++)
{
    usedCity[i]=0;
    //printf("%d\n", usedCity[i]);
}
for(int j=0; j<tables->n; j++)
{
for(int i=0; i<tables->n*tables->n; i++)
{
    if(currentCity == tables->distances[i].from)
    {
        //printf("%d ", i);
        if(currentCity != tables->distances[i].to)
        {
            //printf("%d ", i);
            if(usedCity[i%tables->n]==0)
            {
                //printf("%d ", i);
                if(tables->distances[i].to != start || j == tables->n-1)
                {
                    //printf("%d ", i);
                    if(tables->distances[i].dist<compareDist)
                    {
                        testtest=tables->distances[i].dist;
                        //printf("%d ", testtest);
                        compareDist=tables->distances[i].dist;
                        //printf("%d ", compareDist);
                        compareCity=tables->distances[i].to;
                    }
                }
            }
        }
    }
}

sumOfDistance=sumOfDistance+compareDist;
compareDist= 2147483647;
//printf("\n Distanz betraegt: %d ", sumOfDistance);
usedCity[compareCity]=1;
currentCity=compareCity;
//printf("\n");
}
printf("\n Die kuerzeste Route berechnet nach dem heuristischen Verfahren mit der\nStartstadt %s betraegt: %d \n", tables->cities[start], sumOfDistance);
}

int calcPermutations(DistanceTable* tables, int start, int permutation[], int compareDist)
{
    int sumDist=0;
    int currentCity=start;
    int travelRoute[tables->n+1];
    travelRoute[0]=start;
    travelRoute[tables->n]=start;
    for(int i=1; i<tables->n; i++)
    {
        travelRoute[i]=permutation[i-1];
    }

    /*for(int i=0; i<tables->n+1;i++)
            printf("%d -> ", travelRoute[i]);
            printf("\n");
    */
    for(int i=0; i<tables->n; i++)
    {
        int j=currentCity*tables->n+travelRoute[i+1];
        //printf("%d\n", j);
        sumDist=sumDist+tables->distances[j].dist;
        currentCity=travelRoute[i+1];
    }
   return sumDist;
}

int permutate(DistanceTable* tables, int start, int permutation[], int cid, int compareDist, int counter, int* distances[], int sizearray)
{
    int dist=0;
    int copyPermutation[tables->n-1];
    for(int i=0; i<tables->n-1; i++)
    copyPermutation[i]=permutation[i];
    dist=calcPermutations(tables, start, permutation, compareDist);
    if(dist<compareDist)
    compareDist=dist;


    /*
    for (int i=cid;i<tables->n-1;i++)
     {
        swap(&permutation[cid],&permutation[i]);
        cid++;
        if(cid == tables->n-1)
        {
        int dist=calcPermutations(tables, start, permutation, compareDist);
        //printf("Index: %d\n", cid);
       // printf("Buffer: %d\n", dist);
       if(dist<compareDist)
        {distances[counter]=dist;
        //printf("Geringste: %d\n", compareDist);
        //printf("%d\n",counter);
        for(int i=0; i<tables->n-1;i++)
        printf("%d ", permutation[i]);
        printf("%d\n", distances[counter]);
        counter++;
        compareDist=dist;
        }
        permutate(tables, start, permutation, cid, compareDist, counter, distances);
        cid--;
        swap(&permutation[cid],&permutation[i]);
     }
    return dist;*/
}


void exact(DistanceTable* tables, int start)
{
    int temp;
    int counter=0;
    int compareDist = 2147483647;
    int permutation[tables->n-1];
    int k=0;
    int cid=0;
    int final=0;
    int sizearray=1;
     for(int i=1; i<tables->n; i++)
        sizearray=sizearray*i;
     int distances[sizearray];
    for(int i=0; i<tables->n; i++)
    {
        permutation[i]=0;
        if(tables->distances[i].to != start)
        {
            permutation[k]=tables->distances[i].to;
            k++;
        }
    }
   /* for(int i=0; i<tables->n-1;i++)
    printf("%d ", permutation[i]);*/
    // Permutationen werden gebildet
    printf("\n");
    final=permutate(tables, start, permutation, cid, compareDist, counter, &distances, sizearray);
    printf("\n Die kuerzeste Route berechnet nach dem exakten Verfahren mit der\nStartstadt %s betraegt: %d \n", tables->cities[start], final);

}

void shortestRoute(DistanceTable* tables)
{
    int start;
    int pos;
    char methode;
    int compare=0;
    printf("\n\n--------------| Route berechnen |------------\n\n");
    do
    {
    printf("Nennen Sie mir die Startstadt aus folgender Liste:\n");
    for(int i=0; i<tables->n; i++)
    {
        printf("%d: %s\n", i, tables->cities[i]);
    }
    printf("\n");
    scanf("%d", &start);

        for(int i=0; i<tables->n; i++)
        {
            if(start == tables->distances[i].to)
            {
                i=tables->n;
                printf("%s ist als Startstadt festgelegt.\n", tables->cities[start]);
                compare=1;
            }
        }
    if(compare==0)
    printf("Die Startstadt ist nicht in der Liste enthalten.\n");
    }while(compare!=1);
    int give = start;
    do
    {
    printf("Waehlen Sie nun die Berechnungsmethode:\n");
    printf("(a) Heuristische Verfahren\n");
    printf("(b) Exakte Verfahren\n");
    scanf("%s", &methode);
    switch(methode)
        {
    case 'a': heuristic(tables, give); break;
    case 'b': exact(tables, give); break;
    default: printf("Keine gueltige Eingabe.\n"); break;
    }
    }while(methode !='a' && methode != 'b');
}

int main()
{
    char readFile[128];
    printf("\"Traveling Salesman\"-Problem\n");
    DistanceTable tables; //Struktur DistanceTable als Teil der main definieren, damit sie drauf zugreigen kann
    DistanceTable *ptrTables=NULL;
    char ans; //Character, der die EIngaben des Nutzers liest
    while (ans!='a'||ans!='b'||ans!='c'||ans!='d'||ans!='e'||ans!='f') //Schleife wieder holen, bis mindestens eins der Zeichen geschrieben wurde
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
        scanf("%s", &ans); //Antwort in den char ans lesen
        switch(ans) //Abfrage aller Antwortm�glichkeiten
        {
            case 'a': ptrTables = readCities(&tables, &readFile); //Function zum laden der Datei �ffnen
                      break;
            case 'b': writeCities(&tables);//Funktion zum schreiben der Daten �ffnen
                      break;
            case 'c': break;
            case 'd': break;
            case 'e': shortestRoute(&tables);break;
            case 'f': if (checkChanges(&tables, &readFile)&& ptrTables) //Erst schauen, ob die Daten ich ge�ndert haben
                      {
                        char save;
                        while (save!='y'||save!='n') //Antwortm�glichkeiten auf y oder n reduzieren und wiederholen
                        {
                            printf("Sie haben veraenderte ungespeicherte Daten, wollen Sie sie speichern? y/n:\n");
                            scanf("%s",&save);
                            if (save == 'y') //Wenn der Nutzer speichern m�chte...
                            {
                                writeCities(&tables); //...Speicherfunktion aufrufen
                            }
                            else
                            {
                                freeSpace(&tables); //..Nach beendigung Speicher wieder frei geben
                                printf("understandable have a great day\n"); //Nachricht, dass das Programm beendet wurde
                                return 0;
                            }
                        }
                      }
                      freeSpace(&tables); //..Nach beendigung Speicher wieder frei geben
                      printf("understandable have a great day\n"); //Nachricht, dass das Programm beendet wurde
                      return 0;
                      break;
            default: printf("Bitte nur mit a,b,c,d,e oder f antworten\n"); ans=NULL; break; //Abfang, wenn ein anderer Buchstabe gesendet wurde
        }
    }
}
