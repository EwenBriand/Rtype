
/*
** @file Deleteme.hpp
** @brief Definition of the Deleteme class.
** @date 2024-01-05 08:40:12.
**
*/

#pragma once

#include "../Component.hpp"
#include "metadata.hpp"

serialize class Deleteme : public ecs::Component<Deleteme> {
    public:
        GENERATE_METADATA(Deleteme)
        Deleteme() = default;
        ~Deleteme() = default;

        void Update(int entityID);
    private:
};

