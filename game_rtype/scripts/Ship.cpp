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
const std::string Ship::COMMAND_SHOOT_TCEMORT = "shootTcemort";
const std::string Ship::COMMAND_LAUNCH = "launch";

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
    SetupCollisions();

    _audio->AddSoundName("Muse/laser_gun.ogg");
}

void Ship::SetupCollisions()
{
    _collider->SetTag("player");
    _collider->SetOnCollisionEnter([this](int entityID, int otherID) {
        try {
            std::string tag = (_entity == entityID) ? SYS.GetComponent<Collider2D>(otherID).GetTag() : SYS.GetComponent<Collider2D>(entityID).GetTag();
            std::cout << "collision with " << tag << std::endl;
            if (tag.compare(0, 12, "Enemy laser ") == 0) {
                this->_health -= 1;
                _textField->SetText((std::to_string(_health) + " HP"));
            } else if (tag.compare(0, 4, "Heal") == 0) {
                if (this->_health < 10)
                    this->_health += 1;
                _textField->SetText((std::to_string(_health) + " HP"));
            } else if (tag.compare(0, 2, "X2") == 0) {
                if (_nb_laser < 2)
                    _nb_laser += 1;
            } else if (tag.compare(0, 2, "X3") == 0) {
                if (_nb_laser < 3)
                    _nb_laser += 1;
            } else if (tag.compare(0, 7, "Tcemort") == 0) {
                _tcemort = true;
            } else if (tag.compare(0, 8, "Force_ic") == 0) {
                if (_force_existing || not eng::Engine::GetEngine()->IsServer())
                    return;
                auto laser = SYS.GetResourceManager().LoadPrefab("Force");
                auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
                laserTransform.x = _core->x + 100;
                laserTransform.y = _core->y;
                _forceID = laser;
                _force_existing = true;

                eng::Engine::GetEngine()->SetGlobal("ForceID " + std::to_string(_entity), laser);
                eng::Engine::GetEngine()->SetGlobal("ForceAttached " + std::to_string(_entity), true);

                std::cout << "ForceID " + _entity << "|" << eng::Engine::GetEngine()->GetGlobal<int>("ForceID " + std::to_string(_entity)) << std::endl;

                serv::bytes args(std::vector<int>({ _entity,
                    static_cast<int>(laserTransform.x),
                    static_cast<int>(laserTransform.y) }));
                auto instruction = serv::Instruction(eng::RType::I_FORCE_SPAWN, 0, args);
                eng::Engine::GetEngine()->GetServer().Broadcast(instruction);
            } else if (tag.compare(0, 5, "Force") == 0) {
                eng::Engine::GetEngine()->SetGlobal("ForceAttached " + std::to_string(_entity), true);
                _forceID = (_entity == entityID) ? otherID : entityID;
            } else if (tag == "boss") {
                // this->_health -= 10;
                _textField->SetText((std::to_string(_health) + " HP"));
            } else if (tag == "boss-laser") {
                std::cout << "health minus two" << std::endl;
                // this->_health -= 2;
                _textField->SetText((std::to_string(_health) + " HP"));
            }
        } catch (std::exception& e) {
            std::cerr << "Ship::Start(): " << e.what() << std::endl;
        }
    });
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

int Ship::GetNbLaser() const
{
    return _nb_laser;
}

CoreTransform* Ship::GetCore() const
{
    return _core;
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
    if (_audio->IsPlaying<Sound>("Muse/laser_gun.ogg"))
        _audio->Stop<Sound>("Muse/laser_gun.ogg");
    _audio->Play<Sound>("Muse/laser_gun.ogg");

    switch (_nb_laser) {
    case 1:
        creatOneLaser();
        break;
    case 2:
        creatTwoLaser();
        break;
    case 3:
        creatThreeLaser();
        break;
    };
}

void Ship::creatOneLaser()
{
    try {
        int laser = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = _core->x;
        laserTransform.y = _core->y;

        SendShoot(laserTransform.x, laserTransform.y);
    } catch (std::exception& e) {
        return;
    }
}

void Ship::creatTwoLaser()
{
    try {
        int laser = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = _core->x;
        laserTransform.y = _core->y - 10;

        int laser1 = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& laserTransform1 = SYS.GetComponent<CoreTransform>(laser1);
        laserTransform1.x = _core->x;
        laserTransform1.y = _core->y + 10;

        SendShoot(laserTransform.x, laserTransform.y);
        SendShoot(laserTransform1.x, laserTransform1.y);
    } catch (std::exception& e) {
        return;
    }
}

void Ship::creatThreeLaser()
{
    try {
        int laser = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
        laserTransform.x = _core->x;
        laserTransform.y = _core->y - 15;

        int laser1 = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& laserTransform1 = SYS.GetComponent<CoreTransform>(laser1);
        laserTransform1.x = _core->x;
        laserTransform1.y = _core->y + 15;

        int laser2 = SYS.GetResourceManager().LoadPrefab("Laser");
        auto& laserTransform2 = SYS.GetComponent<CoreTransform>(laser2);
        laserTransform2.x = _core->x;
        laserTransform2.y = _core->y;

        SendShoot(laserTransform.x, laserTransform.y);
        SendShoot(laserTransform1.x, laserTransform1.y);
        SendShoot(laserTransform2.x, laserTransform2.y);
    } catch (std::exception& e) {
        return;
    }
}

void Ship::SendShoot(int x, int y)
{
    auto* engine = eng::Engine::GetEngine();

    if (engine->IsClient()) {
        try {
            auto& transform = SYS.GetComponent<CoreTransform>(_entity);
            std::vector<int> data = {
                _id,
                static_cast<int>(x),
                static_cast<int>(y)
            };
            std::cout << "\rplayer " << _id << " shoots at " << x << ", " << y << std::endl;
            engine->GetClient().Send(serv::Instruction(eng::RType::I_PLAYER_SHOOTS, 0, serv::bytes(data)));
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        };
    }
}

void Ship::shootTcemort()
{
    auto* engine = eng::Engine::GetEngine();

    if (engine->IsClient() && _tcemort) {
        try {
            int laser = SYS.GetResourceManager().LoadPrefab("LaserTcemort");
            auto& laserTransform = SYS.GetComponent<CoreTransform>(laser);
            laserTransform.x = _core->x;
            laserTransform.y = _core->y - 15;

            std::vector<int> data = {
                _id,
                static_cast<int>(laserTransform.x),
                static_cast<int>(laserTransform.y)
            };
            std::cout << "\rplayer " << _id << " shoots a TCEMORT at " << laserTransform.x << ", " << laserTransform.y << std::endl;
            engine->GetClient().Send(serv::Instruction(eng::RType::I_PLAYER_SHOOTS_TCEMORT, 0, serv::bytes(data)));
        } catch (const std::exception& e) {
            CONSOLE::err << "\rFailed to send shoot instruction to server." << std::endl;
        };
    }
}

void Ship::checkForDeath()
{
    if (_health <= 0) {
        SYS.UnregisterEntity(_entity);
        eng::Engine::GetEngine()->GetServer().Broadcast(serv::Instruction(eng::RType::I_PLAYER_DIES, 0, serv::bytes(std::vector<int> { _id })));
    }
}

void Ship::launch()
{
    if (_forceID != -1) {
        eng::Engine::GetEngine()->SetGlobal("ForceAttached " + std::to_string(_entity), false);
        _forceID = -1;
    }
}
