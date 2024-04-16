
#include <mongoc/mongoc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paziente.h"

#define MAX_BUFFER_SIZE 1024

// Funzione per la connessione al database
mongoc_client_t *connect_to_mongodb() {
    

    mongoc_init();
    mongoc_client_t *client = mongoc_client_new("mongodb://localhost:27017"); // socket
    return client;
}

void remove_whitespace(char *dst, const char *src) {
    while (*src) {
        if (!isspace((unsigned char)*src)) { // Se il carattere non è uno spazio bianco o un carattere di controllo
            *dst++ = *src; // Copia il carattere nella nuova posizione e incrementa il puntatore
        }
        src++; // Passa al prossimo carattere nella stringa originale
    }
    *dst = '\0'; // Termina la stringa
}


// Funzione per stampare i dati del paziente
void stampa_dati_paziente(const char *nome, const char *cognome, char sesso, const char *residenza, const char *data_di_nascita, const char *codice_fiscale) {
    printf("\nDati del Paziente:\n");
    printf("-------------------\n");
    printf("Nome: %s\n", nome);
    printf("Cognome: %s\n", cognome);
    printf("Sesso: %c\n", sesso);
    printf("Residenza: %s\n", residenza);
    printf("Data di Nascita: %s\n", data_di_nascita);
    printf("Codice Fiscale: %s\n", codice_fiscale);
    printf("-------------------\n");
}

// Funzione per controllare se una stringa è una data nel formato dd-mm-yy
bool controlla_data(int g, int m, int a, char *data_di_nascita) {
    int giorno = g;
    int mese = m;
    int anno = a;

    if (giorno < 1 || giorno > 31 || mese < 1 || mese > 12 || anno < 1900 || anno > 9999) {
        return false;
    }

    // Utilizza un'altra variabile per memorizzare la data di nascita
    char data_buffer[11]; // "dd-mm-yyyy\0" + 1 per il terminatore null
    sprintf(data_buffer, "%02d-%02d-%04d", giorno, mese, anno);

    // Copia la stringa formattata nel parametro data_di_nascita
    strcpy(data_di_nascita, data_buffer);

    return true;
}


// Funzione per controllare se una stringa rappresenta il sesso 'M' o 'F'
bool controlla_sesso(char sesso) {
    return (sesso == 'M' || sesso == 'F');
}

// Funzione per controllare se una stringa rappresenta un codice fiscale di 16 cifre
bool controlla_codice_fiscale(const char *codice_fiscale) {
    return (strlen(codice_fiscale) == 16);
}

// Funzione per chiedere conferma all'utente
bool chiedi_conferma(const char *messaggio) {
    char risposta[MAX_BUFFER_SIZE];
    while (1) {
        printf("%s (y/n): ", messaggio);
        fgets(risposta, sizeof(risposta), stdin);

        // Rimuovi il carattere di nuova riga
        risposta[strcspn(risposta, "\n")] = '\0';

        if (strncmp(risposta, "y", 2) == 0) {
            return true;
        } else if (strncmp(risposta, "n", 2) == 0) {
            return false;
        }
        printf("Input non valido. Rispondi 'y' o 'n'.\n");
    }
}

bool chiedi_conferma_stringa(char *messaggio) {
    char conferma[2]; // Usiamo un array di 2 per contenere la risposta (s/n)

    // Chiedi all'utente di confermare la stringa inserita
    do {
        printf("Vuoi inserire \"%s\" (y/n): ", messaggio);
        scanf("%1s", conferma);
        getchar(); // Pulisci il buffer di input

        // Converti la prima lettera in maiuscolo
        messaggio[0] = toupper(messaggio[0]);
    } while (strcmp(conferma, "y") != 0 && strcmp(conferma, "n") != 0);

    // Restituisci true se la risposta è "s", altrimenti restituisci false
    return conferma[0] == 'y';
}




