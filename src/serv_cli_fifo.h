#ifndef SERV_CLI_FIFO_H
#define SERV_CLI_FIFO_H

/* Entêtes des fichiers systèmes nécessaires */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/* Constantes et macros communes */
#define NMAX 100           // Nombre maximum de nombres aléatoires
#define FIFO1 "fifo1"      // Tube pour questions (client → serveur)
#define FIFO2 "fifo2"      // Tube pour réponses (serveur → client)
#define MAX_QUESTIONS 10  // Limite de sécurité

/* Structures des données */

// Structure pour représenter une question (client → serveur)
struct question {
    int client_id;         // PID du client
    int n;                 // Nombre de nombres aléatoires demandés (1 ≤ n ≤ NMAX)
    int nb_questions;      // Nombre total de questions que le client veut poser
    int question_actuelle; // Numéro de la question actuelle
};

// Structure pour représenter une réponse (serveur → client)
struct reponse {
    int client_id;         // PID du client destinataire
    int numbers[NMAX];     // Tableau des nombres aléatoires
    int n;                 // Nombre effectif de nombres générés
    int question_actuelle; // Pour synchronisation
};

#endif