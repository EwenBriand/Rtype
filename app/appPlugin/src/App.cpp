/*
** EPITECH PROJECT, 2024
** app.cpp
** File description:
** Application
*/

#include "App.hpp"
// #include "Engine.hpp"

// void reload() {
//         static App app;

//     if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::APP)) {
//         std::cout << "Push app to pipeline" << std::endl;
//         eng::Engine::GetEngine()->pushPipeline([&]() {
//             analyser.run();
//         }, -825);
//     } else {
//         std::cout << "App not enabled next time use --app" << std::endl;
//     }
// }

// extern "C" void entry_point() {

// }


App::App()
{
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    close(STDOUT_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);

    close(STDIN_FILENO);
    dup2(pipefd[0], STDIN_FILENO);

    close(pipefd[0]);  // Fermez le côté du pipe non utilisé
    close(pipefd[1]);
}

App::~App()
{

}

void App::run()
{
    std::string input;

    while (true) {
        std::getline(std::cin, input);

        std::cout << "Données reçues de l'interface web : " << input << std::endl;

        // Effectuer le traitement nécessaire avec les données reçues

        // Envoyer des données à l'interface web
        std::cout << "Données à envoyer à l'interface web" << std::endl;
    }
}