char *update_patient_data() {

}
// Funziynper creare un array nel formato chiave#valore#chiave#valore#...
char *create_patient_data() {
    // Variabili per i dati inseriti dall'utente
    char nome[50];
    char cognome[50];
    char data_di_nascita[11];
    char sesso;
    char codice_fiscale[17];
    char residenza[50];

    int giorno;
    int mese;
    int anno;

    // Input dei dati dall'utente
    do{
        do {
            printf("Inserisci il nome: ");
            scanf("%s", nome);
            getchar();
        } while (!chiedi_conferma_stringa(nome));
printf("\n");
        do {
            printf("Inserisci il cognome: ");
            scanf("%s", cognome);
            getchar();
        } while (!chiedi_conferma_stringa(cognome));
printf("\n");
        do {
            printf("Inserisci il sesso (M/F): ");
            scanf(" %c", &sesso);
            getchar(); // Pulisci il buffer di input
            // Converti il carattere inserito in maiuscolo per confrontarlo con 'M' o 'F'
            sesso = toupper(sesso);
            // Controlla se il valore inserito è valido ('M' o 'F')
        } while (!controlla_sesso(sesso) || !chiedi_conferma("Confermi il sesso inserito?"));
printf("\n");
        do {
            printf("Inserisci residenza: ");
            scanf("%s", residenza);
            getchar();
        } while (!chiedi_conferma_stringa(residenza));
printf("\n");
        do {
            printf("Inserisci la data di nascita. \nGiorno(DD): ");
            scanf("%d", &giorno);
            getchar();
            printf(" Mese(MM): ");
            scanf("%d", &mese);
            getchar();
            printf(" Anno(YYYY): ");
            scanf("%d", &anno);
            getchar();
        } while (!controlla_data(giorno, mese, anno, data_di_nascita) || !chiedi_conferma("Confermi la data di nascita inserita?"));
printf("\n");
        do {
            printf("Inserisci il codice fiscale (16 cifre): ");
            scanf("%16s", codice_fiscale);
            getchar(); // Pulisci il buffer di input
        } while (!controlla_codice_fiscale(codice_fiscale) || !chiedi_conferma("Confermi il codice fiscale inserito?"));
printf("\n");
        // Allocazione di memoria per il buffer di output
        char *output_buffer = malloc(MAX_BUFFER_SIZE);
        if (output_buffer == NULL) {
            perror("Errore nell'allocazione di memoria");
            exit(EXIT_FAILURE);
        }

        stampa_dati_paziente(nome, cognome, sesso, residenza, data_di_nascita, codice_fiscale);

    } while(!chiedi_conferma("Confermi di voler inserire questi dati?"));
printf("\n");

        // Allocazione di memoria per il buffer di output
    char *output_buffer = malloc(MAX_BUFFER_SIZE);
    if (output_buffer == NULL) {
        perror("Errore nell'allocazione di memoria");
        exit(EXIT_FAILURE);
    }

    // Costruzione dell'array nel formato chiave#valore#chiave#valore#...
    snprintf(output_buffer, MAX_BUFFER_SIZE, "CREATE^nome#%s#cognome#%s#data_di_nascita#%s#sesso#%c#codice_fiscale#%s#residenza#%s",
             nome, cognome, data_di_nascita, sesso, codice_fiscale, residenza);

    return output_buffer;
}

void insert_log_document(mongoc_client_t *client, const char *db_name, const char *collection_name, const char *operation, const char *codice_fiscale) {
    // Crea un nuovo documento BSON per il log
    bson_t *log_doc = bson_new();

    // Aggiungi i campi al documento del log
    bson_oid_t log_id;
    bson_oid_init(&log_id, NULL);
    BSON_APPEND_OID(log_doc, "_id", &log_id); // ID univoco per il log
    BSON_APPEND_UTF8(log_doc, "operazione", operation);

    // Aggiungi il timestamp corrente al documento del log
    time_t current_time = time(NULL);
    BSON_APPEND_TIME_T(log_doc, "timestamp", current_time);

    // Aggiungi il codice fiscale come campo "patient_code"
    BSON_APPEND_UTF8(log_doc, "patient_code", codice_fiscale);

    // Ottieni la collezione "log" dal database
    mongoc_database_t *database = mongoc_client_get_database(client, db_name);
    mongoc_collection_t *collection = mongoc_database_get_collection(database, collection_name);

    // Inserisci il documento del log nella collezione "log"
    if (!mongoc_collection_insert_one(collection, log_doc, NULL, NULL, NULL)) {
        fprintf(stderr, "Failed to insert log document\n");
    }

    // Rilascia le risorse
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);

    // Rilascia la memoria allocata per il documento del log
    bson_destroy(log_doc);
}


