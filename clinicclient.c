#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include "client_connection.c" 
#include "commands.c" 
#include "crudi.c" 
#include "paziente.h"

#define MAX_PAZIENTI 5
#define MAX_BUFFER_SIZE 1024
#define PORT 7799





int main() {
    // funzione definita in client_connection.c stampa l'errore e termina il programma o continua se la conn è stabilità
    int risultato_connessione = connessione_al_server();
    int sockets;

    if(risultato_connessione == 1 ){
        exit(EXIT_FAILURE); 
    }
    else{
        sockets = risultato_connessione;
        // Inserisci un documento di esempio
        char message[1024] = "1 Ok";
        send(sockets, message, strlen(message), 0);

        // Ricezione della conferma dal server
        char ok_msg[1024] = {0};
        recv(sockets, ok_msg, sizeof(ok_msg), 0);
        printf("Server: %s\n", ok_msg); // il messaggio dovrebbe essere OK in caso di successo
    }

    int MAX_TOKENS = 10;
    char comando[200];
    char *tokens[MAX_TOKENS];// 10 argomenti massimo per ora
    int numero_comando;
    printf("Benvenuto in Medical Dimension ^-^\n\ndigita 'cli -man' o 'cli manuale' per aprire il manuale\n");
    while(1){
        
        fgets(comando, sizeof(comando), stdin);
        
    // rimuovo il newline se presente
    comando[strcspn(comando, "\r\n")] = 0;

    size_t current_length = strlen(comando)+5;

    // Alloca spazio per la query
        char query[current_length];  
        
        //suddivo il comando in token
        char *commands = strtok(comando, " "); 
        if (strncmp(commands, "cli", 3) == 0) { //controllo primo token
            // Se sì, ottieni la parte successiva e verifica il numero corrispondente
            // char *comando_parte_2 = comando + 4; 

            // il secondo token deve essere un OPZIONE ovvero : c, r, u, d, i
            char *secondocomando = strtok(NULL, " ");
            numero_comando = commandNumber(secondocomando);// ottengo il numero corrispondente al comando salvato in commands.c
        } else {
            printf("Non esiste un comando \"%s\" \n", comando);
            numero_comando = -1;
        }
                    
        switch (numero_comando) {
            case 1:// Logica per il comando di creazione (es. cli -c oppure create)

                printManual();
                break;
            case 2: // Logica per il comando di lettura (es. clinic -r oppure read)

                printf("Eseguo la routine per il comando di creazione.\n");
                char *patientdata;
                patientdata = create_patient_data();
                printf("il dato che invierò:\"%s\"", patientdata);
                send(sockets, patientdata, strlen(patientdata),0);
                
                break;
            case 3: // Logica per il comando di lettura (es. clinic -r oppure read) // NON FUNZIONA (in realtà funziona ma pulisci e prova di piu .)

                printf("Eseguo la routine per il comando di read.\n");
                // send(sockets, 'read', strlen('read'), 0);  
                    
                    strcpy(query, "READ^");

                    // Crea il messaggio includendo il flag e la query
                    int indexxx2 = 2; 
                    current_length = strlen(query);

                    while ((commands = strtok(NULL, " ")) != NULL && (indexxx2 < MAX_TOKENS)) {

                        if((indexxx2 % 2) == 0){

                            if (commands[0] == '-') {

                                commands++;
                            }else{
                                printf("Errore: nessun campo inserito, forse volevi '-%s'?", commands);
                                free(query);
                                break; // non esce da case ma da ciclo.
                            }
                                                
                           current_length = build_read_query_2("key",commands, query, current_length);// è pari quindi è una chiave

                        }
                        else if((indexxx2 % 2) != 0){

                           current_length = build_read_query_2("value",commands, query, current_length);// è dispari quindi un valore (della chiave sopra)
                           
                        }
                        indexxx2 += 1;
                    }

                        if((indexxx2 % 2) != 0){
                                                
                            printf("not enough value for keys\n");

                            break; // non esce da case ma da ciclo.
                        }
                                                    
                    
                    printf("%s\n", query); 
                    // Invia il messaggio al server
                    send(sockets, query, strlen(query), 0);

                    // parsing della lista ottenuta, procedura di stampa lista pazienti e successiva scelta.poi
                    char raw_lista_pazienti[1024];
                    memset(raw_lista_pazienti, 0, sizeof(raw_lista_pazienti)); // Inizializzazione del buffer a zero

                    // Ricezione dei dati
                    int bytes_received = recv(sockets, raw_lista_pazienti, sizeof(raw_lista_pazienti) - 1, 0); // -1 per lasciare spazio per il terminatore null

                    if (bytes_received == -1) {
                        perror("Errore nella ricezione");
                        exit(EXIT_FAILURE);
                    } else if (bytes_received == 0) {
                        // Connessione chiusa
                        printf("Connessione chiusa dal peer\n");
                        exit(EXIT_SUCCESS);
                    } else{
                        char temp_lista_pazienti[MAX_BUFFER_SIZE];
                        char noDoc_message[MAX_BUFFER_SIZE];
                        strcpy(temp_lista_pazienti, raw_lista_pazienti);// variabile temporanea da manipolare senza toccare la lista originale
                        remove_whitespace(noDoc_message, temp_lista_pazienti);
                        printf("\nmsg:%s:",noDoc_message);
                        if(strcmp(noDoc_message,"noDoc") == 0 || strcmp(noDoc_message,"noDocOK") == 0){
                            printf("oo%s%s%soo",noDoc_message,raw_lista_pazienti,temp_lista_pazienti);
                            printf("\nnessun paziente trovato, Riprova\n");
                            send(sockets, "closeThread", strlen("closeThread"), 0);
                            break;
                        } else{

                            printf("\nininininininininininininin\n");
                            // Aggiunta del terminatore null
                            raw_lista_pazienti[bytes_received] = '\0';

                            // Stampa dei dati ricevuti
                            char *pazienti[MAX_PAZIENTI]; // Array di puntatori per memorizzare i dati dei pazienti
                            char Oid[MAX_PAZIENTI][256]; // Array per memorizzare gli Oid dei pazienti
                            int num_pazienti = 0;

                            // Parsing dei dati con strtok
                            char *paziente = strtok(raw_lista_pazienti, "^");
                            while (paziente != NULL && num_pazienti < MAX_PAZIENTI) {
                                pazienti[num_pazienti] = paziente;
                                
                                // Copia dell'Oid in Oid[num_pazienti]
                                strcpy(Oid[num_pazienti], paziente);
                                
                                num_pazienti++;
                                paziente = strtok(NULL, "^");
                            }

                            // Stampa dei dati dei pazienti
                            printf("\n\tNome\t\tCognome\t\tCodice_Fiscale\n");
                            printf("--------------------------------------------------------\n");
                            for (int i = 0; i < num_pazienti; i++) {
                                char *campo = strtok(pazienti[i], "#");
                                if(campo != NULL){
                                    printf("%d\t", i+1); // Numerazione dei pazienti
                                }
                                
                                // Scorrimento degli altri campi
                                    campo = strtok(NULL, "#");// il primo non lo stampo (oid)
                                while (campo != NULL) {
                                    printf("%s\t\t", campo);
                                    campo = strtok(NULL, "#");
                                }
                                printf("\n");
                            }

                            // scelta del paziente tra quelli trovati
                            int scelta = 0;
                            char view_request[100] = ""; // dichiarazione di una stringa sufficientemente grande
                            printf("\nInserisci il numero del paziente: ");
                            while (scelta > num_pazienti || scelta <= 0) {
                                scanf("%d", &scelta);
                                getchar(); // Pulisci il buffer di input
                                if (scelta > num_pazienti || scelta <= 0) {
                                    printf("\nInput non valido, inserisci un numero del paziente valido: ");
                                    fflush(stdin); // pulisci il buffer di input
                                }
                            } //OPERAZIONE UPDATE UGUALE FINO A QUI.

                            //aggiungo l object id del pazinete scelto al messaggio da inviare
                            char tipo_richiesta[50] = ""; // Dichiarazione di una stringa sufficientemente grande per il tipo di richiesta
                            char *Oid_scelto = strtok(Oid[scelta - 1], "#");
                            strcpy(view_request, Oid_scelto);
                            strcat(view_request, "#");

                            // Scelta della sezione da visualizzare
                            int scelta2 = 0;
                            printf("\n\nQuale sezione vuoi visualizzare?\n");
                            printf("------------------------------------\n");
                            printf("1\tAnagrafica\n");
                            printf("2\tCartella Clinica\n");
                            printf("\nInserisci il numero della sezione scelta: ");

                            // Costruzione della richiesta da inviare al server
                            while (scelta2 > 2 || scelta2 <= 0) {
                                scanf("%d", &scelta2);
                                getchar(); // Pulisci il buffer di input
                                if (scelta2 > 2 || scelta2 <= 0) {
                                    printf("\nInput non valido, inserisci un numero del paziente valido: ");
                                    fflush(stdin); // pulisci il buffer di input
                                }
                            } 

                            if(scelta2 == 1){
                                strcat(view_request, "dati_anagrafici");
                                printf("view_request%s", view_request);
                                if(send(sockets, view_request, strlen(view_request), 0) == -1){
                                    printf("Errore nell'invio della richiesta al server\n");
                                    break;
                                }
                            }else if(scelta2 == 2){
                                strcat(view_request, "cartella_clinica");
                                printf("view_request%s", view_request);
                                if(send(sockets, view_request, strlen(view_request), 0) == -1){
                                    printf("Errore nell'invio della richiesta al server\n");
                                    break;
                                }
                            }


                        }

                    }
                     
                    
    // Invio della richiesta al server

                break;
            case 4: // Logica per il comando di update (es. clinic -u oppure update)

                printf("Eseguo la routine per il comando di update.\n");
/*
               // char *patientdata_to_update;
                //patientdata_to_update = update_patient_data();
                //printf("il dato che invierò:\"%s\"", patientdata_to_update);
                //send(sockets, patientdata_to_update, strlen(patientdata_to_update),0);

                    strcpy(query, "UPDATE^");

                    // Crea il messaggio includendo il flag e la query
                    int indexxx = 2; 
                    current_length = strlen(query);

                    while ((commands = strtok(NULL, " ")) != NULL && (indexxx < MAX_TOKENS)) {

                        if((indexxx % 2) == 0){

                            if (commands[0] == '-') {

                                commands++;
                            }else{
                                printf("Errore: nessun campo inserito, forse volevi '-%s'?", commands);
                                free(query);
                                break; // non esce da case ma da ciclo.
                            }
                                                
                           current_length = build_read_query_2("key",commands, query, current_length);// è pari quindi è una chiave

                        }
                        else if((indexxx % 2) != 0){

                           current_length = build_read_query_2("value",commands, query, current_length);// è dispari quindi un valore (della chiave sopra)
                           
                        }
                        indexxx += 1;
                    }

                        if((indexxx % 2) != 0){
                                                
                            printf("not enough value for keys\n");

                            break; // non esce da case ma da ciclo.
                        }
                                                    
                    
                    printf("%s\n", query); 
                    // Invia il messaggio al server
                    send(sockets, query, strlen(query), 0);

                    // parsing della lista ottenuta, procedura di stampa lista pazienti e successiva scelta.poi
                    char raw_lista_pazienti2[1024];
                    memset(raw_lista_pazienti2, 0, sizeof(raw_lista_pazienti2)); // Inizializzazione del buffer a zero

                    // Ricezione dei dati
                    int bytes_received2 = recv(sockets, raw_lista_pazienti2, sizeof(raw_lista_pazienti2) - 1, 0); // -1 per lasciare spazio per il terminatore null
                    if (bytes_received2 == -1) {
                        perror("Errore nella ricezione");
                        exit(EXIT_FAILURE);
                    } else if (bytes_received2 == 0) {
                        // Connessione chiusa
                        printf("Connessione chiusa dal peer\n");
                        exit(EXIT_SUCCESS);
                    }

                    // Aggiunta del terminatore null
                    raw_lista_pazienti2[bytes_received2] = '\0';

                    // Stampa dei dati ricevuti
                        char *pazienti2[MAX_PAZIENTI]; // Array di puntatori per memorizzare i dati dei pazienti
                        char Oid2[MAX_PAZIENTI][256]; // Array per memorizzare gli Oid2 dei pazienti
                        int num_pazienti2 = 0;

                        // Parsing dei dati con strtok
                        char *paziente2 = strtok(raw_lista_pazienti2, "^");
                        while (paziente2 != NULL && num_pazienti2 < MAX_PAZIENTI) {
                            pazienti2[num_pazienti2] = paziente2;
                            
                            // Copia dell'Oid2 in Oid2[num_pazienti2]
                            strcpy(Oid2[num_pazienti2], paziente2);
                            
                            num_pazienti2++;
                            paziente2 = strtok(NULL, "^");
                        }

                        // Stampa dei dati dei pazienti
                        printf("\n\tNome\t\tCognome\t\tCodice_Fiscale\n");
                        printf("--------------------------------------------------------\n");
                        for (int i = 0; i < num_pazienti2; i++) {
                            char *campo = strtok(pazienti2[i], "#");
                            if(campo != NULL){
                                printf("%d\t", i+1); // Numerazione dei pazienti
                            }
                            
                            // Scorrimento degli altri campi
                                campo = strtok(NULL, "#");// il primo non lo stampo (oid2)
                            while (campo != NULL) {
                                printf("%s\t\t", campo);
                                campo = strtok(NULL, "#");
                            }
                            printf("\n");
                        }

                        // scelta del paziente2 tra quelli trovati
                        int scelta2 = 0;
                        char view_request2[50] = ""; // dichiarazione di una stringa sufficientemente grande
                        printf("\nInserisci il numero del paziente2: ");
                        while (scelta2 > num_pazienti2 || scelta2 <= 0) {
                            scanf("%d", &scelta);
                            if (scelta2 > num_pazienti2 || scelta2 <= 0) {
                                printf("\nInput non valido, inserisci un numero del paziente2 valido: ");
                                fflush(stdin); // pulisci il buffer di input
                            }
                        } //OPERAZIONE UPDATE UGUALE FINO A QUI.

                        //aggiungo l object id del pazinete scelto al messaggio da inviare
                        char *Oid2_scelto = strtok(Oid2[scelta2 - 1], "#");
                        strcpy(view_request2, Oid2_scelto);
                            printf("view_request2: %s\n", view_request2);
                            fflush(stdin);
                        strcat(view_request2, "#");
                        printf("view_request2: %s\n", view_request2);
                        fflush(stdin);
                        // scelta2 della sezione da visualizzare
                        scelta2 = 0;
                        printf("\n\nQuale sezione vuoi visualizzare?\n");
                        printf("------------------------------------\n");
                        printf("1\tAnagrafica\n");
                        printf("2\tCartella Clinica\n");
                        printf("\nInserisci il numero della sezione scelta: ");
                            printf("view_request2: %s\n", view_request2);
                            fflush(stdin);
                        while (1) {
                            if (scanf("%d", &scelta) == 1 && (scelta2 == 1 || scelta2 == 2)) {
                                break;
                            }
                            printf("\nInput non valido, inserisci un numero della sezione valido: ");
                            while (getchar() != '\n'); // Pulizia del buffer di input
                        }
                        printf("view_request2: %s\n", view_request2);
                        fflush(stdin);
                        // Costruzione della richiesta da inviare al server
                        switch (scelta) {
                            case 1:
                                strcat(view_request2, "anagrafica");
                                break;
                                    if (send(sockets, view_request2, strlen(view_request2), 0) == -1) {
                            exit(EXIT_FAILURE);
                        }
                            case 2:
                                strcat(view_request2, "cartella_clinica");
                                break;
                                    if (send(sockets, view_request2, strlen(view_request2), 0) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        }


*/
                break;

            case 5: // Logica per il comando di eliminazione (es. clinic -d oppure delete)

                printf("Eseguo la routine per il comando di eliminazione.\n");
                break;
            case 6: // custom comando per fare prove : -p o prova
                printf("Eseguo la routine per il comando di prova custom.\n");
                char query[] = "PROVA {\"paziente2.dati_anagrafici.nome\": \"Mario\"}"; 
                size_t query_length = strlen(query);
                printf("query%s",query);
                // Invia la lunghezza della query
                send(sockets, &query_length, sizeof(size_t), 0);

                // Invia la query stessa
                send(sockets, query, query_length, 0);
                break;
            default:
                printf("Comando non valido. Riprova.\n");
                break;
            case -1:
                printf("Comando non valido, riprova.\n");
                break;
        }

    }

/*

    // Inserisci un documento di esempio
    char message[4096];

    // Simulazione dei dati
    int timestamp_inizio1 = 1641819000;
    int timestamp_fine1 = 1645365000;
    int timestamp_evento1 = 1642241400;
    int timestamp_evento2 = 1642636200;
    int timestamp_medicazione1 = 1643124600;

    // Utilizzo di snprintf per formattare la stringa JSON
    snprintf(message, sizeof(message),
        "{"
        "   \"paziente\": {"
        "       \"dati_anagrafici\": {"
        "           \"nome\": \"Mario\","
        "           \"cognome\": \"Rossi\","
        "           \"data_di_nascita\": \"1980-05-15\","
        "           \"sesso\": \"Maschio\","
        "           \"codice_fiscale\": \"RSSMRA80M15H501A\""
        "       },"
        "       \"dati_medici\": {"
        "           \"allergie\": ["
        "               {"
        "                   \"sostanza\": \"Penicillina\","
        "                   \"rea\": \"Sensibilità allergica nota con rash cutaneo\""
        "               },"
        "               {"
        "                   \"sostanza\": \"Aspirina\","
        "                   \"rea\": \"Intolleranza gastrica\""
        "               }"
        "           ],"
        "           \"gruppo_sanguigno\": \"A+\","
        "           \"farmaci_in_uso\": ["
        "               {"
        "                   \"nome\": \"Losartan\","
        "                   \"dosaggio\": \"50 mg\","
        "                   \"frequenza\": \"una volta al giorno\""
        "               },"
        "               {"
        "                   \"nome\": \"Metformina\","
        "                   \"dosaggio\": \"1000 mg\","
        "                   \"frequenza\": \"due volte al giorno\""
        "               }"
        "           ]"
        "       },"
        "       \"ricoveri\": ["
        "           {"
        "               \"cartella_clinica\": {"
        "                   \"numero_cartella\": 1,"
        "                   \"periodo_di_ricovero\": {"
        "                       \"timestamp_inizio\": %i,"
        "                       \"timestamp_fine\": %i"
        "                   },"
        "                   \"eventi_clinici\": ["
        "                       {"
        "                           \"tipo\": \"somministrazione\","
        "                           \"descrizione\": \"Somministrazione di farmaco X\","
        "                           \"timestamp\": %i"
        "                       },"
        "                       {"
        "                           \"tipo\": \"evento\","
        "                           \"descrizione\": \"Evento durante il ricovero\","
        "                           \"timestamp\": %i"
        "                       },"
        "                       {"
        "                           \"tipo\": \"medicazione\","
        "                           \"descrizione\": \"Medicazione per ferita\","
        "                           \"timestamp\": %i"
        "                       }"
        "                   ]"
        "               }"
        "           },"
        "           {"
        "               \"cartella_clinica\": {"
        "                   \"numero_cartella\": 2,"
        "                   \"periodo_di_ricovero\": {"
        "                       \"timestamp_inizio\": %i,"
        "                       \"timestamp_fine\": %i"
        "                   },"
        "                   \"eventi_clinici\": ["
        "                       {"
        "                           \"tipo\": \"somministrazione\","
        "                           \"descrizione\": \"Somministrazione di farmaco Y\","
        "                           \"timestamp\": %i"
        "                       },"
        "                       {"
        "                           \"tipo\": \"evento\","
        "                           \"descrizione\": \"Nuovo evento\","
        "                           \"timestamp\": %i"
        "                       }"
        "                   ]"
        "               }"
        "           }"
        "       ]"
        "   }"
        "}",
        timestamp_inizio1,
        timestamp_fine1,
        timestamp_evento1,
        timestamp_evento2,
        timestamp_medicazione1,
        timestamp_inizio1+36000, // Esempio per il secondo ricovero
        timestamp_fine1+36000,
        timestamp_evento1+36000,
        timestamp_evento2+36000   // Esempio per il secondo ricovero
    );




        // Invia la stringa JSON
        send(sockets, message, strlen(message), 0);

        // Ricezione della conferma dal server
        char buffer[1024] = {0};
        recv(sockets, buffer, 1024, 0);
        printf("Server: %s\n", buffer);// il messaggio dovrebbe essere OK in caso di successo
    */

        // libera risorse 
        // Chiusura delle connessioni

        close(sockets);

    return 0;
}
