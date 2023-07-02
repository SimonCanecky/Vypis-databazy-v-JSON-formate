#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

int Menu(FILE *txt){
    char riadok[MAX_LINE_LENGTH];
    int dlzka = 0;

    while (fgets(riadok, MAX_LINE_LENGTH, txt))
        dlzka++;

    return dlzka;
}

void JSON(FILE *txt, int vypis, int dlzkaMenu){
    char riadok[MAX_LINE_LENGTH];
    int dlzka, pRiadok = 0;
    char *hodnota;

    if (vypis == 0)
        fprintf(stdout, "{\n\"Restaurants\":\n[");

    while (fgets(riadok, MAX_LINE_LENGTH, txt)) {
        dlzka = strlen(riadok);
        if (dlzka == 1)
            break;

        if (riadok[dlzka-1] == '\n')
            riadok[dlzka-1] = '\0';

        if (pRiadok == 0)
            fprintf(stdout, "\n{\n\"Name\": \"%s\",", riadok);

        else if (pRiadok == 1) {
            hodnota = strtok(riadok, ",");
            hodnota = strtok(NULL, ",");
            fprintf(stdout, "\n\"Position\":\n{\n\"X\": %s,\n\"Y\": %s\n},\n\"Menu\":\n[", strtok(riadok, ","), hodnota);
        }
        else {
            hodnota = strtok(riadok, ",");
            hodnota = strtok(NULL, ",");
            if(pRiadok == dlzkaMenu-2)
                fprintf(stdout, "\n{\n\"Name\": \"%s\",\n\"Price\": %s\n}", strtok(riadok, ","), hodnota);
            else
                fprintf(stdout, "\n{\n\"Name\": \"%s\",\n\"Price\": %s\n},", strtok(riadok, ","), hodnota);
        }
        pRiadok++;
    }
    fprintf(stdout, "\n]\n}");
}

int E2(FILE *txt){
    char subor[MAX_LINE_LENGTH];
    int dlzka1, dlzka2, riadok = 0;

    while (fgets(subor, MAX_LINE_LENGTH, txt)){
        dlzka1 = strlen(subor);
        if (dlzka1 == 1)
            return 0;

        if (subor[dlzka1-1] == '\n')
            subor[dlzka1-1] = '\0';
        riadok++;

        if (riadok >= 2){
            if (strchr(subor,',') != NULL){
                dlzka1 = strlen(subor);
                dlzka2 = strlen(strchr(subor,','));

                for (int i=0; i < dlzka2-1; i++) {
                    if (!isdigit((strchr(subor, ',')+ 1)[i]))
                        return 1;
                }

                if (dlzka1 - dlzka2 == 0 || dlzka2 - 1 == 0)
                    return 1;

                if (riadok == 2) {
                    for (int i = 0; i < dlzka1 - dlzka2; i++) {
                        if (!isdigit(strtok(subor, ",")[i]))
                            return 1;
                    }
                }
            }
            else
                return 1;
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *db, *txt;
    char subor[MAX_LINE_LENGTH];
    char cesta[MAX_LINE_LENGTH];
    int dlzka, dlzkaMenu, ret, vypis = 0;

    db = fopen(argv[1], "r");
    if (db != NULL) {
        while (fgets(subor, MAX_LINE_LENGTH, db)) {
            dlzka = strlen(subor);
            if (subor[dlzka-1] == '\n')
                subor[dlzka-1] = '\0';
            strcpy(cesta,"db/");
            strcat(cesta,subor);
            txt = fopen(cesta, "r");
            if (txt == NULL)
                fprintf(stderr, "E1 %s\n", subor);

            else {
                ret = E2(txt);
                if (ret == 1) {
                    fprintf(stderr, "E2 %s\n", subor);
                    fclose(txt);
                }
                else{
                    txt = fopen(cesta, "r");
                    dlzkaMenu = Menu(txt);
                    fclose(txt);
                    if (vypis > 0)
                        fprintf(stdout, ",");

                    txt = fopen(cesta, "r");
                    JSON(txt, vypis, dlzkaMenu);
                    vypis++;
                    fclose(txt);
                }
            }
        }
        fprintf(stdout, "\n]\n}");
        fclose(db);
    }

    return 0;
}