// Funzione per l'inserimento di un documento
void crud_create_document(mongoc_client_t *client, const char *db_name, const char *collection_name, char *buffer, size_t bytes_received) {
    if (bytes_received <= 0) {
        // Gestisci l'errore o la connessione chiusa
        fprintf(stderr, "Error receiving data from client\n");
    } else {
    
    int max_tokens = 15;
    char *tokens_key_or_value[max_tokens];  // Array per salvare i token
    int num_tokens = 0;  // Contatore per il numero di token
    char *token = strtok(buffer, "#");  // Estrai il primo token

    while (token != NULL && num_tokens < max_tokens) {
        tokens_key_or_value[num_tokens++] = token;  // Salva il token nell'array
        token = strtok(NULL, "#");  // Estrai il token successivo
    }

    // Crea la query BSON
    bson_t *inner_doc = bson_new();
    bson_t *outer_doc = bson_new();
    char *codice_fiscale = NULL;
    for (int i = 0; i < num_tokens; i += 2) {
        if (i + 1 < num_tokens) {
            if(strcmp(tokens_key_or_value[i], "codice_fiscale") == 0){
                codice_fiscale = strdup(tokens_key_or_value[i + 1]);
                printf("codice_fiscale: %s", codice_fiscale);
            }
            bson_append_utf8(inner_doc, tokens_key_or_value[i], -1, tokens_key_or_value[i + 1], -1); //faccio append nella inner_doc che sto costruendo , la chiave e il suo valore (token[i] e token[i+1])
        } else {
            printf("Errore: manca il valore per la chiave %s\n", tokens_key_or_value[i]);
            bson_destroy(inner_doc);
            return;
        }
    }

    bson_append_document(outer_doc, "dati_anagrafici", -1, inner_doc); // Aggiungi il documento interno come campo "dati_anagrafici"

    char *query_str = bson_as_canonical_extended_json(outer_doc, NULL);
    printf("outer_doc: %s\n", query_str);

        // Ottieni la collezione dal database
        mongoc_database_t *database = mongoc_client_get_database(client, db_name);
        mongoc_collection_t *collection = mongoc_database_get_collection(database, collection_name);

        // Inserisci il documento nella collezione
        if (!mongoc_collection_insert_one(collection, outer_doc, NULL, NULL, NULL)) {
            fprintf(stderr, "Failed to insert new patient\n");
        } else {
            insert_log_document(client, db_name, "log", "CREATE", codice_fiscale);
            printf("New patient inserted successfully.\n");
        }

        // Rilascia le risorse
        mongoc_collection_destroy(collection);
        mongoc_database_destroy(database);

        // Dealloca la memoria per il codice fiscale
        free(codice_fiscale);
    }
}


