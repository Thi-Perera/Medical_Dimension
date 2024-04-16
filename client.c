#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mongoc/mongoc-client.h>

#define PORT 7799

// Dichiarazione delle funzioni dal file crudi.c (da modificare ancora)
extern void create_document(mongoc_client_t *client, const char *db_name, const char *collection_name);
extern void read_document(mongoc_client_t *client, const char *db_name, const char *collection_name);
extern void update_document(mongoc_client_t *client, const char *db_name, const char *collection_name);
extern void delete_document(mongoc_client_t *client, const char *db_name, const char *collection_name);
extern void insert_into_document(mongoc_client_t *client, const char *db_name, const char *collection_name);

int main() {
    int sockets;
    struct sockaddr_in address;

    sockets = socket(AF_INET, SOCK_STREAM, 0);
    if (sockets < 0) {
        printf("Errore durante la creazione del socket\n");
        return 1;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        printf("Indirizzo del server non valido\n");
        return 1;
    }

    if (connect(sockets, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Errore durante la connessione al server\n");
        return 1;
    }

    // Menù di scelta per l'utente
    printf("Scegli un'operazione:\n");
    printf("1. Create\n");
    printf("2. Read\n");
    printf("3. Update\n");
    printf("4. Delete\n");
    printf("5. Delete\n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            create_document(sockets,"db_medical_records", "patient");
            break;
        case 2:
            read_document(sockets,"db_medical_records", "patient");
            break;
        case 3:
            update_document(sockets,"db_medical_records", "patient");
            break;
        case 4:
            delete_document(sockets,"db_medical_records", "patient");
            break;
        case 5:
            insert_into_document(sockets,"db_medical_records", "patient");
            break;
        default:
            printf("Scelta non valida\n");
            break;
    }

    // Chiudi la connessione
    close(sockets);

    return 0;
}
