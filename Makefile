# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = 

# Fichiers sources
SERVER_SRC = src/serveur.c src/serv_cli_fifo.h src/Handlers_Serv.h
CLIENT_SRC = src/client.c src/serv_cli_fifo.h src/Handlers_Cli.h

# Exécutables
SERVER_EXEC = serveur
CLIENT_EXEC = client

# Règle par défaut
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Compilation du serveur
$(SERVER_EXEC): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) src/serveur.c $(LDFLAGS)

# Compilation du client  
$(CLIENT_EXEC): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) src/client.c $(LDFLAGS)

# Nettoyage
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC) fifo1 fifo2 *.o

# Nettoyage complet
distclean: clean
	rm -f *~ \#* .\#*

# Installation (création des tubes)
install: all
	@echo "Création des tubes nommés..."
	@./$(SERVER_EXEC) &
	@sleep 2
	@killall $(SERVER_EXEC) || true
	@echo "Installation terminée - tubes créés"

# Test simple
test: all
	@echo "=== Test du système client/serveur ==="
	@echo "Lancement du serveur en arrière-plan..."
	@./$(SERVER_EXEC) &
	@sleep 2
	@echo "Lancement de 3 clients..."
	@./$(CLIENT_EXEC) &
	@sleep 1
	@./$(CLIENT_EXEC) &
	@sleep 1
	@./$(CLIENT_EXEC) &
	@sleep 5
	@echo "Arrêt du serveur..."
	@killall $(SERVER_EXEC) || true
	@killall $(CLIENT_EXEC) || true
	@echo "=== Test terminé ==="

# Débogage
debug: CFLAGS += -g -DDEBUG
debug: all

.PHONY: all clean distclean install test debug
