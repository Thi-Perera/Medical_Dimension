// connessioneclient.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include </usr/include/libmongoc-1.0/mongoc/mongoc.h>
#include <bson/bson.h>

#define PORT 7799

// funzione per la connessione al server da parte del client
int connessione_al_server() {
    // Dichiarazione di un array di socket
    int sockets = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    
    if (sockets < 0) {
        printf("Errore durante la creazione del socket\n");

        return 1;
    }
    

    // Impostazione dell'indirizzo del server
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    char *ip_localhost = "127.0.0.1";
    char *ip_lan = "192.168.128.215";
    if (inet_pton(AF_INET, ip_localhost, &address.sin_addr) <= 0) {
        printf("Indirizzo del server non valido\n");

        return 1;
    }

    // Connessione al server
    if (connect(sockets, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Errore durante la connessione al server\n");
        return 1;
    }

    printf("Connesso al server.\n");
    return sockets; // Ritorna 0 se la connessione è avvenuta con successo
}

