SYSTÈME CLIENT/SERVEUR AVEC TUBES NOMMÉS
=========================================

Description:
------------
Système de communication client/serveur utilisant des tubes nommés (FIFO)
avec synchronisation par signaux SIGUSR1.

Fonctionnement:
---------------
- Le serveur attend les questions des clients dans fifo1
- Chaque question demande n nombres aléatoires
- Le serveur répond dans fifo2 et signale le client avec SIGUSR1
- Le client confirme la lecture avec SIGUSR1

Compilation:
------------
make all

Utilisation:
------------
1. Lancer le serveur: ./serveur
2. Lancer un ou plusieurs clients: ./client

Tests:
------
make test

Nettoyage:
----------
make clean

Auteurs: [Votre nom]
Date: [Date]
UVT - Programmation Système