void crud_read_document(int client_socket, mongoc_client_t *client, const char *db_name, const char *collection_name, char *arguments, int arguments_length) {
    bson_error_t erroraccio;
    printf("(db_name: %s)(collection_name: %s)(keyvalues: %s)\n", db_name, collection_name, arguments);  //temp
    fflush(stdout); 

    if(arguments_length <= 0){
        // Gestisci l'errore o la connessione chiusa
        fprintf(stderr, "Error receiving data from client\n");
    }else{

        // Parse degli argomenti
        char *tokens[9];  // Array per salvare i token
        int num_tokens = 0;  // Contatore per il numero di token
        char *token = strtok(arguments, "#");  // Estrai il primo token

        while (token != NULL && num_tokens < 9) {
            tokens[num_tokens++] = token;  // Salva il token nell'array
            token = strtok(NULL, "#");  // Estrai il token successivo
        }

        // Crea la query BSON
        bson_t *raw_query = bson_new();
        for (int i = 0; i < num_tokens; i += 2) {
            char chiave[strlen(tokens[i]) + strlen("dati_anagrafici.") + 1];
            snprintf(chiave, sizeof(chiave), "dati_anagrafici.%s", tokens[i]);

            if (i + 1 < num_tokens) {
                bson_append_utf8(raw_query, chiave, -1, tokens[i + 1], -1);
            } else {
                printf("Errore: manca il valore per la chiave %s\n", tokens[i]);
                bson_destroy(raw_query);
                return;
            }
        }

        

        mongoc_collection_t *collection = mongoc_client_get_collection(client, db_name, collection_name);
        mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, raw_query, NULL, NULL);
        const bson_t *result;
        

        // Array di dati anagrafici dei pazienti


        bool found_documents = false;
        char *ListaPazienti = malloc(1000);  // Alloca inizialmente 1000 byte per la stringa
        ListaPazienti[0] = '\0';  // Inizializza la stringa vuota
        

        // Iterazione sui risultati della query e salvataggio dei pazienti
        while (mongoc_cursor_next(cursor, &result)) {
            found_documents = true; 

            char paziente_info[250];  // Array temporaneo per salvare i dati del paziente
            paziente_info[0] = '\0';

            const bson_value_t *value;
            bson_iter_t iter;
            bson_iter_init(&iter, result);

            // Lettura dell'id del documento MongoDB
            if (bson_iter_init_find(&iter,result, "_id") && BSON_ITER_HOLDS_OID(&iter)) {
                bson_oid_to_string(bson_iter_oid(&iter), paziente_info);
                printf("_id: %s", paziente_info);
                fflush(stdout);
                strcat(paziente_info, "#");
            }

            // Lettura dei dati anagrafici
            if (bson_iter_find_descendant(&iter, "dati_anagrafici", &iter) && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
                const uint8_t *buf;
                uint32_t len;
                bson_iter_document(&iter, &len, &buf);

                bson_t subdoc;
                bson_init_static(&subdoc, buf, len);
                bson_iter_t subiter;
                bson_iter_init(&subiter, &subdoc);

                while (bson_iter_next(&subiter)) {
                    
                    fflush(stdout);
                    value = bson_iter_value(&subiter);
                    if (strcmp(bson_iter_key(&subiter), "nome") == 0) {
                        printf("nome: %s\n", value->value.v_utf8.str);
                        fflush(stdout);
                        strcat(paziente_info, value->value.v_utf8.str);
                        strcat(paziente_info, "#");
                    } else if (strcmp(bson_iter_key(&subiter), "cognome") == 0) {
                        printf("cognome: %s\n", value->value.v_utf8.str);
                        fflush(stdout);
                        strcat(paziente_info, value->value.v_utf8.str);
                        strcat(paziente_info, "#");
                    } else if (strcmp(bson_iter_key(&subiter), "codice_fiscale") == 0) {
                        printf("codefiscale: %s\n", value->value.v_utf8.str);
                        fflush(stdout);
                        strcat(paziente_info, value->value.v_utf8.str);
                        strcat(paziente_info, "^");
                        printf("msg0:");
                    }
                    printf("msg1:");
                }
                printf("msg2:");
            }
            printf("msg3:");

            // aumento l allocazione per la lista e aggiungo un paziente.y
            ListaPazienti = realloc(ListaPazienti, strlen(ListaPazienti) + strlen(paziente_info) + 20); // +2 per il separatore "^" e il terminatore NULL
            strcat(ListaPazienti, paziente_info);
printf("msg4: ");
        }
printf("msg5: ");
        if (!found_documents) {
            printf("msg6: ");
            char *messaggio_notfound = "noDoc#";
            send(client_socket, messaggio_notfound, strlen(messaggio_notfound), 0);
            return;

            // Altre azioni da eseguire nel caso in cui non vengano trovati documenti
        } else{
            printf("msg7: ");
            send(client_socket, ListaPazienti, strlen(ListaPazienti), 0);
            
        }

        printf("msg:");
        char msg[MAX_BUFFER_SIZE];
        char temp_msg[MAX_BUFFER_SIZE];
        // Ricezione del messaggio dal client
        int bytes_received = recv(client_socket, msg, MAX_BUFFER_SIZE, 0);
        remove_whitespace(temp_msg, msg);
        printf(":%s:%s: ", temp_msg,msg);
        if (bytes_received < 0) {
            perror("Errore nella ricezione del messaggio dal client");
            return;
        } else if (bytes_received == 0) {
            fprintf(stderr, "Il client ha chiuso la connessione\n");
            return;
        } else if (strcmp(temp_msg, "closeThread") == 0 || strcmp(temp_msg, "closeThreadOK") == 0 ){
            printf("client request end, closing Thread");
            return;
        }
        printf("msg:%s", msg);

        // Tokenizzazione del messaggio ricevuto
        char *oid_str;
        char *sezione;
        printf("msg:%s", msg);

        oid_str = strtok(msg, "#");
        sezione = strtok(NULL, "#");
printf("msg:%s", msg);
            bson_oid_t oid;
            bson_t *filter;
            bson_t *opts;
            mongoc_cursor_t *cursor2;
            const bson_t *result2;
            
            bson_error_t error;
printf("msg:%s", msg);
            // Converti la stringa ObjectId in un tipo bson_oid_t
            bson_oid_init_from_string(&oid, oid_str);

            // Crea un filtro per l'ObjectId specificato
            filter = BCON_NEW("_id", BCON_OID(&oid));

            // Crea le opzioni di ricerca per includere solo il campo specificato
            opts = BCON_NEW("projection", "{", sezione, BCON_INT32(1), "}");

            // Esegui la query sulla collezione con il filtro e le opzioni specificati
            cursor2 = mongoc_collection_find_with_opts(collection, filter, opts, NULL);

        // Controllo se la sezione è "anagrafica" o "cartella_clinica"
        if (sezione != NULL && (strcmp(sezione, "dati_anagrafici") == 0 || strcmp(sezione, "cartella_clinica") == 0)) {



            if(strcmp(sezione,"dati_anagrafici") == 0){

            
                while (mongoc_cursor_next(cursor2, &result2)) {
                    char dati_anagrafici_del_paziente_scelto[250];  // Array temporaneo per salvare i dati del paziente
                    dati_anagrafici_del_paziente_scelto[0] = '\0'; // Assicura che la stringa sia vuota all'inizio

                    const bson_value_t *value;
                    bson_iter_t iter2;
                    bson_iter_t subiter2;
                    bson_iter_init(&iter2, result2);

                    // Itera attraverso le chiavi e i valori nel documento BSON
                    while (bson_iter_next(&iter2)) {
                        // Ottieni la chiave e il valore correnti
                        const char *key = bson_iter_key(&iter2);
                        value = bson_iter_value(&iter2);

                        // Concatena la chiave e il valore alla stringa con il formato desiderato
                        strcat(dati_anagrafici_del_paziente_scelto, key);
                        strcat(dati_anagrafici_del_paziente_scelto, "#");
                        strcat(dati_anagrafici_del_paziente_scelto, value->value.v_utf8.str);
                        strcat(dati_anagrafici_del_paziente_scelto, "#");
                    }

                    // Invia la stringa al client
                    printf("messaggiono: %s", dati_anagrafici_del_paziente_scelto);
                    send(client_socket, dati_anagrafici_del_paziente_scelto, strlen(dati_anagrafici_del_paziente_scelto), 0);
                }



            } else if (strcmp(sezione,"cartella_clinica") == 0){

                    printf("marameo");
                    send(client_socket, "marameo", strlen("marameo"), 0);
                    //da implementare 
            }

        } else {
            fprintf(stderr, "Sezione inesistente nel datamodel\n");
            return;
        }


 

        // Deallocazione delle risorse

    }
  
  
}



