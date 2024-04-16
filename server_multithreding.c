#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include </usr/include/libmongoc-1.0/mongoc/mongoc.h>
#include <bson/bson.h>
#include "crudi.c"
#include "paziente.h"
#include "server_utils.c"


#define MAX_CONNECTIONS 30


pthread_mutex_t lock;
mongoc_client_t *client;
volatile sig_atomic_t server_running = 1; // Variabile che indica se il server è in esecuzione

// Gestore di segnali per SIGINT (CTRL+C)
void handle_sigint(int signum) {
    printf("\nServer interrotto. Pulizia delle risorse in corso...\n");
    server_running = 0; // Imposta la variabile a 0 per terminare il ciclo while
}


void initialize_mongodb() {// Funzione di inizializzazione del client MongoDB
    mongoc_init();
    client = mongoc_client_new("mongodb://localhost:27017");
    if (!client) {
        fprintf(stderr, "Failed to create MongoDB client\n");
        exit(EXIT_FAILURE);
    }
    mongoc_client_set_appname(client, "server-medical-dimension");
}

// Funzione di pulizia del client MongoDB
void cleanup_mongodb() {
    mongoc_client_destroy(client);
    mongoc_cleanup();
}

void* handle_connection(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char ack_msg[1024] = {0};
    recv(client_socket, ack_msg, sizeof(ack_msg), 0);
    printf("Client: %s\n", ack_msg); // il messaggio dovrebbe essere OK in caso di successo

    // Invio di una conferma al client
    char* confirm_msg = "OK\n";
    write(client_socket, confirm_msg, strlen(confirm_msg));

    printf("Connessione accettata\n");

    // Ciclo di ascolto continuo
    while (1) {
        char buffer[2048] = {0};
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            // Errore o connessione chiusa, uscire dal ciclo
            printf("Connessione chiusa o errore di ricezione.\n");
            break;
        }

        // Gestione della sezione critica
        pthread_mutex_lock(&lock);
        //sezione critica inizio

        char *flagged_query = buffer;

        char *flag;
        char *query;
        flag = strtok(flagged_query, "^");
        query = strtok(NULL, "^");
        size_t query_size = strlen(query);

        // Verifica l'operazione ricevuta
        if (strcmp(flag, "READ") == 0) {
            // Ora puoi utilizzare la query come necessario
            printf("Operazione di READ: %s\n", flag);
            crud_read_document(client_socket, client, "prova_hospital_db", "patient", query, query_size);
        } else if (strcmp(flag, "CREATE") == 0) {
            // Gestisci operazione CREATE
            printf("Operazione di CREATE: %s\n", query);
            crud_create_document(client, "prova_hospital_db", "patient", query, query_size);
        } else if (strcmp(flag, "UPDATE") == 0) {
            // Gestisci operazione UPDATE
            printf("Operazione di UPDATE: %s\n", query);
            crud_read_document(client_socket, client, "prova_hospital_db", "patient", query, query_size);
            // Aggiorna il documento nel database
        } else if (strcmp(flag, "REMOVE") == 0) {
            // Gestisci operazione REMOVE
            printf("Operazione di REMOVE: %s\n", query);
            // Rimuovi il documento dal database
        } else if (strcmp(flag, "PROVA") == 0) {
            // Gestisci operazione PROVA
            printf("Operazione di PROVA: %s\n", query);
            // Altra operazione di prova
        } else {
            // Gestisci altri tipi di operazioni se necessario
            printf("Operazione sconosciuta: %s\n", flag);
            send(client_socket, "ERROR", strlen("ERROR"), 0);
        }

        // Invio di una conferma al client
        write(client_socket, confirm_msg, strlen(confirm_msg));

        //sezione critica fine
        pthread_mutex_unlock(&lock);
    }

    // Chiusura del socket
    close(client_socket); // Chiusura del socket del client
    pthread_exit(NULL);
    return NULL;
}


int main() {
    

    //inizializzo il client mongodb
    initialize_mongodb();

    //inizzializzo larray di mutex per la concorrenza
    initialize_mutexes();


    // Creazione del socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Errore durante la creazione del socket\n");
        return 1;
    }

    // Impostazione delle opzioni del socket
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        printf("Errore durante l'impostazione delle opzioni del socket\n");
        return 1;
    }

    // Impostazione dell'indirizzo locale del socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7799);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("Errore durante la bind del socket\n");
        return 1;
    }

    // Inizio ad ascoltare su una porta
    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        printf("Errore durante la listen del socket\n");
        return 1;
    }
    printf("Server in ascolto..\n");

 

    // Accettazione di connessioni in entrata
    int client_socket;
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            printf("Errore durante la accept del socket\n");
            continue;
        }

        pthread_t thread;
        int* new_sock = malloc(sizeof(int));
        *new_sock = client_socket;//socket di comunicazione tra server e client

        if (pthread_create(&thread, NULL, handle_connection, (void*)new_sock) < 0) {
            printf("Errore durante la creazione del thread\n");
            free(new_sock);
            continue;
        }

        pthread_detach(thread);// i thread una volta finita l'esecuzione rilasceranno in automatico le risorse, senza aspettare un join con altri thread
    }

    pthread_exit(NULL);// cleanup di pthread
    pthread_mutex_destroy(&lock);// Rilascio delle risorse utilizzate dal mutex
    close(server_socket); // Chiusura del socket
    cleanup_mongodb(); // Chiusura del client MongoDB alla chiusura del server
    
    printf("Server terminato correttamente.\n");

    return 0;
}