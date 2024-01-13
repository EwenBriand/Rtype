#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipeParentToChild[2];
    int pipeChildToParent[2];

    // Créer les pipes
    if (pipe(pipeParentToChild) == -1 || pipe(pipeChildToParent) == -1) {
        perror("Erreur lors de la création des pipes");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Erreur lors de la création du processus fils");
        return 1;
    }

    if (pid == 0) {
        // Code du processus fils

        // Fermer les extrémités inutilisées des pipes
        close(pipeParentToChild[1]);
        close(pipeChildToParent[0]);

        // Rediriger l'entrée standard vers le pipe du parent vers le fils
        dup2(pipeParentToChild[0], STDIN_FILENO);

        // Rediriger la sortie standard vers le pipe du fils vers le parent
        dup2(pipeChildToParent[1], STDOUT_FILENO);

        // Fermer les extrémités des pipes maintenant redirigées
        close(pipeParentToChild[0]);
        close(pipeChildToParent[1]);

        std::string command;

        while (true) {
            std::cout << "[C++ Terminal] Enter a command: ";
            std::getline(std::cin, command);

            if (command == "exit") {
                break;
            }

            std::cout << "[C++ Terminal] Executing command: " << command << std::endl;
        }

    } else {
        // Code du processus parent

        // Fermer les extrémités inutilisées des pipes
        close(pipeParentToChild[0]);
        close(pipeChildToParent[1]);

        // Exemple de communication parent vers enfant
        std::string messageToChild = "Bonjour, enfant!";
        write(pipeParentToChild[1], messageToChild.c_str(), messageToChild.length());
        close(pipeParentToChild[1]);

        // Attendre que le fils traite le message
        wait(nullptr);

        // Exemple de communication enfant vers parent
        char buffer[100];
        ssize_t bytesRead = read(pipeChildToParent[0], buffer, sizeof(buffer));
        buffer[bytesRead] = '\0';
        std::cout << "Message du fils: " << buffer << std::endl;

        // Fermer les extrémités des pipes maintenant utilisées
        close(pipeParentToChild[1]);
        close(pipeChildToParent[0]);
    }

    return 0;
}