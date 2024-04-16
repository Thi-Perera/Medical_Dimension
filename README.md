# Medical_Dimension
Sistema client-server per effettuare operazioni CRUD su cartelle cliniche di pazienti di un ospedale, multithreding in C (pthread), archiviazione in mongodb(mongoc-driver)

## per compilare i file usa i comandi:
```gcc
gcc -o server_temporaneo server_insert_to_mongo.c -pthread -I/usr/include -lmongoc-1.0 -lbson-1.0
gcc -o client_temporaneo clinicclient.c -pthread -I/usr/include -lmongoc-1.0 -lbson-1.0
```

## Vedere i seguenti file per i dettagli implementativi:

[report del progetto MedicalDimension](Report_MedicalDimension.pdf)