/*void crud_update_document(int client_socket, mongoc_client_t *client, const char *db_name, const char *collection_name, char *arguments, int arguments_length) {
    bson_error_t error;
    printf("(db_name: %s)(collection_name: %s)(keyvalues: %s)\n", db_name, collection_name, arguments);  //temp
    fflush(stdout); 

    if(arguments_length <= 0){
        // Gestisci l'errore o la connessione chiusa
        fprintf(stderr, "Error receiving data from client\n");
    }else{

        // Parse degli argomenti
        char *tokens[9];  // Array per salvare i token
        int num_tokens = 0;  // Contatore per il numero di token
        char *token = strtok(arguments, "#");  // Estrai il primo token

        while (token != NULL && num_tokens < 9) {
            tokens[num_tokens++] = token;  // Salva il token nell'array
            token = strtok(NULL, "#");  // Estrai il token successivo
        }

        // Crea la query BSON
        bson_t *raw_query = bson_new();
        for (int i = 0; i < num_tokens; i += 2) {
            char chiave[strlen(tokens[i]) + strlen("dati_anagrafici.") + 1];
            snprintf(chiave, sizeof(chiave), "dati_anagrafici.%s", tokens[i]);

            if (i + 1 < num_tokens) {
                bson_append_utf8(raw_query, chiave, -1, tokens[i + 1], -1);
            } else {
                printf("Errore: manca il valore per la chiave %s\n", tokens[i]);
                bson_destroy(raw_query);
                return;
            }
        }

        mongoc_collection_t *collection = mongoc_client_get_collection(client, db_name, collection_name);
        mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, raw_query, NULL, NULL);
        const bson_t *result;
        

        // Array di dati anagrafici dei pazienti



        char *ListaPazienti = malloc(1);  // Alloca inizialmente 1 byte per la stringa
        ListaPazienti[0] = '\0';  // Inizializza la stringa vuota
        
        fflush(stdout);
        // Iterazione sui risultati della query e salvataggio dei pazienti
        while (mongoc_cursor_next(cursor, &result)) {

    fflush(stdout);
            char paziente_info[250];  // Array temporaneo per salvare i dati del paziente
            paziente_info[0] = '\0';

            const bson_value_t *value;
            bson_iter_t iter;
            bson_iter_init(&iter, result);

            // Lettura dell'id del documento MongoDB
            if (bson_iter_init_find(&iter,result, "_id") && BSON_ITER_HOLDS_OID(&iter)) {
                bson_oid_to_string(bson_iter_oid(&iter), paziente_info);
                printf("_id: %s", paziente_info);
                fflush(stdout);
                strcat(paziente_info, "#");
            }

            // Lettura dei dati anagrafici
            if (bson_iter_find_descendant(&iter, "dati_anagrafici", &iter) && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
                const uint8_t *buf;
                uint32_t len;
                bson_iter_document(&iter, &len, &buf);

                bson_t subdoc;
                bson_init_static(&subdoc, buf, len);
                bson_iter_t subiter;
                bson_iter_init(&subiter, &subdoc);

                while (bson_iter_next(&subiter)) {
                    
                    fflush(stdout);
                    value = bson_iter_value(&subiter);
                    if (strcmp(bson_iter_key(&subiter), "nome") == 0) {
                        printf("nome: %s", value->value.v_utf8.str);
                        fflush(stdout);
                        strcat(paziente_info, value->value.v_utf8.str);
                        strcat(paziente_info, "#");
                    } else if (strcmp(bson_iter_key(&subiter), "cognome") == 0) {
                        printf("cognome: %s", value->value.v_utf8.str);
                        fflush(stdout);
                        strcat(paziente_info, value->value.v_utf8.str);
                        strcat(paziente_info, "#");
                    } else if (strcmp(bson_iter_key(&subiter), "codice_fiscale") == 0) {
                        printf("codefiscale: %s", value->value.v_utf8.str);
                        fflush(stdout);
                        strcat(paziente_info, value->value.v_utf8.str);
                        strcat(paziente_info, "^");
                    }
                }
                printf("paziinfo: %s", paziente_info);
                fflush(stdout);
            }

            // aumento l allocazione per la lista e aggiungo un paziente.y
            ListaPazienti = realloc(ListaPazienti, strlen(ListaPazienti) + strlen(paziente_info) + 3); // +2 per il separatore "^" e il terminatore NULL
            strcat(ListaPazienti, paziente_info);
                printf("lista iter: %s", ListaPazienti);
                fflush(stdout);
        }

        char *msg;
        send(client_socket, ListaPazienti, strlen(ListaPazienti), 0);
        recv(client_socket, msg, strlen(msg), 0);
        printf("to'ò %s", msg);

        // Deallocazione delle risorse
        free(ListaPazienti);
        bson_destroy(raw_query);
        mongoc_cursor_destroy(cursor);
        mongoc_collection_destroy(collection);

    }
  
}
*/

