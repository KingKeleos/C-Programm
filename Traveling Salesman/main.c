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

DistanceTable* readCities(DistanceTable* tables, char filename[]) //Funktion liest die komplette Datei, wenn sie passt
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
        for (int i=0; i<5-1; i++) //Bei jeder Stadt, bis auf der letzen..
        {
            char *citieform = tables->cities[i]; //citieform liest den Namen der Stadt aus
            strncat(citieform," ",1); //Dem Namen der Stadt ein Leerzeichen hinzuf�gen um differenzieren zu k�nnen
            fputs(citieform,fptr); //Transformierten Namen der Stadt in die Datei schreiben
        }
        fputs(tables->cities[5-1],fptr); //Letzte Stadt ohne Leerzeichen speichern
        fputs("\n",fptr); //Zeilen Umbruch
        int k=0;
        for (int i =0; i<5;i++) //5=tables->n
        {
            for (int j=0; j<5-1;j++) //5tables->n
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
            if (i<5-1) //letzten Zeilenummbruch abfangen, damit Matrix erhalten bleibt //5=tables->n
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

void printtable(DistanceTable* tables)                                                                     //gibt die Tabelle aus
{
    int citiesMax=0;
    int d[5];   //5 ist tables->n
    int f;

    for (int i=0;i<5;i++){                                                                                 //findet den l�ngsten St�dtenamen, um das Format der Tabelle anzupassen
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


    printf("| ");                                                                                          //f�gt in der Tabelle eine Trennlinie hinzu
    for (int i=0;i<5;i++)           //5 ist gleich tales->n
    {
        printf("%s | ", tables->cities[i]);

    }
    printf("\n");
    int distIndex=0;

    for (int i=0;i<5;i++){            //5 ist gleich tables->n                                             //gibt die komplette Tabelle aus
            f=citiesMax-d[i];
            printf("%s", tables->cities[i]);                                                               //als erstes wird die Stadt ausgegeben
            for (int j=0;j<f;j++){
                printf(" ");
            }
        for (int c=0;c<5;c++){          //5 ist gleich tables->n                                           //die schleife formartiert die Tabelle
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
    for (int l=0;l<5;l++){                                                                                      //5 ist gleich tables->n
        if (strcmp(tables->cities[l],citieSave)==0){                                                       //wenn eine Stadt gefunden wurde, gibt er "wahr" heraus
            return true;
        }
    }
    return false;
}
int getCitieIndex(DistanceTable* tables,char citieSave[]){                                                  //sucht nach dem Index der eingegebenen Stadt
    for (int l=0;l<5;l++){                    //5 ist gleich tables->n
        if (strcmp(tables->cities[l],citieSave)==0){
            return l;
        }
    }
    return -1;
}

void changeDist(DistanceTable* tables){                                                                       //ist f�r die �nderung der Distanzen zust�ndig
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
            int k1=g1*5+g2;                                                                                   //findet den ersten Index der Distanz f�r die St�dte heraus
            printf("%s -> %s %d\n",tables->cities[g1],tables->cities[g2], tables->distances[k1].dist);        //zeigt die Distanz zwischen den St�dten an
            int k2=g2*5+g1;                                                                                   //findet den zweiten Index der Distanz f�r die St�dte heraus
            printf("%s -> %s %d\n",tables->cities[g2],tables->cities[g1], tables->distances[k2].dist);        //zeigt die Distanz zwischen den St�dten an
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

            case 'c': printtable(&tables);
                      break;
            case 'd': changeDist(&tables);
                      break;
            case 'e': break;
            case 'f': if (checkChanges(&tables, &readFile)&& ptrTables!=NULL) //Erst schauen, ob die Daten ich ge�ndert haben
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
                      //freeSpace(&tables); //..Nach beendigung Speicher wieder frei geben
                      printf("understandable have a great day\n"); //Nachricht, dass das Programm beendet wurde
                      return 0;
                      break;
            default: printf("Bitte nur mit a,b,c,d,e oder f antworten\n"); ans=NULL; break; //Abfang, wenn ein anderer Buchstabe gesendet wurde
        }
    }
}
