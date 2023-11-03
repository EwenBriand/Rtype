/*
** EPITECH PROJECT, 2023
** ecs
** File description:
** FastStorage.hpp
*/

#pragma once

#include <vector>

namespace ecs {

    /**
     * @brief An implementation of the SParseArray pattern, using its matrix representation.
     * It stores data as void * and can be used to store any type of data.
     *
     */
    class SParseArray {
        public:


        private:
            std::vector<std::vector<void *>> _data;
    };
}
