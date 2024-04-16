#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 7799
#define MAX_CONNECTIONS 30

int main() {
    // Dichiarazione di un array di socket
    int sockets[MAX_CONNECTIONS];
    // Dichiarazione della struttura per l'indirizzo del server
    struct sockaddr_in address;

    // Ciclo per creare e gestire le connessioni
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        // Creazione del socket
        sockets[i]  = socket(AF_INET, SOCK_STREAM, 0);
        if (sockets[i] < 0) {
            printf("Errore durante la creazione del socket\n");
            return 1;
        }

        // Impostazione dell'indirizzo del server
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(PORT);
        if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
            printf("Indirizzo del server non valido\n");
            return 1;
        }

        // Connessione al server
        if (connect(sockets[i], (struct sockaddr *)&address, sizeof(address)) < 0) {
            printf("Errore durante la connessione al server\n");
            return 1;
        }

        // Invio di un messaggio al server
        char* message = "Ciao, sono il client";
        send(sockets[i], message, strlen(message), 0);

        // Ricezione della conferma dal server
        char buffer[1024] = {0};
        recv(sockets[i], buffer, 1024, 0);
        printf("Server: %s\n", buffer);
    }

    // Chiusura delle connessioni
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        close(sockets[i]);
    }

    return 0;
}
