#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_CONNECTION 30
#define OID_LENGTH 24

typedef struct {
    pthread_mutex_t mutex;
    int is_locked;
    char Oid[OID_LENGTH];
} OidMutex;

pthread_mutex_t lock;
mongoc_client_t *client;
OidMutex oid_mutexes[MAX_CONNECTION] = {0};

void initialize_mutexes() {
    for (int i = 0; i < MAX_CONNECTION; i++) {
        pthread_mutex_init(&oid_mutexes[i].mutex, NULL);
        oid_mutexes[i].is_locked = 0;
        memset(oid_mutexes[i].Oid, '\0', sizeof(oid_mutexes[i].Oid));
    }
}

void wait_for_mutex(char* Oid) {
    // Non necessario senza l'utilizzo di una variabile di condizione
}

void lock_mutex(char* Oid) {
    // Ricerca dell'Oid esistente
    for (int i = 0; i < MAX_CONNECTION; i++) {
        if (strcmp(oid_mutexes[i].Oid, Oid) == 0) {
            // Se il mutex è locked, restituisci
            if (oid_mutexes[i].is_locked) {
                printf("Mutex già locked per l'Oid %s\n", Oid);
                return;
            }
            // Altrimenti, blocca il mutex e restituisci
            pthread_mutex_lock(&oid_mutexes[i].mutex);
            oid_mutexes[i].is_locked = 1;
            printf("Mutex locked per l'Oid %s\n", Oid);
            return;
        }
    }

    // Assegnazione di un nuovo mutex
    for (int i = 0; i < MAX_CONNECTION; i++) {
        if (!oid_mutexes[i].is_locked) {
            // Blocca il mutex, memorizza l'Oid e restituisci
            pthread_mutex_lock(&oid_mutexes[i].mutex);
            oid_mutexes[i].is_locked = 1;
            strcpy(oid_mutexes[i].Oid, Oid);
            printf("Nuovo mutex locked per l'Oid %s\n", Oid);
            return;
        }
    }

    // Sovrascrittura del primo mutex non locked
    for (int i = 0; i < MAX_CONNECTION; i++) {
        if (!oid_mutexes[i].is_locked) {
            // Sovrascrivi l'Oid e blocca il mutex
            pthread_mutex_lock(&oid_mutexes[i].mutex);
            oid_mutexes[i].is_locked = 1;
            strcpy(oid_mutexes[i].Oid, Oid);
            printf("Mutex esistente sovrascritto per l'Oid %s\n", Oid);
            return;
        }
    }

    // Se non c'è spazio nell'array, restituisci
    printf("Spazio non disponibile per l'Oid %s\n", Oid);
}


void unlock_mutex(char* Oid) {
    // Cerca il mutex corrispondente all'Oid specificato
    for (int i = 0; i < MAX_CONNECTION; i++) {
        if (strcmp(oid_mutexes[i].Oid, Oid) == 0) {
            // Sblocca il mutex e cancella l'Oid
            pthread_mutex_unlock(&oid_mutexes[i].mutex);
            oid_mutexes[i].is_locked = 0;
            memset(oid_mutexes[i].Oid, '\0', OID_LENGTH);
            printf("Mutex unlocked per l'Oid %s\n", Oid);
            return;
        }
    }
    // Se non viene trovato alcun mutex per l'Oid specificato
    printf("Nessun mutex presente con l'Oid %s\n", Oid);
}

int is_mutex_locked(char* Oid) {
    // Cerca il mutex corrispondente all'Oid specificato
    for (int i = 0; i < MAX_CONNECTION; i++) {
        if (strcmp(oid_mutexes[i].Oid, Oid) == 0) {
            // Restituisci lo stato del mutex
            if (oid_mutexes[i].is_locked) {
                printf("Mutex locked per l'Oid %s\n", Oid);
                return 1;
            } else {
                printf("Mutex non locked per l'Oid %s\n", Oid);
                return 0;
            }
        }
    }
    // Se non viene trovato alcun mutex per l'Oid specificato
    printf("Nessun mutex trovato per l'Oid %s\n", Oid);
    return 0;
}
