#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"

int main(void) {
    /* Déclarations */
    int fd_fifo1, fd_fifo2;
    struct question q;
    struct reponse r;
    struct sigaction sa;
    char buffer[10];
    int nb_questions_total, i;
    
    /* Initialisation aléatoire pour n */
    srand(getpid() * time(NULL));
    
    printf("=== CLIENT TUBES NOMMÉS ===\n");
    printf("Client PID: %d\n", getpid());
    
    /* Ouverture des tubes nommés */
    fd_fifo1 = open(FIFO1, O_WRONLY);
    if (fd_fifo1 == -1) {
        perror("Client: Erreur ouverture fifo1 en écriture");
        exit(EXIT_FAILURE);
    }
    
    fd_fifo2 = open(FIFO2, O_RDONLY | O_NONBLOCK);  // ← MODIFICATION IMPORTANTE
    if (fd_fifo2 == -1) {
        perror("Client: Erreur ouverture fifo2 en lecture");
        close(fd_fifo1);
        exit(EXIT_FAILURE);
    }
    
    printf("Client %d: Tubes ouverts avec succès.\n", getpid());
    
    /* Installation des Handlers */
    ignorer_signaux_clavier_client();
    
    sa.sa_handler = hand_reveil;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    
    /* Construction et envoi d'une question */
    q.client_id = getpid();
    q.n = (rand() % NMAX) + 1;
    
    printf("Client %d: Envoi question (n=%d)\n", q.client_id, q.n);
    write(fd_fifo1, &q, sizeof(q));
    
    /* Attente de la réponse */
    printf("Client %d: En attente du signal SIGUSR1...\n", getpid());
    pause();  // Attente du signal du serveur
    
    /* LECTURE INTELLIGENTE - Chercher SA réponse */
    printf("Client %d: Recherche de ma réponse...\n", getpid());
    
    int reponse_trouvee = 0;
    int tentatives = 0;
    
    while (!reponse_trouvee && tentatives < 10) {
        ssize_t bytes_read = read(fd_fifo2, &r, sizeof(r));
        
        if (bytes_read > 0) {
            if (r.client_id == getpid()) {
                // ✅ C'EST MA RÉPONSE !
                printf("Client %d: ✅ RÉPONSE TROUVÉE - %d nombres: ", getpid(), r.n);
                for (int i = 0; i < r.n; i++) {
                    printf("%d ", r.numbers[i]);
                }
                printf("\n");
                reponse_trouvee = 1;
            } else {
                // ❌ Ce n'est pas ma réponse
                printf("Client %d: ↪️  Ignore réponse pour client %d (n=%d)\n", 
                       getpid(), r.client_id, r.n);
                tentatives++;
            }
        } else if (bytes_read == 0) {
            // Tube vide, attendre un peu
            sleep(0.1); // 100ms
            tentatives++;
        } else {
            if (errno != EAGAIN) { // Erreur autre que "would block"
                perror("Client: Erreur lecture réponse");
            }
            tentatives++;
        }
    }
    
    if (!reponse_trouvee) {
        printf("Client %d: ❌ ERREUR - Ma réponse n'a pas été trouvée!\n", getpid());
        close(fd_fifo1);
        close(fd_fifo2);
        exit(EXIT_FAILURE);
    }
    
    /* Envoi du signal SIGUSR1 au serveur */
    printf("Client %d: Envoi confirmation au serveur...\n", getpid());
    kill(getppid(), SIGUSR1);
    
    /* Traitement local de la réponse */
    printf("Client %d: Traitement terminé avec succès.\n", getpid());
    printf("----------------------------------------\n");
    
    /* Fermeture */
    close(fd_fifo1);
    close(fd_fifo2);
    
    return 0;
}