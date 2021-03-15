// Gruppenarbeit von Clemens Kund, Andy Kranz und Denis Binder.
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
    int dist; // Entfernung der Städte
    } Distance;

typedef struct // Forgegebene Struktur
{
    int n; // Anzahl der Städte
    char **cities; // Pointer Feld der Staedtenamen
    Distance *distances; //Pointer auf Feld vom Typ Distance
    } DistanceTable;

void deleteSpace(DistanceTable* tables)                                                                            //Funktion leert den Speicher
{
    for (int i=0; i<tables->n;i++)
    {
        free(tables->cities[i]);                                                                                   //Allokierter Speicher wird für die Staedte freigegeben
    }
        free(tables->distances);                                                                          //Allokierter Speicher für alle Daten in der Distanz Struktur wird freigegeben
}

bool checkReadability(FILE *fpointer, DistanceTable* ptrTables) //Funktion Testet, ob die zu lesende Datei konform der Regeln ist
{
    int lines=0; //Zeilen Zähler
    int i=0; //Laufvariable
    char clipboard[128]={0}; //Zwischenspeicher für die komplette Zeile
    fseek(fpointer, 0, SEEK_SET); // Datei von Anfang an durchsuchen

    while(fgets(clipboard, 128, fpointer)!=NULL) //While Schleife zählt die Anzahl der Zeilen
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

    int n=0; // Zählervariable für die Zeilen
    int j=0; // Zählervariable für die Spalten
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
        if(j!=i) //Tabelle nicht Vollständig -> Fehler
        {
            return false;
        }
        j=0;
        n++;
    }
    return true;
}