/*
size_t build_read_query(char **command, char **query, size_t allocated_size) {
    printf("Initial size of query: %zu\n", strlen(*query));
    
    // Inizializza la stringa query con "{" ma aggiungi anche il flag che serve a distinguere la query nel server.
    *query = malloc(allocated_size);
    strcpy(*query, "READ {");
    int sizeofcommands = sizeof(command)/sizeof(command[0]);
    printf("sizeofcommands:%i\n", sizeofcommands);
    printf("Pre for\n");

    for (int index=2 ; index < sizeofcommands-1; index += 2) {
            printf("index:%i\n", index);
                printf("sizeofcommands:%i\n", sizeofcommands);
                    
        // Calcola la lunghezza della stringa concatenata
        size_t current_length = strlen(*query);
        // Verifica se c'è spazio sufficiente nel buffer

        printf("cuurent+quello che aggiungerò size: %zu\n", strlen(command[index + 1]));
                        printf("Pre while piccolo\n");
        printf("Allocated size: %zu\n", allocated_size);
        while (current_length + strlen(command[index]) + strlen(command[index + 1]) + 6 > allocated_size) {
            // Raddoppia la dimensione allocata
            allocated_size *= 2;

            // Rialloca il buffer con la nuova dimensione
            *query = realloc(*query, allocated_size);
            if (*query == NULL) {
                // Gestisci l'errore di allocazione
                printf("Errore durante la riallocazione della memoria\n");
            }
            printf("New query size: %zu\n", allocated_size);
        }
        printf("Post while piccolo\n");

        // Concatena la coppia chiave-valore al formato della query MongoDB
        printf("1\n");
        snprintf(*query + current_length, allocated_size - current_length, "\"%s\": \"%s\", ", command[index] + 1, command[index + 1]);
        printf("6\n");

        index += 2;
        printf("7\n");
    }
    printf("Post while grande\n");

    // Rimuovi la virgola finale se la stringa non è vuota
    printf("Before if\n");
    if (strlen(*query) > 1) {
        printf("Inside if\n");
        (*query)[strlen(*query) - 2] = '}';
        (*query)[strlen(*query) - 1] = '\0';
    } else {
        // Se la stringa è vuota, aggiungi solo la parentesi chiusa
        printf("Inside else\n");
        (*query)[1] = '}';
        (*query)[2] = '\0';
    }
    printf("After else\n");
    printf("Length of query: %zu\n", strlen(*query));

    return strlen(*query);
} */

size_t build_read_query_2(char *key_or_value, char *token, char *query, size_t current_length) {
     // size della query al momento della chiamata alla funzione

    if(strcmp(key_or_value, "key") == 0){

        snprintf(query + current_length, sizeof(token)+1, "%s#",  token);

    }else if(strcmp(key_or_value, "value") == 0){

        snprintf(query + current_length, sizeof(token)+1, "%s#",  token);

    }else if(strcmp(key_or_value, "close_query") == 0){

        snprintf(query + current_length, sizeof(token), "%s",  token);

    }else{
        printf("opzione non riconosciuta");
         return EXIT_FAILURE;
    }

    return strlen(query);
}

/*
size_t build_read_query_3(char *key_or_value, char *token, char **query, size_t allocated_size) {
    size_t current_length = strlen(*query); // size della query al momento della chiamata alla funzione
    size_t command_length = strlen(token); // size del comando da aggiungere

    while (current_length + command_length > allocated_size) {
            // Raddoppia la dimensione allocata
            allocated_size *= 2;

            // Rialloca il buffer con la nuova dimensione
            *query = realloc(*query, allocated_size);
            if (*query == NULL) {
                // Gestisci l'errore di allocazione
                printf("Errore durante la riallocazione della memoria\n");
                return EXIT_FAILURE;
            }

    }


    if(strcmp(key_or_value, "key") == 0){

        snprintf(*query + current_length, allocated_size - current_length, "%s###",  token);

    }else if(strcmp(key_or_value, "value") == 0){

        snprintf(*query + current_length, allocated_size - current_length, "%s###",  token);

    }else if(strcmp(key_or_value, "close_query") == 0){

        snprintf(*query + current_length, allocated_size - current_length, "%s",  token);

    }else{
        printf("opzione non riconosciuta");
         return EXIT_FAILURE;
    }

    return strlen(*query);
}
*/

