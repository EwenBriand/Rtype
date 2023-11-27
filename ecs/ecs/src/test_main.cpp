/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** test_main.cpp
*/

#include <memory>
#include <map>
#include "GraphicalRayLib/GraphicalRayLib.hpp"
#include "Engine.hpp"
#include "ECSImpl.hpp"

int main(int argc, char **argv)
{
    eng::Engine *engine = eng::Engine::GetEngine();
    engine->ParseOptions(argc, argv);
    engine->Run();
    return 0;
}
