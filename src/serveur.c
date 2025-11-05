#include "serv_cli_fifo.h"
#include "Handlers_Serv.h"

int main(void) {
    /* Déclarations */
    int fd_fifo1, fd_fifo2;
    struct question q;
    struct reponse r;
    struct sigaction sa;
    
    printf("=== SERVEUR TUBES NOMMÉS ===\n");
    printf("Serveur PID: %d\n", getpid());
    
    /* Création des tubes nommés */
    if (mkfifo(FIFO1, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Serveur: Erreur création fifo1");
            exit(EXIT_FAILURE);
        }
    }
    
    if (mkfifo(FIFO2, 0666) == -1) {
        if (errno != EEXIST) {
            perror("Serveur: Erreur création fifo2");
            unlink(FIFO1);
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Serveur: Tubes nommés créés.\n");
    
    /* Initialisation du générateur de nombres aléatoires */
    srand(getpid());
    printf("Serveur: Générateur aléatoire initialisé.\n");
    
    /* Ouverture des tubes nommés */
    fd_fifo1 = open(FIFO1, O_RDONLY);
    if (fd_fifo1 == -1) {
        perror("Serveur: Erreur ouverture fifo1");
        unlink(FIFO1);
        unlink(FIFO2);
        exit(EXIT_FAILURE);
    }
    
    fd_fifo2 = open(FIFO2, O_WRONLY);
    if (fd_fifo2 == -1) {
        perror("Serveur: Erreur ouverture fifo2");
        close(fd_fifo1);
        unlink(FIFO1);
        unlink(FIFO2);
        exit(EXIT_FAILURE);
    }
    
    printf("Serveur: Tubes ouverts avec succès.\n");
    
    /* Installation des Handlers */
    ignorer_signaux_clavier();  // Ignorer les signaux clavier en premier
    
    sa.sa_handler = hand_reveil;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    
    sa.sa_handler = fin_serveur;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);
    
    printf("Serveur: Handlers installés. En attente de questions...\n");
    printf("----------------------------------------\n");
    
    /* Boucle principale */
    while(1) {
        /* Lecture d'une question */
        ssize_t bytes_read = read(fd_fifo1, &q, sizeof(q));
        if (bytes_read > 0) {
            printf("Serveur: Question reçue de client %d (n=%d)\n", q.client_id, q.n);
            
            /* Construction de la réponse */
            r.client_id = q.client_id;
            r.n = q.n;
            
            for (int i = 0; i < q.n; i++) {
                r.numbers[i] = rand() % 100;  // Nombres entre 0 et 99
            }
            
            /* Envoi de la réponse */
            write(fd_fifo2, &r, sizeof(r));
            printf("Serveur: Réponse envoyée à client %d (%d nombres)\n", r.client_id, r.n);
            
            /* Envoi du signal SIGUSR1 au client concerné */
            if (kill(q.client_id, SIGUSR1) == 0) {
                printf("Serveur: Signal SIGUSR1 envoyé à client %d\n", q.client_id);
            } else {
                perror("Serveur: Erreur envoi signal");
            }
            printf("----------------------------------------\n");
        } else if (bytes_read == 0) {
            printf("Serveur: Aucun client connecté...\n");
            sleep(2);
        } else {
            perror("Serveur: Erreur lecture question");
        }
    }
    
    /* Fermeture (normalement jamais atteint) */
    close(fd_fifo1);
    close(fd_fifo2);
    unlink(FIFO1);
    unlink(FIFO2);
    
    return 0;
}