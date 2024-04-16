void crud_client_inserimento_evento(char* Oid, int numero_cartella_clinica) {
    char* dati_evento = malloc(MAX_STRING_LENGTH);
    if (dati_evento == NULL) {
        fprintf(stderr, "Errore: impossibile allocare memoria.\n");
        exit(EXIT_FAILURE);
    }
    
    char risposta[MAX_STRING_LENGTH];
    char tipo[MAX_STRING_LENGTH];
    char descrizione[MAX_STRING_LENGTH];
    char campo[MAX_STRING_LENGTH];
    char valore[MAX_STRING_LENGTH];
    
    while (1) {
        printf("Vuoi inserire un evento? (y/n): ");
        fgets(risposta, sizeof(risposta), stdin);
        risposta[strcspn(risposta, "\n")] = '\0'; // Rimuovi il carattere newline
        
        if (strcmp(risposta, "y") != 0) {
            break;
        }
        
        printf("Inserisci il tipo di evento: ");
        fgets(tipo, sizeof(tipo), stdin);
        tipo[strcspn(tipo, "\n")] = '\0'; 
        
        printf("Inserisci la descrizione dell'evento: ");
        fgets(descrizione, sizeof(descrizione), stdin);
        descrizione[strcspn(descrizione, "\n")] = '\0'; 
        
        time_t timestamp = time(NULL);
        
        // Aggiungi i dati dell'evento alla stringa dati_evento
        snprintf(dati_evento + strlen(dati_evento), MAX_STRING_LENGTH - strlen(dati_evento), "UPDATE^Oid#%s#numero_cartella#%d#tipo#%s#descrizione#%s#timestamp#%ld#", Oid, numero_cartella_clinica, tipo, descrizione, timestamp);
        
        while (1) {
            printf("Vuoi aggiungere un nuovo campo e valore? (y/n): ");
            fgets(risposta, sizeof(risposta), stdin);
            risposta[strcspn(risposta, "\n")] = '\0'; 
            
            if (strcmp(risposta, "y") != 0) {
                break;
            }
            
            printf("Inserisci il nome del campo: ");
            fgets(campo, sizeof(campo), stdin);
            campo[strcspn(campo, "\n")] = '\0'; // 
            
            printf("Inserisci il valore del campo: ");
            fgets(valore, sizeof(valore), stdin);
            valore[strcspn(valore, "\n")] = '\0';
            
            // Aggiungi il nuovo campo e valore alla stringa dati_evento
            snprintf(dati_evento + strlen(dati_evento), MAX_STRING_LENGTH - strlen(dati_evento), "%s#%s#", campo, valore);
        }
    }
    
    // Una volta terminata la raccolta dei dati, invia i dati al server
    send(server_socket, dati_evento, strlen(dati_evento), 0);

    free(dati_evento); // Libera la memoria allocata dinamicamente
}


void crud_client_eliminazione_cartella_clinica(char* Oid, int numero_cartella_clinica) {
    char risposta[MAX_BUFFER_SIZE];

    printf("Vuoi eliminare la cartella clinica? (y/n): ");
    fgets(risposta, sizeof(risposta), stdin);
    risposta[strcspn(risposta, "\n")] = '\0'; // Rimuovi il carattere newline

    if (strcmp(risposta, "y") != 0) {
        printf("Operazione di eliminazione annullata.\n");
        return;
    }

    char delete_command[MAX_STRING_LENGTH];
    snprintf(delete_command, sizeof(delete_command), "DELETE^Oid#%s#numero_cartella#%d", Oid, numero_cartella_clinica);

    // Invia il comando di eliminazione al server
    send(server_socket, delete_command, strlen(delete_command), 0);
}