DistanceTable* readCities(DistanceTable* tables, char filename[]) //Funktion liest die komplette Datei, wenn sie passt
{
    int lines=0; //ZeilenzählerVariable
    char *string; //String erstellen aus chars
    char clipboard[128]={0}; //zwischenspeicher
    printf("\n\n---------------| Datei laden |--------------\n\n");
    printf("Wie heisst die Datei: ");
    scanf("%s", filename); //Nutzer kann seine Datei aussuchen
    FILE *fptr=fopen(filename,"r"); //Ausgewählte datei wird nur gelesen
    if(fptr == NULL)
    {
        printf("\n !-------| Fehler, Datei nicht gefunden |------!\n"); //Fehlerwurf, bei nicht existenter Zeile
        tables =NULL;
        return tables;//Null Pointer ausgeben bei Fehler
    }
    bool readable = checkReadability(fptr, tables); //Test, ob die Datei im richtigen "Format" ist
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
        lines++; //Zählrvariable hochzähhlen
        string=strtok(NULL," \t");
    }
    tables->cities = malloc(lines * sizeof(char*)); //Speicher für die Städte wird angelegt
    tables->n=lines; //Anzahl der Städte der Struktur über den Pointer zuweisen
    fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
    fgets(clipboard, 128, fptr);
    string=strtok(clipboard, " \t");
    while (string != NULL)
    {
        if(string[strlen(string)-1]=='\n')
        {
            string[strlen(string)-1]=0;
        }
        tables->cities[i]= malloc((strlen(string)+1)*sizeof(char)); //Dem Namen der Städte passenden Speicher anlegen
        tables->cities[i]= strcpy(tables->cities[i],string); //Über dem Pointer der Struktur den Namen der Stadt übergeben
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
    tables->distances = malloc(tables->n*tables->n*sizeof(Distance)); //Speicher für die Distanzen Anlegen mit (tables->n)^2 weil jede Stadt mit Jeder eine Distanz hat
    string=strtok(NULL, " \t");
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            tables->distances[k].from=n;//Über den Tables Pointer dem Wert "from" von Distance den aktuellen Index zuweisen
            tables->distances[k].to=j; //Über den Tables Pointer dem Wert "to" von Distance den aktuellen Index zuweisen
            tables->distances[k].dist=atoi(string); //Gelesenen String als Int in über den Pointer in Distancectable der Distanz zuweisen.
            string=strtok(NULL, " \t");
            k++;
            j++;
        }
        j=0;
        n++;
    }
    fclose(fptr); //Dateizugriff schließen, um Datei zu schreiben
    return tables; //Pointer zurück geben
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
        FILE *fptr=fopen(saveFilename,"w"); //Ausgewählte datei wird nur geschrieben
        fseek(fptr, 0, SEEK_SET); // Datei von Anfang an durchsuchen
        for (int i=0; i<tables->n-1; i++) //Bei jeder Stadt, bis auf der letzen..
        {
            fputs(tables->cities[i], fptr);//Schreibt Aktuelle Staedtenamen in die Datei
            fputs(" ", fptr);//Schreibt ein Leerzeichen zur Differenzierung
        }
        fputs(tables->cities[tables->n-1],fptr); //Letzte Stadt ohne Leerzeichen speichern
        fputs("\n",fptr); //Zeilen Umbruch
        int k=0;
        for (int i =0; i<tables->n;i++)
        {
            for (int j=0; j<tables->n-1;j++)
            {
                sprintf(expoDist, "%d",tables->distances[k].dist);
                fputs(expoDist,fptr); //In Datei ausgeben
                fputs(" ", fptr);//Schreibt ein Leerzeichen zur Differenzierung
                k++;
            }
            sprintf(expoDist, "%d",tables->distances[k].dist);
            fputs(expoDist,fptr);
            k++;
            if (i<tables->n-1) //letzten Zeilenummbruch abfangen, damit Matrix erhalten bleibt
            {
                fputs("\n",fptr); //Zeilenumbruch am Ende der Zeile
            }
        }
        fclose(fptr);
        printf("Daten in %s gespeichert\n", saveFilename);
    }
}
bool checkChanges(DistanceTable* tables, char filename[])
{
    char *string;
    char clipboard[128]={0};
    int lines=0;
    FILE *fptr=fopen(filename,"r"); //Zuletzt geladene Datei wird geöffnet
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
    if (lines!=5) //Wenn die Anzahl der Zeilen sich von der letzten Datei unterscheidet
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
    int k=0; //DistanzIndex
    int n=0; //FromIndex
    int j=0; //ToIndex
    while(fgets(clipboard, 128, fptr)!=0)
    {
        string=strtok(clipboard, " \t");
        while(string!=NULL)
        {
            if(tables->distances[k].from!=n) //Wenn sich die Position geändert hat...
            {
                return true; //...Wahr ausgeben
            }
            if(tables->distances[k].to!=j) //Wenn sich die Position geändert hat...
            {
                return true; //...Wahr ausgeben
            }
            if(tables->distances[k].dist!=atoi(string)) //Wenn sich der Wert in der aktuellen Position geändert hat
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
        free(tables->cities[i]); //Städte aus den Speicher werfen
    }
    for (int i=0; i>(tables->n*tables->n);i++) // Die dazu gehörigen Werte aus dem Speicher werfen
    {
        free(tables->distances[i].from);
        free(tables->distances[i].to);
        free(tables->distances[i].dist);
    }
    free(tables->n); //Zuletzt die Anzahl der Städte freigeben
}

void printtable(DistanceTable* tables)                                                                     //gibt die Tabelle aus
{
    if(tables->n == 0)
    {
        printf("Es sind keine Daten vorhanden.");
        return;
    }
    int citiesMax=0;
    int d[tables->n];
    int f;

    for (int i=0;i<tables->n;i++){                                                                                 //findet den längsten Städtenamen, um das Format der Tabelle anzupassen
            int j=0;
        while(tables->cities[i][j]!=0){
            j++;
        }
        if (j>citiesMax){
            citiesMax=j;
        }
        d[i]=j;
        }

        for (int j=0;j<citiesMax;j++){
            printf(" ");
        }


    printf("| ");                                                                                          //fügt in der Tabelle eine Trennlinie hinzu
    for (int i=0;i<tables->n;i++)
    {
        printf("%s | ", tables->cities[i]);

    }
    printf("\n");
    int distIndex=0;

    for (int i=0;i<tables->n;i++){                                                      //gibt die komplette Tabelle aus
            f=citiesMax-d[i];
            printf("%s", tables->cities[i]);                                                               //als erstes wird die Stadt ausgegeben
            for (int j=0;j<f;j++){
                printf(" ");
            }
        for (int c=0;c<tables->n;c++){                                                 //die schleife formartiert die Tabelle
                int count=0;
                int cmp=tables->distances[distIndex].dist;
                while(cmp){                                                                                //hier wird die Anzahl der Stellen zum darstellen berechnet
                    count++;
                    cmp/=10;
                }
            if(count==0){
                count=1;
            }
            int b=((d[c]+2)-count)/2;                                                                      //berechnet die Anzahl der Leerzeichen vor und nach der Zahl
            int e;
            if(((d[c]+2)-count)%2){
                e=b+1;
            }else{
                e=b;
            }

            printf("|");
            for(int d=0;d<b;d++){
                printf(" ");
            }
            printf("%d", tables->distances[distIndex].dist);
            distIndex++;
            for(int d=0;d<e;d++){
                printf(" ");
            }
        }
        printf("|");
        printf("\n");
    }
}

bool citieExist(DistanceTable* tables,char citieSave[]){                                                   //Testet, ob die eingegebene Stadt existiert
    for (int l=0;l<tables->n;l++){
        if (strcmp(tables->cities[l],citieSave)==0){                                                       //wenn eine Stadt gefunden wurde, gibt er "wahr" heraus
            return true;
        }
    }
    return false;
}
int getCitieIndex(DistanceTable* tables,char citieSave[]){                                                  //sucht nach dem Index der eingegebenen Stadt
    for (int l=0;l<tables->n;l++){
        if (strcmp(tables->cities[l],citieSave)==0){
            return l;
        }
    }
    return -1;
}

void changeDist(DistanceTable* tables){                                                                       //ist für die Änderung der Distanzen zuständig
    if(tables->n == 0)
    {
        printf("Es sind keine Daten vorhanden.");
        return;
    }
    char citie1[128];
    char citie2[128];

    printf("Welche Stadte moechten Sie aendern?\n");
    printf("Stadt Nr.1: ");
    scanf("%s", &citie1);
    if (citieExist(tables,&citie1)){                                                                          //wenn die Stadt existiert, dann...
        int g1=getCitieIndex(tables,&citie1);                                                                 //wird der Index in "g1" gespeichert
        printf("Stadt Nr.2: ");
        scanf("%s", &citie2);
        if (citieExist(tables,citie2)&&(strcmp(citie1,citie2)!=0)){                                           //wenn die zweite Stadt existiert, dann...
            int g2=getCitieIndex(tables,&citie2);                                                             //wird der Index in "g2" gespeichert
            printf("\nDiese Staedte sind vorhanden\n");
            printf("Die momentane Distanz zwischen den beiden Staedten ist: \n");
            int k1=g1*tables->n+g2;                                                                                   //findet den ersten Index der Distanz für die Städte heraus
            printf("%s -> %s %d\n",tables->cities[g1],tables->cities[g2], tables->distances[k1].dist);        //zeigt die Distanz zwischen den Städten an
            int k2=g2*tables->n+g1;                                                                                   //findet den zweiten Index der Distanz für die Städte heraus
            printf("%s -> %s %d\n",tables->cities[g2],tables->cities[g1], tables->distances[k2].dist);        //zeigt die Distanz zwischen den Städten an
            int nd1=-32;
            int nd2=-23;
            while(nd1<0){                                                                                     //wiederholt solange, bis die eingegebene Zahl positiv ist
                printf("Neue Distanz fuer %s -> %s: ",tables->cities[g1],tables->cities[g2]);
                scanf("%d", &nd1);
                if(nd1<0){
                    printf("Die Zahl muss positiv sein\n");
                }
            }
            while(nd2<0){                                                                                     //wiederholt solange, bis die eingegebene Zahl positiv ist
                printf("Neue Distanz fuer %s -> %s: ",tables->cities[g2],tables->cities[g1]);
                scanf("%d", &nd2);
                if(nd2<0){
                    printf("Die Zahl muss positiv sein\n");
                }
            }

            tables->distances[k1].dist=nd1;
            tables->distances[k2].dist=nd2;

        }
        else {
            printf("\nDiese Stadt ist nicht in unserer Datenbank zu finden.");
    }
    }
    else {
        printf("\nDiese Stadt ist nicht in unserer Datenbank zu finden.");
    }

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
int sumOfDistance=0;
int usedCity[tables->n];
int compareDist = 2147483647;
int compareCity;
int testtest;
for(int i=0; i<tables->n; i++)
{
    usedCity[i]=0;
}
for(int j=0; j<tables->n; j++)
{
for(int i=0; i<tables->n*tables->n; i++)
{
    if(currentCity == tables->distances[i].from)
    {
        if(currentCity != tables->distances[i].to)
        {
            if(usedCity[i%tables->n]==0)
            {
                if(tables->distances[i].to != start || j == tables->n-1)
                {
                    if(tables->distances[i].dist<compareDist)
                    {
                        testtest=tables->distances[i].dist;
                        compareDist=tables->distances[i].dist;
                        compareCity=tables->distances[i].to;
                    }
                }
            }
        }
    }
}

sumOfDistance=sumOfDistance+compareDist;
compareDist= 2147483647;
usedCity[compareCity]=1;
currentCity=compareCity;
}
printf("\n Die kuerzeste Route berechnet nach dem heuristischen Verfahren mit der\nStartstadt %s betraegt: %d \n", tables->cities[start], sumOfDistance);
}

int calcPermutations(DistanceTable* tables, int start, int permutation[])
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
    for(int i=0; i<tables->n; i++)
    {
        int j=currentCity*tables->n+travelRoute[i+1];
        sumDist=sumDist+tables->distances[j].dist;
        currentCity=travelRoute[i+1];
    }
   return sumDist;
}

