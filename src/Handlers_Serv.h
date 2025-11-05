#ifndef HANDLERS_SERV_H
#define HANDLERS_SERV_H

#include "serv_cli_fifo.h"
#include <signal.h>

/* Handler pour le réveil du serveur suite à SIGUSR1 */
void hand_reveil(int sig) {
    (void)sig;  // Marquer le paramètre comme utilisé
    // Ce handler est déclenché quand un client confirme avoir lu sa réponse
    printf("Serveur: Client a confirmé la lecture de sa réponse (Signal %d).\n", sig);
}

/* Handler pour la fin du serveur */
void fin_serveur(int sig) {
    printf("\nServeur: Signal %d reçu. Arrêt propre du serveur...\n", sig);
    
    /* Nettoyage des tubes nommés */
    if (unlink(FIFO1) == 0) {
        printf("Serveur: Tube %s supprimé.\n", FIFO1);
    } else {
        perror("Serveur: Erreur suppression fifo1");
    }
    
    if (unlink(FIFO2) == 0) {
        printf("Serveur: Tube %s supprimé.\n", FIFO2);
    } else {
        perror("Serveur: Erreur suppression fifo2");
    }
    
    printf("Serveur: Arrêt terminé.\n");
    exit(EXIT_SUCCESS);
}

/* Fonction pour ignorer les signaux du clavier */
void ignorer_signaux_clavier() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGTSTP, &sa, NULL);  // Ctrl+Z
    sigaction(SIGQUIT, &sa, NULL);  // Ctrl+backslash
}

#endif