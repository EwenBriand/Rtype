
/*
** @file MenuScript.hpp
** @brief Definition of the MenuScript class.
** @date 2023-12-14 11:30:57.
**
*/

#pragma once

#include "Components.Vanilla/UIButton.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

serialize class MenuScript : public AUserComponent {
public:
    GENERATE_METADATA(MenuScript)
    MenuScript() = default;
    ~MenuScript();

    void Start() override;
    void Update(int entityID) override;
    void OnAddComponent(int entityID) override;

    static bool _callbacksRegistered;

private:
    void registerButtonCallbacks();

    std::size_t _menuUIHandle = -1;
};
