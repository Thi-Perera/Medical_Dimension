#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "client_connection.c" 

#define PORT 7799
#define MAX_CONNECTIONS 30


int main() {
    // funzione definita in client_connection.c stampa l'errore e termina il programma o continua se la conn è stabilità
    int risultato_connessione = connessione_al_server();
    int sockets;

    if(risultato_connessione == 1 ){
        exit(EXIT_FAILURE); 
    }
    else{
        sockets = risultato_connessione;
    }

    

        // Invio di un messaggio al server


        // Inserisci un documento di esempio
        char message[2048];
        snprintf(message, sizeof(message),
                "{\"patient_id\":\"%s\","
                "\"name\":{\"first_name\":\"%s\",\"last_name\":\"%s\"},"
                "\"diagnosis\":\"%s\","
                "\"medications\":[\"%s\",\"%s\"],"
                "\"appointments\":["
                "{\"date\":\"%s\",\"doctor\":\"%s\"},"
                "{\"date\":\"%s\",\"doctor\":\"%s\"}"
                "]}",
                "12345", "John", "Doe", "Hypertension", "Aspirin", "Lisinopril",
                "2023-01-01", "Dr. Smith", "2023-02-15", "Dr. Johnson");

        // Ora `json_message` contiene la rappresentazione JSON del messaggio


        // Ottenere una rappresentazione JSON del documento BSON


        // Invia la stringa JSON
        send(sockets, message, strlen(message), 0);

        // Ricezione della conferma dal server
        char buffer[1024] = {0};
        recv(sockets, buffer, 1024, 0);
        printf("Server: %s\n", buffer);// il messaggio dovrebbe essere OK in caso di successo
    

        // libera risorse 
    // Chiusura delle connessioni

        close(sockets);

    return 0;
}
