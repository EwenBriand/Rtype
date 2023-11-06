/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** main.cpp
*/

#include "metadataGenerator.hpp"

int main(int ac, char **av)
{
    meta::MetadataGenerator generator;
    std::string path = (ac == 2) ? av[1] : "";

    system("pwd");

    if (path == "")
        exit(0);
    else
        generator.generateMetadata(path);
    return 0;
}
