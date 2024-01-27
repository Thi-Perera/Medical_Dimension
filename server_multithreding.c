#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>


#define MAX_CONNECTIONS 30

pthread_mutex_t lock;

void* handle_connection(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    // Gestione della sezione critica
    pthread_mutex_lock(&lock);
                        //sezione critica inizio
                        printf("Connessione accettata\n");

                        // Lettura del messaggio inviato dal client
                        char buffer[2048] = {0};
                        recv(client_socket, buffer, sizeof(buffer), 0);

                        // elaborazione effettiva.
                        printf("Messaggio ricevuto:\n%s\n", buffer);// da qui dovrei invece di stampare, gestire l 'inserimento (o altra operazione crud definita nel messaggio stesso) nel db mongo.

                        // Invio di una conferma al client
                        char* confirm_msg = "OK\n";
                        write(client_socket, confirm_msg, strlen(confirm_msg));

                        // Chiusura del socket
                        close(client_socket);
                        //sezione critica fine

    pthread_mutex_unlock(&lock);


    return NULL;
}

int main() {
    // Inizializzazione del mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Errore durante l'inizializzazione del mutex\n");
        return 1;
    }

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

        pthread_detach(thread);
    }

    pthread_exit(NULL);

    // Rilascio delle risorse utilizzate dal mutex
    pthread_mutex_destroy(&lock);

    // Chiusura del socket
    close(server_socket);
    
    return 0;
}