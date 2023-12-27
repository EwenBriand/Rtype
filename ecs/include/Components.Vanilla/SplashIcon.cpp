
/*
** @file SplashIcon.cpp
** @brief Implementation of the SplashIcon class.
** @date 2023-12-27 11:43:29.
**
*/

#include "SplashIcon.hpp"
#include "Engine.hpp"
#include "ResourceManager.hpp"

void SplashIcon::Update(int entityID)
{
    if (!_icon) {
        loadIcon();
    }
    try {
        auto& transform = SYS.GetComponent<CoreTransform>(entityID);
        _icon->SetOrigin({ transform.x, transform.y });
    } catch (std::exception& e) {
        return;
    }
    _icon->Draw();
    _icon->SetRotation(_icon->GetRotation() + _rotationSpeed);
}

void SplashIcon::OnAddComponent(int entityID)
{
    if (_iconPath.empty()) {
        std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
        try {
            _iconPath = ecs::ResourceManager::MakePath({ assetRoot, "sprites", "splash_icon.png" });
            loadIcon();
        } catch (std::exception& e) {
            std::cerr << "WARNING: Failed to load splash icon: " << e.what() << std::endl;
            _iconPath = "";
        }
    }
}

void SplashIcon::loadIcon()
{
    if (_iconPath.empty()) {
        return;
    }
    _icon = std::make_shared<Sprite>(_iconPath);
}
