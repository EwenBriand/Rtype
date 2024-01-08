/*
** @file Ship.cpp
** @brief Implementation of the Ship class.
** @date 2023-12-14 17:30:39.
**
*/

#include "Ship.hpp"
#include "Components.Vanilla/CoreTransform.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "GameRtype.hpp"

MANAGED_RESOURCE(Ship)

const std::string Ship::COMMAND_UP = "up";
const std::string Ship::COMMAND_DOWN = "down";
const std::string Ship::COMMAND_LEFT = "left";
const std::string Ship::COMMAND_RIGHT = "right";
const std::string Ship::COMMAND_SHOOT = "shoot";

// ===========================================================================================================
// Component
// ===========================================================================================================

void Ship::OnAddComponent(int entityID)
{
    _entity = entityID;
}

void Ship::Start()
{
    _rb = &SYS.SafeGet<RigidBody2D>(_entity);
    _audio = &SYS.SafeGet<AudioSource>(_entity);
    _core = &SYS.SafeGet<CoreTransform>(_entity);

    _textField = &SYS.SafeGet<TextField>(_entity);
    _textField->SetPosition({ -50, -80 });
    _textField->SetText((std::to_string(_health) + " HP"));

    std::cout << "player id " << _entity << std::endl;

    _collider = &SYS.SafeGet<Collider2D>(_entity);
    _collider->SetTag("player");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            if (tag.compare(0, 11, "Enemy laser") == 0) {
                this->_health -= 1;
                _textField->SetText((std::to_string(_health) + " HP"));
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });

    _audio->AddSoundName("Muse/laser_gun.ogg");
}

void Ship::Update(int entityID)
{
    if (!_controller) {
        return;
    }
    _controller->PollDirectives();
    _rb->SetVelocity({ 0, 0 });
    applyDirectives();
    if (eng::Engine::GetEngine()->IsServer()) {
        checkForDeath();
    }
}

// ===========================================================================================================
// Public methods
// ===========================================================================================================

void Ship::SetID(int id)
{
    _id = id;
}

int Ship::GetID() const
{
    return _id;
}

// ===========================================================================================================
// Private methods
// ===========================================================================================================

void Ship::applyDirectives()
{
    if (not eng::Engine::GetEngine()->PlayMode()) {
        return;
    }
    std::vector<std::string>& directives = _controller->GetDirectives();
    _rb->SetVelocity({ 0, 0 });
    for (auto& directive : directives) {
        if (_actions.find(directive) != _actions.end()) {
            (this->*_actions[directive])();
        }
    }
}

// ===========================================================================================================
// Directives
// ===========================================================================================================

void Ship::moveUp()
{
    if (_core->y > 50)
        _rb->SetVelocity({ _rb->GetVelocity().x, -_speed });
}

void Ship::moveDown()
{
    if (_core->y < 1080 - 100)
        _rb->SetVelocity({ _rb->GetVelocity().x, _speed });
}

void Ship::moveLeft()
{
    if (_core->x > 0 + 50)
        _rb->SetVelocity({ -_speed, _rb->GetVelocity().y });
}

void Ship::moveRight()
{
    if (_core->x < 1920 - 100)
        _rb->SetVelocity({ _speed, _rb->GetVelocity().y });
}

void Ship::shoot()
{
    std::cout << "Ship::shoot()" << std::endl;
    int laser = SYS.GetResourceManager().LoadPrefab("Laser");
    if (_audio->IsPlaying<Sound>("Muse/laser_gun.ogg"))
        _audio->Stop<Sound>("Muse/laser_gun.ogg");
    _audio->Play<Sound>("Muse/laser_gun.ogg");
    try {
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = _core->x;
        laserTransform.y = _core->y;
    } catch (std::exception& e) {
        return;
    }
}

void Ship::checkForDeath()
{
    if (_health <= 0) {
        SYS.UnregisterEntity(_entity);
        eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(eng::RType::I_PLAYER_DIES, 0, serv::bytes(std::vector<int> { _id })));
    }
}
