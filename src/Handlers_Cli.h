#ifndef HANDLERS_CLI_H
#define HANDLERS_CLI_H

#include "serv_cli_fifo.h"
#include <signal.h>

/* Handler pour le réveil du client suite à SIGUSR1 */
void hand_reveil(int sig) {
    (void)sig;  // Marquer le paramètre comme utilisé pour éviter le warning
    // Ce handler est déclenché quand le serveur signale que la réponse est prête
    printf("Client %d: Réponse prête! Signal SIGUSR1 reçu.\n", getpid());
}

/* Fonction pour ignorer les signaux du clavier */
void ignorer_signaux_clavier_client() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

#endif