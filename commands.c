#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funzione di parsing per array di array in C
int parseArrayAndGetIndex(char *commands[][2], int numCommands, char *commandToFind) {
    for (int i = 0; i < numCommands; ++i) {
        if (strcmp(commands[i][0], commandToFind) == 0 || strcmp(commands[i][1], commandToFind) == 0) {
            
            return i+1;  // Restituisce l'indice dell'array in cui il comando è stato trovato
        }
    }

    return -1;  // Nessuna corrispondenza trovata
}

const char *cli_manuale = 
    "    NAME\n"
    "        cli - Gestore della collezione \"patient\"\n\n"
    "    SYNOPSIS\n"
    "        cli [OPZIONE] [ARGOMENTO]\n\n"
    "    DESCRIPTION\n"
    "        Il comando cli è un'utilità per gestire la collezione \"patient\" che contiene informazioni sui pazienti,\n"
    "        inclusi dati anagrafici, dati medici e cartelle cliniche dei ricoveri.\n\n"
    "    OPZIONI\n"
    "        -man, manuale\n"
    "            Stampa il manuale del comando cli.\n\n"
    "        -c, create\n"
    "            Fa partire la routine per la creazione di un nuovo documento \"patient\" o una nuova cartella clinica.\n"
    "            Esempio: cli -c\n\n"
    "        -r, read [ID]\n"
    "            Recupera le informazioni di un paziente o di una cartella clinica specificando da uno a tre paramentri dei dati anagrafici del paziente,\n"
    " le chiavi devono essere precedute da \"-\" e l'argomento successivo sarà il valore corrispondente.\n"
    "            Esempio: cli -r -nome Carlo -cognome Magno\n\n"
    "    NOTA\n"
    "        Le operazioni di delete e insert sono state rimosse da questo sistema di comandi, perchè successivamente integrato alla routine\n"
    "        di READ, per rendere le due operazioni più user friendly, visualizzando sul momento i dati da manipolare\n";


void printManual(){
    printf("%s\n", cli_manuale);
}


int commandNumber(char *commandToFind) {
    // all commands
    char *commands[][2] = {
        {"-man", "manuale"},
        {"-c", "create"},
        {"-r", "read"},
        {"-u", "update"},
        {"-d", "delete"},
        {"-p", "prova"}
    };

    int numCommands = sizeof(commands) / sizeof(commands[0]);

    int commandIndex = parseArrayAndGetIndex(commands, numCommands, commandToFind);

    return commandIndex;
}
