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

Questo esempio illustra la base per costruire un server HTTP: ascoltare le connessioni, leggere le richieste, e inviare risposte. Da qui, puoi espandere il server per gestire diversi tipi di richieste, analizzare più dettagliatamente le intestazioni HTTP, e servire file statici o contenuti dinamici. 

Certamente, esaminiamo nel dettaglio i metodi utilizzati per gestire le socket in programmazione di rete e la configurazione dell'indirizzo del server.

### 1. `socket()`

La funzione `socket()` crea una nuova socket, ovvero un endpoint di comunicazione. Restituisce un file descriptor (un intero) che rappresenta la socket appena creata.

- **Sintassi**: `int socket(int domain, int type, int protocol)`
- **Parametri**:
  - `domain`: Specifica il dominio di comunicazione; `AF_INET` è usato per IPv4.
  - `type`: Specifica il tipo di socket; `SOCK_STREAM` per stream socket (TCP) e `SOCK_DGRAM` per datagram socket (UDP).
  - `protocol`: Specifica il protocollo; normalmente impostato a 0 per scegliere automaticamente il protocollo appropriato per il tipo dato.

### 2. `bind()`

La funzione `bind()` associa un indirizzo locale, come l'indirizzo IP e il numero di porta, alla socket. Questo è necessario per i server affinché i client possano connettersi a essi usando questo indirizzo.

- **Sintassi**: `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`
- **Parametri**:
  - `sockfd`: Il file descriptor della socket.
  - `addr`: Puntatore a una struttura `sockaddr` che contiene l'indirizzo IP e il numero di porta.
  - `addrlen`: La dimensione in byte della struttura puntata da `addr`.

### 3. `listen()`

Dopo il binding, la socket viene messa in ascolto per le connessioni in entrata. `listen()` marca la socket come passiva, pronta ad accettare connessioni in entrata.

- **Sintassi**: `int listen(int sockfd, int backlog)`
- **Parametri**:
  - `sockfd`: Il file descriptor della socket.
  - `backlog`: Il numero massimo di connessioni pendenti nella coda di ascolto.

### 4. `accept()`

La funzione `accept()` estrae la prima richiesta di connessione nella coda di ascolto per la socket in ascolto, crea una nuova socket connessa, e restituisce un nuovo file descriptor che si riferisce a quella socket. Questa nuova socket è quella usata per comunicare con il client.

- **Sintassi**: `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`
- **Parametri**:
  - `sockfd`: Il file descriptor della socket in ascolto.
  - `addr`: Puntatore a una struttura `sockaddr` che riceve l'indirizzo del client in connessione.
  - `addrlen`: Puntatore a una variabile che contiene la dimensione di `addr`; alla fine contiene la dimensione effettiva dell'indirizzo del client.

### 5. `read()`

La funzione `read()` legge i dati dalla socket nel buffer fornito.

- **Sintassi**: `ssize_t read(int fd, void *buf, size_t count)`
- **Parametri**:
  - `fd`: Il file descriptor da cui leggere (in questo caso, il file descriptor della socket connessa).
  - `buf`: Il buffer dove memorizzare i dati letti.
  - `count`: Il numero massimo di byte da leggere.

### Configurazione dell'Indirizzo del Server

La struttura `sockaddr_in` è usata per specificare un indirizzo internet (IPv4).

```cpp
address.sin_family = AF_INET;
```
- Imposta il tipo di indirizzo a IPv4.

```cpp
address.sin_addr.s_addr = INADDR_ANY;
```
- Specifica che il server accetterà connessioni su qualsiasi interfaccia di rete del host.

```cpp
address.sin_port = htons(PORT);
```
- Imposta il numero di porta. `htons()` converte il numero di porta in network byte order, necessario per garantire che il numero di porta sia interpretato correttamente su hardware con differenti ordinamenti dei byte.

```cpp
memset(address.sin_zero, '\0', sizeof address.sin_zero);
```
- La funzione `memset()` è usata qui per riempire il resto della struttura con zeri. Questo campo è solo per padding e non ha un significato pratico; è necessario per mantenere la compatibilità con la struttura `sockaddr

` più generale.

Queste operazioni configurano la socket del server per ascoltare le connessioni in entrata su tutte le interfacce di rete disponibili dell'host, su una porta specifica. Questa configurazione è essenziale per permettere ai client di stabilire connessioni con il server.

Il parametro `backlog` nella funzione `listen()` specifica il numero massimo di connessioni in entrata che possono essere messe in coda per la socket in ascolto prima che il sistema operativo inizi a rifiutare le connessioni in entrata aggiuntive. In altre parole, definisce la dimensione della coda di connessioni in attesa di essere accettate.

Quando un server è in ascolto su una porta specifica per le connessioni TCP in entrata, può accadere che più client tentino di connettersi contemporaneamente. Il server, tuttavia, può accettare (tramite la funzione `accept()`) una connessione alla volta. Le richieste di connessione in entrata che arrivano mentre il server è occupato con un'altra connessione non vengono perse; invece, il sistema operativo mette queste connessioni in una coda.

### Come Funziona la Coda di Ascolto

- **Connessioni Pendenti in Attesa di Essere Accettate**: Queste sono connessioni per le quali il client ha completato il handshake TCP, ma il server non ha ancora chiamato `accept()` per accettare la connessione e iniziare a elaborarla.
- **Dimensione della Coda**: La dimensione della coda determina quante di queste connessioni possono essere memorizzate in attesa. Se la coda è piena (cioè, contiene un numero di connessioni uguale al valore di `backlog`) e arrivano nuove richieste di connessione, il sistema operativo può rifiutare le nuove connessioni fino a quando non viene liberato spazio nella coda.

### Importanza del Valore di `backlog`

- **Prestazioni e Scalabilità**: Un valore di `backlog` adeguatamente dimensionato è importante per le prestazioni e la scalabilità di un'applicazione server, in particolare quando ci si aspetta un alto volume di connessioni in entrata. Se `backlog` è troppo piccolo, le connessioni dei client potrebbero essere rifiutate durante i picchi di traffico, anche se il server ha la capacità di elaborarle.
- **Adattamento alla Capacità di Elaborazione**: D'altra parte, impostare un valore di `backlog` molto alto non significa necessariamente che il server sarà in grado di gestire tutte quelle connessioni contemporaneamente. Il server comunque dovrà elaborare ciascuna connessione accettata, quindi il valore di `backlog` dovrebbe riflettere un equilibrio tra la capacità di gestire le connessioni in entrata e la velocità con cui il server può accettare e iniziare a elaborare tali connessioni.

### Comportamento in Diverse Piattaforme

Il comportamento esatto relativo al `backlog` può variare tra diverse implementazioni del sistema operativo. Alcuni sistemi possono interpretare il valore di `backlog` in modo leggermente diverso o possono avere limiti massimi imposti. Inoltre, i sistemi operativi moderni potrebbero automaticamente regolare il valore di `backlog` per ottimizzare le prestazioni, ma è comunque buona pratica fornire un valore che rifletta le esigenze della tua applicazione.

*/