int permutate(DistanceTable* tables, int start, int permutation[], int cid, int compareDist)
{
    int dist=0;
    for (int i=cid;i<tables->n-1;i++) //rekursiver Aufruf bildet alle Permutationen mit Hilfe von Backtracking
    {
        swap(&permutation[cid],&permutation[i]);
        cid++;
        if(cid == tables->n-1)
        {
            dist=calcPermutations(tables, start, permutation);
            if(dist<compareDist)
            {
                compareDist=dist;
            }
        }
        int tmp = permutate(tables, start, permutation, cid, compareDist);
        compareDist>tmp?compareDist=tmp:compareDist;
        cid--;
        swap(&permutation[cid],&permutation[i]);
    }
    return compareDist;
}


void exact(DistanceTable* tables, int start)
{
    int temp;
    int counter=0;
    int back=0;
    int cDist = 2147483647;
    int permutation[tables->n-1];
    int k=0;
    int cid=0;
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
    printf("\n");
    cDist=permutate(tables, start, permutation, cid, cDist);
    printf("\n Die kuerzeste Route berechnet nach dem exakten Verfahren mit der\nStartstadt %s betraegt: %d \n", tables->cities[start], cDist);

}

void shortestRoute(DistanceTable* tables)
{
    if(tables->n == 0)
    {
        printf("Es sind keine Daten vorhanden.");
        return;
    }
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
        switch(ans) //Abfrage aller Antwortmöglichkeiten
        {
            case 'a': ptrTables = readCities(&tables, &readFile); //Function zum laden der Datei öffnen
                      break;
            case 'b': writeCities(&tables);//Funktion zum schreiben der Daten öffnen
                      break;

            case 'c': printtable(&tables);
                      break;
            case 'd': changeDist(&tables);
                      break;
            case 'e': shortestRoute(&tables);
                      break;
            case 'f': if (checkChanges(&tables, &readFile)&& ptrTables!=NULL) //Erst schauen, ob die Daten ich geändert haben
                      {
                        char save;
                        while (save!='y'&& save!='n') //Antwortmöglichkeiten auf y oder n reduzieren und wiederholen
                        {
                            printf("Sie haben veraenderte ungespeicherte Daten, wollen Sie sie speichern? y/n:\n");
                            scanf("%s",&save);
                            if (save == 'y') //Wenn der Nutzer speichern möchte...
                            {
                                writeCities(&tables); //...Speicherfunktion aufrufen
                            }
                            else
                            {
                                deleteSpace(&tables); //..Nach beendigung Speicher wieder frei geben
                                printf("understandable have a great day\n"); //Nachricht, dass das Programm beendet wurde
                                return 0;
                            }
                        }
                      }
                      deleteSpace(&tables); //..Nach beendigung Speicher wieder frei geben
                      printf("understandable have a great day\n"); //Nachricht, dass das Programm beendet wurde
                      return 0;
                      break;
            default: printf("Bitte nur mit a,b,c,d,e oder f antworten\n"); ans=NULL; break; //Abfang, wenn ein anderer Buchstabe gesendet wurde
        }
    }
}
