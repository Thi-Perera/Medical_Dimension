#include <stdio.h>
#include <stdlib.h>
#include </usr/include/libmongoc-1.0/mongoc/mongoc.h>
#include <bson/bson.h>

int main() {
    // Inizializza il driver MongoDB C
    mongoc_init();

    // Crea un oggetto client MongoDB
    mongoc_client_t *client = mongoc_client_new("mongodb://localhost:27017");

    if (!client) {
        fprintf(stderr, "Failed to create MongoDB client\n");
        return EXIT_FAILURE;
    }

    // Esegui operazioni sul database (ad esempio, inserimento e lettura di dati)
    mongoc_database_t *database = mongoc_client_get_database(client, "prova_hospital_db");
    mongoc_collection_t *collection = mongoc_database_get_collection(database, "patient");

    // Inserisci un documento di esempio
    bson_t *patient_record = BCON_NEW(
        "name", "{", "first_name", BCON_UTF8("John"), "last_name", BCON_UTF8("Doe"), "}",
        "diagnosis", BCON_UTF8("Hypertension"),
        "medications", "[", BCON_UTF8("Aspirin"), BCON_UTF8("Lisinopril"), "]",
        "appointments", "[",
            "{", "date", BCON_UTF8("2023-01-01"), "doctor", BCON_UTF8("Dr. Smith"), "}",
            "{", "date", BCON_UTF8("2023-02-15"), "doctor", BCON_UTF8("Dr. Johnson"), "}",
        "]"
    );

    if (!mongoc_collection_insert_one(collection, patient_record, NULL, NULL, NULL)) {
        fprintf(stderr, "Failed to insert document\n");
        bson_destroy(patient_record);
        return EXIT_FAILURE;
    } else {
        printf("Document inserted successfully.\n");
    }

    // Leggi il documento inserito
    bson_t *query = BCON_NEW("name.first_name", BCON_UTF8("John"), "name.last_name", BCON_UTF8("Doe"));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    const bson_t *result;
    int index = 0;
    while (mongoc_cursor_next(cursor, &result)) {
        index+=1;
        char *str = bson_as_json(result, NULL);
        printf("Found document %i: %s\n", index, str);
        bson_free(str);
    }

    // Rilascia le risorse
    mongoc_cursor_destroy(cursor);
    bson_destroy(query);
    bson_destroy(patient_record);
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return EXIT_SUCCESS;
}