/*    // Inizializza la stringa query con "{" ma aggiungi anche il flag che serve a distinguere la query nel server.
    *query = malloc(allocated_size);
    strcpy(*query, "READ {");
    int sizeofcommands = sizeof(command)/sizeof(command[0]);
    printf("sizeofcommands:%i\n", sizeofcommands);

    while (current_length + strlen(command) > allocated_size) {
            // Raddoppia la dimensione allocata
            allocated_size *= 2;

            // Rialloca il buffer con la nuova dimensione
            *query = realloc(*query, allocated_size);
            if (*query == NULL) {
                // Gestisci l'errore di allocazione
                printf("Errore durante la riallocazione della memoria\n");
            }
            printf("New query size: %zu\n", allocated_size);
    }

    for (int index=2 ; index < sizeofcommands-1; index += 2) {
            printf("index:%i\n", index);
                printf("sizeofcommands:%i\n", sizeofcommands);
                    
        // Calcola la lunghezza della stringa concatenata
        size_t current_length = strlen(*query);
        // Verifica se c'è spazio sufficiente nel buffer
        //NON FUNZIONA se stampo o ho a che fare con strlen(command[index + 1]) in questa fase = seg fault.
        printf("cuurent+quello che aggiungerò size: %zu\n", strlen(command[index + 1]));
                        printf("Pre while piccolo\n");
        printf("Allocated size: %zu\n", allocated_size);
        while (current_length + strlen(command) > allocated_size) {
            // Raddoppia la dimensione allocata
            allocated_size *= 2;

            // Rialloca il buffer con la nuova dimensione
            *query = realloc(*query, allocated_size);
            if (*query == NULL) {
                // Gestisci l'errore di allocazione
                printf("Errore durante la riallocazione della memoria\n");
            }
            printf("New query size: %zu\n", allocated_size);
        }
        printf("Post while piccolo\n");

        // Concatena la coppia chiave-valore al formato della query MongoDB
        printf("1\n");
        snprintf(*query + current_length, allocated_size - current_length, "\"%s\": \"%s\", ", command[index] + 1, command[index + 1]);
        printf("6\n");

        index += 2;
        printf("7\n");
    }
    printf("Post while grande\n");

    // Rimuovi la virgola finale se la stringa non è vuota
    printf("Before if\n");
    if (strlen(*query) > 1) {
        printf("Inside if\n");
        (*query)[strlen(*query) - 2] = '}';
        (*query)[strlen(*query) - 1] = '\0';
    } else {
        // Se la stringa è vuota, aggiungi solo la parentesi chiusa
        printf("Inside else\n");
        (*query)[1] = '}';
        (*query)[2] = '\0';
    }
    printf("After else\n");
    printf("Length of query: %zu\n", strlen(*query));

    return strlen(*query);
}
*/



// Funzione per aggiornare un documento
//void crud_update_document(mongoc_client_t *client, const char *db_name, const char *collection_name) {
    /*
    mongoc_collection_t *collection = mongoc_client_get_collection(client, db_name, collection_name);

    bson_t *query = BCON_NEW("patient_id", BCON_UTF8("12345"));
    bson_t *update = BCON_NEW("$set", "{", "diagnosis", BCON_UTF8("New Diagnosis"), "}");

    bson_error_t error;
    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        fprintf(stderr, "%s\n", error.message);
    }

    bson_destroy(query);
    bson_destroy(update);
    mongoc_collection_destroy(collection);
    */

//}#include <bson.h>

void crud_update_into_document(mongoc_client_t *client, const char *db_name, const char *collection_name, char *raw_query) {
    /*
    bson_error_t error;
    bson_oid_t oid;
    mongoc_collection_t *collection;
    bson_t *update;

    // Ottieni il riferimento alla collezione
    collection = mongoc_client_get_collection(client, db_name, collection_name);

    // Costruisci la query di ricerca del documento con l'oid specificato
    update = BCON_NEW("_id", BCON_OID(&oid));

    // Esegui la ricerca del documento
    const bson_t *doc = mongoc_collection_find_one_with_opts(collection, update, NULL, NULL, NULL);

    // Se il documento esiste, aggiungi un nuovo evento alla cartella clinica specificata
    if (doc) {
        bson_t *evento = bson_new();
        char *token;
        char *saveptr;
        
        // Tokenizza la stringa raw_query
        token = strtok_r(raw_query, "#", &saveptr);
        while (token != NULL) {
            // Aggiungi chiave-valore al documento evento
            bson_append_utf8(evento, token, -1, strtok_r(NULL, "#", &saveptr), -1);
            token = strtok_r(NULL, "#", &saveptr);
        }

        // Aggiungi l'evento alla cartella clinica nel documento
        bson_t *push_evento = BCON_NEW("$push", "{",
                                            "cartelle_cliniche.$.eventi_clinici", evento,
                                        "}");
        mongoc_collection_update_one(collection, doc, push_evento, NULL, NULL, &error);
        
        // Rilascia le risorse
        bson_destroy(evento);
        bson_destroy(push_evento);
    }

    // Rilascia le risorse
    bson_destroy(update);
    mongoc_collection_destroy(collection);
    mongoc_cleanup();
    */
}



