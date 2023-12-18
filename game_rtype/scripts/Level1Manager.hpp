
/*
** @file Level1Manager.hpp
** @brief Definition of the Level1Manager class.
** @date 2023-12-18 13:23:00.
**
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

serialize class Level1Manager : public AUserComponent {
    public:
        GENERATE_METADATA(Level1Manager)
        Level1Manager() = default;
        ~Level1Manager() = default;

        void Start() override;
        void Update(int entityID) override;

    private:
};

