
/*
** @file SplashIcon.hpp
** @brief Definition of the SplashIcon class.
** @date 2023-12-27 11:43:29.
**
*/

#pragma once

#include "../Component.hpp"
#include "Animation.hpp"
#include "metadata.hpp"

serialize class SplashIcon : public ecs::Component<SplashIcon> {
public:
    GENERATE_METADATA(SplashIcon)
    SplashIcon() = default;
    ~SplashIcon() = default;

    void OnAddComponent(int entityID);
    void Update(int entityID);

private:
    /**
     * @brief Loads the icon from the filepath _iconPath.
     * If the filepath is empty, it will load the default icon.
     * If the default icon is not found, it will not display anything and
     * output a warning to the console.
     *
     */
    void loadIcon();

    serialize std::string _iconPath = "";
    serialize float _rotationSpeed = 1.0f;

    std::shared_ptr<Sprite> _icon = nullptr;
};
