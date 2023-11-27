/*
** EPITECH PROJECT, 2023
** tmp2
** File description:
** test_generic_storage_system.cpp
*/

#include <iostream>
#include "GenericStorageSystem.hpp"

struct TestCpt {
    int a;
    int b;
};

void print(TestCpt *cpt) {
    std::cout << cpt->a << " " << cpt->b << std::endl;
}

// int do_the_stuff(ecs::GenericStorageSystem &storage) {
//     storage.RegisterComponent([](TestCpt *cpt){print(cpt);});
//     return 0;
// }

int main() {
    ecs::GenericStorageSystem storage;
    // do_the_stuff(storage);

    storage.AddComponent(new TestCpt{1, 2});
    storage.RunSystems();
    std::cout << "done, ok" << std::endl;
    return 0;
}