// Funzione per eliminare un documento

void crud_delete_document(mongoc_client_t *client, const char *db_name, const char *collection_name , char *delete_command) {
 /*  bson_error_t error;
    mongoc_collection_t *collection;
    bson_t *query;
    bson_t *update;
    char *token;
    char *saveptr;
    char *Oid = NULL;
    int numero_cartella_clinica;
    char codice_fiscale[20];
    
    // Inizializza la libreria MongoDB
    mongoc_init();
    
    // Ottieni il riferimento alla collezione
    collection = mongoc_client_get_collection(client, db_name, collection_name);
    
    // Tokenizza il comando di eliminazione
    token = strtok_r(delete_command, "^#", &saveptr);
    while (token != NULL) {
        if (strcmp(token, "Oid") == 0) {
            // Ottieni il valore di Oid
            token = strtok_r(NULL, "^#", &saveptr);
            if (token != NULL) {
                Oid = strdup(token);
            }
        } else if (strcmp(token, "numero_cartella") == 0) {
            // Ottieni il valore di numero_cartella_clinica
            token = strtok_r(NULL, "^#", &saveptr);
            if (token != NULL) {
                numero_cartella_clinica = atoi(token);
            }
        }
        token = strtok_r(NULL, "^#", &saveptr);
    }
    
    // Costruisci la query per eliminare il documento
    query = BCON_NEW("cartelle_cliniche", "{",
                         "$elemMatch", "{",
                             "numero_cartella", BCON_INT32(numero_cartella_clinica),
                         "}",
                     "}");
    
    // Esegui la ricerca del documento da eliminare
    const bson_t *doc = mongoc_collection_find_one_with_opts(collection, query, NULL, NULL, NULL);
    if (!doc) {
        printf("Documento non trovato.\n");
        bson_destroy(query);
        mongoc_collection_destroy(collection);
        mongoc_cleanup();
        return;
    }
    
    // Ottieni il codice fiscale del cliente
    const bson_value_t *codice_fiscale_value = bson_lookup_value(doc, "dati_anagrafici.codice_fiscale");
    if (codice_fiscale_value && codice_fiscale_value->value_type == BSON_TYPE_UTF8) {
        strncpy(codice_fiscale, codice_fiscale_value->value.v_utf8.str, 20);
    } else {
        printf("Codice fiscale non trovato.\n");
        bson_destroy(query);
        mongoc_collection_destroy(collection);
        mongoc_cleanup();
        return;
    }
    
    // Salva i dati dell'elemento eliminato
    bson_t *log_entry = BCON_NEW("operazione", BCON_UTF8("DELETE"),
                                 "timestamp", BCON_DATE_TIME((int64_t)time(NULL)),
                                 "codice_fiscale", BCON_UTF8(codice_fiscale),
                                 "cartella_clinica", "{",
                                     "numero_cartella", BCON_INT32(numero_cartella_clinica),
                                 "}");
    
    // Inserisci il log nella collezione di log
    mongoc_collection_t *log_collection = mongoc_client_get_collection(client, db_name, "log");
    mongoc_collection_insert_one(log_collection, log_entry, NULL, NULL, &error);
    mongoc_collection_destroy(log_collection);
    bson_destroy(log_entry);

    // Costruisci l'update per eliminare il documento dalla collezione
    update = BCON_NEW("$pull", "{",
                         "cartelle_cliniche", "{",
                             "numero_cartella", BCON_INT32(numero_cartella_clinica),
                         "}",
                     "}");
    
    // Esegui l'eliminazione del documento
    if (!mongoc_collection_update_one(collection, query, update, NULL, NULL, &error)) {
        printf("Errore durante l'eliminazione del documento: %s\n", error.message);
    } else {
        printf("Documento eliminato con successo.\n");
    }
    
    // Rilascia le risorse
    bson_destroy(query);
    bson_destroy(update);
    mongoc_collection_destroy(collection);
    mongoc_cleanup();
    */
}


void crud_insert_into_document(mongoc_client_t *client, const char *db_name, const char *collection_name) {
    /*
    mongoc_collection_t *collection = mongoc_client_get_collection(client, db_name, collection_name);

    bson_t *query = BCON_NEW("patient_id", BCON_UTF8("12345"));
    bson_error_t error;
    if (!mongoc_collection_delete_one(collection, query, NULL, NULL, &error)) {
        fprintf(stderr, "%s\n", error.message);
    }

    bson_destroy(query);
    mongoc_collection_destroy(collection);
    */

}

/*
int main() {

    mongoc_client_t *client = connect_to_mongodb();

    // Esempio di utilizzo delle funzioni CRUD
    insert_document(client, "database_name", "collection_name");
    read_document(client, "database_name", "collection_name");
    update_document(client, "database_name", "collection_name");
    delete_document(client, "database_name", "collection_name");

    mongoc_client_destroy(client);
    mongoc_cleanup();

    return 0;
}
*/
