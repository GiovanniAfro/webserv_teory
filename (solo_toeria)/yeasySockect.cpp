#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;

int main() {
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char buffer[30000] = {0};
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello Word!</h1></body></html>";

    // Creazione della socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket creation");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // Associazione della socket all'indirizzo e alla porta specificati
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // Ascolto di connessioni in entrata
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while(true) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, buffer, 30000);
        std::cout << "----- Request -----\n" << buffer << std::endl;
        write(new_socket, hello, strlen(hello));
        std::cout << "----- Response sent -----\n";
        close(new_socket);
    }
    return 0;
}



/* 
### Inclusione delle Librerie Necessarie

```cpp
#include <iostream> // Per output/input di base.
#include <sys/socket.h> // Per le socket API.
#include <netinet/in.h> // Per la struttura `sockaddr_in`.
#include <unistd.h> // Per `read()`, `write()`, e `close()`.
#include <cstring> // Per `memset()`.
```

Queste librerie forniscono le funzionalità necessarie per la creazione di socket, la gestione delle connessioni, e le operazioni di input/output di base.

### Definizione della Porta

```cpp
const int PORT = 8080;
```

Questa linea definisce la porta su cui il server ascolterà le connessioni in entrata. La porta 8080 è comunemente usata per lo sviluppo web locale.

### Inizializzazione del Server

```cpp
int server_fd, new_socket;
long valread;
struct sockaddr_in address;
int addrlen = sizeof(address);
```

- `server_fd` è il file descriptor della socket del server.
- `new_socket` rappresenterà il file descriptor per ogni nuova connessione accettata.
- `valread` conterrà il numero di byte letti dalla richiesta del client.
- `address` è una struttura che contiene l'indirizzo del server.
- `addrlen` è la dimensione della struttura `address`, necessaria per alcune funzioni socket.

### Creazione della Socket

```cpp
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("In socket creation");
    exit(EXIT_FAILURE);
}
```

Qui, creiamo una nuova socket usando `socket()`. `AF_INET` indica che useremo l'IPv4. `SOCK_STREAM` indica che stiamo creando una socket orientata alla connessione (TCP). Se la creazione fallisce, stampiamo un errore e usciamo.

### Configurazione dell'Indirizzo del Server

```cpp
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);

memset(address.sin_zero, '\0', sizeof address.sin_zero);
```

- Impostiamo `sin_family` su `AF_INET` per usare l'IPv4.
- `sin_addr.s_addr` impostato su `INADDR_ANY` significa che il server ascolterà su tutte le interfacce disponibili.
- `sin_port` contiene la porta, convertita in network byte order con `htons()` per garantire la compatibilità.
- `sin_zero` è un campo non utilizzato che deve essere riempito con zeri per motivi di compatibilità.

### Associazione della Socket

```cpp
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("In bind");
    exit(EXIT_FAILURE);
}
```

Associa la socket all'indirizzo IP e alla porta specificati nella struttura `address`. Se fallisce, stampiamo un errore e usciamo.

### Ascolto di Connessioni

```cpp
if (listen(server_fd, 10) < 0) {
    perror("In listen");
    exit(EXIT_FAILURE);
}
```

Dichiara che la socket è pronta ad accettare connessioni in entrata, con una coda massima di 10 connessioni in attesa.

### Accettazione delle Connessioni

```cpp
while(true) {
    printf("\n+++++++ Waiting for new connection ++++++++\n\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("In accept");
        exit(EXIT_FAILURE);
    }
```

Il server entra in un ciclo infinito, stampando un messaggio che indica che è pronto ad accettare nuove connessioni. `accept()` blocca il server finché non arriva una nuova connessione, dopodiché crea un nuovo socket (`new_socket`) per quella connessione.

### Lettura della Richiesta e Invio della Risposta

```cpp
valread = read(new_socket, buffer, 30000); // Legge la richiesta del client.
std::cout << "----- Request -----\n" << buffer << std::endl; // Stampa la richiesta.
write(new_socket, hello, strlen(hello)); // Invia la risposta al client.
std::cout << "----- Response sent -----\n";
close(new_socket);

 // Chiude la connessione.
```

- Leggiamo i dati inviati dal client nel buffer e stampiamo la richiesta.
- Inviamo una semplice pagina HTML come risposta.
- Infine, chiudiamo la connessione con il client.

### Conclusione

Questo esempio illustra la base per costruire un server HTTP: ascoltare le connessioni, leggere le richieste, e inviare risposte. Da qui, puoi espandere il server per gestire diversi tipi di richieste, analizzare più dettagliatamente le intestazioni HTTP, e servire file statici o contenuti dinamici. */