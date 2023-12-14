
/*
** @file MenuScript.hpp
** @brief Definition of the MenuScript class.
** @date 2023-12-14 11:30:57.
**
*/

#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

serialize class MenuScript : public AUserComponent {
    public:
        GENERATE_METADATA(MenuScript)
        MenuScript() = default;
        ~MenuScript() = default;

        void Start() override;
        void Update(int entityID) override;

    private:
};

