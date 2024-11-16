#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

void zoradenie(char arr[], int n) {
    int zamena;
    do {
        zamena = 0;
        for (int i = 1; i < n; i++) {
            if (arr[i - 1] > arr[i]) {
                // Prohoď písmena, pokud nejsou ve správném pořadí
                char temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
                zamena = 1;
            }
        }
    } while (zamena);
}

int main(int argc, char *argv[]) {
    char adresy[42][100]; //pole pre nacitanie adries
    int adresyPocet = 0; //pocet nacitanych adries

    //nacitanie a spracovanie zoznamu adries zo stdin
    while (adresyPocet < 42 && fgets(adresy[adresyPocet],100,stdin) != NULL) {
    int dlzka = strlen(adresy[adresyPocet]);
    if (dlzka > 0 && adresy[adresyPocet][dlzka-1] == '\n') { //ak je na posledny znak v adrese "\n", tak sa odstrani
        adresy[adresyPocet][dlzka-1] = '\0';
    }
    adresyPocet++;
    }

    //ak boli zadane viac ako 2 argumenty, tak sa vypise chybove hlasenie
    if (argc > 2){
        printf("Nespravny pocet argumentov. Spravny Syntax: ./keyfilter PREFIX <adresy.txt\nPri PREFIXe obsahujucom medzeru/y pouzite: ./keyfilter \"PREFIX\" <adresy.txt\n");
        return 1;
    }

    //ak nebol zadany prefix, alebo prefix = prazdny retazec, vypisu sa povolene pismena, pripadne adresa
    if (argc == 1 || argc == 1 || (argc == 2 && strcasecmp(argv[1], "") == 0)){
        if (adresyPocet == 0) { //ak nebol zadany prefix a vstup je prazdny (neosahuje ziadne adresy), vystup bude "Not found"
            printf("Not found\n");
        } else if (adresyPocet == 1) { //ak nebol zadany prefix a na vstupe je iba jedna adresa, adresa sa priamo vypise
            for (int i = 0; adresy[0][i]; i++) {
                adresy[0][i] = toupper(adresy[0][i]); //adresa sa prevedie na velke pismena
            }
            printf("Found: %s\n", adresy[0]);
        } else { //ak nebol zadany prefix a na vstupe su aspon 2 adresy, vypisu sa povolene pismena
            printf("Enable: ");
            char vypisanePismena[127] = {0}; //do tohto pola sa budu zapisovat uz vypisane pismena, vynuluje sa kvoli zamedzeniu duplicity
            for (int i = 0; i < adresyPocet; i++) {
                if (strlen(adresy[i]) > 0){ //podmienka pre ignorovanie prazdnych riadkov
                    char pismeno = adresy[i][0]; //vyber prveho pismena adresy z pola adres
                    if (pismeno >= 'a' && pismeno <= 'z') {
                        pismeno = pismeno - 32; //prevod malych pismen na velke (rozdiel ich hodnot v ASCII tabulke je 32)
                    }
                    //podmienka na zamedzenie duplicity
                    int index = pismeno; //kazde pismeno bude mat unikatny index
                    if (vypisanePismena[index] == 0) { //kontrola, ci pismeno na pozicii index zatial nebolo vypisane
                        vypisanePismena[index] = pismeno; //na poziciu "index" v poli vypisanePismena sa nastavi dane pismeno a tak sa uz nebude viackrat vypisovat
                    }
                }
            }
            //zoradenie a nasledne vypisanie pola vypisanePismena
            zoradenie(vypisanePismena, 127);
            for (int i = 0; i < 127; i++) {
            if (vypisanePismena[i] != 0) {
                printf("%c", vypisanePismena[i]);}
            }
            printf("\n");
        }
        return 0;
    }

    const char *prefix = argv[1]; //zadefinovanie prveho argumentu z terminalu ako prefix
    int zhodaPocet = 0; //pocet najdenych zhod

    //pri zadani prefixu dlhsieho ako 100 znakov sa vypise chybove hlasenie.
    if (strlen(prefix) > 100){
        printf("Maximalna dlzka prefixu je 100 znakov\n");
        return 1;
    }

    //ak boli zadane 2 argumenty, prefix sa porovna so vsetkymi adresami
    if (argc ==2){
    for (int i = 0; i < adresyPocet; i++) {
        if (strncasecmp(adresy[i], prefix, strlen(prefix)) == 0) { //porovnanie prefixu s kazdou adresou (bez ohladu na velkost pismen)
            zhodaPocet++;
        }
    }
    //ak bola najdena prave jedna zhoda, vypise sa iba vyhovujuca adresa
    if (zhodaPocet == 1) {
        for (int i = 0; i < adresyPocet; i++) {
            if (strncasecmp(adresy[i], prefix, strlen(prefix)) == 0) {
                for (int j = 0; adresy[i][j]; j++) {
                    adresy[i][j] = toupper(adresy[i][j]);
                }
                printf("Found: %s\n", adresy[i]);
                return 0;
            }
        }

    //ak bola najdena viac ako jedna zhoda, vypisu sa vyhovujuce povolene pismena     
    } else if (zhodaPocet > 1) {
        printf("Enable: ");
        char vypisanePismena[127] = {0}; //do tohto pola sa budu zapisovat uz vypisane pismena, vynuluje sa kvoli zamedzeniu duplicity
        for (int i = 0; i < adresyPocet; i++) {
            if (strncasecmp(adresy[i], prefix, strlen(prefix)) == 0) { //porovnanie prefixu s kazdou adresou (bez ohladu na velkost pismen)
                char pismeno = adresy[i][strlen(prefix)]; //vyber pismena z pola adries
                if (pismeno >= 'a' && pismeno <= 'z') {
                    pismeno = pismeno - 32; //prevod malych pismen na velke (rozdiel ich hodnot v ASCII tabulke je 32)
                }
                //podmienka na zamedzenie duplicity
                int index = pismeno; //kazde pismeno bude mat unikatny index
                if (vypisanePismena[index] == 0) { //kontrola, ci pismeno na pozicii index zatial nebolo vypisane
                    vypisanePismena[index] = pismeno; //na poziciu "index" v poli vypisanePismena sa nastavi dane pismeno a tak sa uz nebude viackrat vypisovat
                }
            }
        }
        //zoradenie a nasledne vypisanie pola vypisanePismena
        zoradenie(vypisanePismena, 127);
            for (int i = 0; i < 127; i++) {
            if (vypisanePismena[i] != 0) {
                printf("%c", vypisanePismena[i]);}
            }
            printf("\n");
    return 0;
    //ak nebola najdena ziadna zhoda, vystup bude "Not Found"
    } else {
        printf("Not found\n");
        return 0;
    }
    }
    return 0;
}