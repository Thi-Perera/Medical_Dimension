// connessioneclient.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 7799

int connessione_al_server() {
    // Dichiarazione di un array di socket
    int sockets = socket(AF_INET, SOCK_STREAM, 0);
    /// char risultato_connessione[] = "";
    // Dichiarazione della struttura per l'indirizzo del server
    struct sockaddr_in address;
    
    if (sockets < 0) {
        printf("Errore durante la creazione del socket\n");

        // Creazione di una stringa JSON di errore
        //risultato_connessione[] = "{\"codice\": 1, \"errore\": \"Errore durante la creazione del socket\"}";

        // Output della stringa JSON di errore
        //printf("%s\n", risultato_connessione);

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
    if (connect(sockets, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Errore durante la connessione al server\n");
        return 1;
    }

    // Resto del codice di connessione...

    // Chiusura del socket
    // close(sockets);

    return sockets; // Ritorna 0 se la connessione è avvenuta con successo
}
