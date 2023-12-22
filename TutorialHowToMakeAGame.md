# Making a game with REngine

*Note:* find more technical documentation on https://eliotjanvier.github.io/rengine.github.io/

Running a game in the REngine can be a very straightforward process if the game is really simple and has no network requirements. One just needs to load the first scene of the game, and the scripted components will handle the rest.
However, when it comes to multiplayer games, or even single player games that require more setting up, this approach will quickly fail.

To account for this problem, the REngine offers the possibility of loading a game as a dynamic library, instantiating a class inheriting from the IGame interface.

```C++
class IGame {
    public:
        virtual ~IGame() = default;
        /**
         * @brief Called when the game is loaded by the engine.
         *
         */
        virtual void Init(Engine*) = 0;

        /**
         * @brief Called when the game is unloaded by the engine.
         *
         */
        virtual void Cleanup(Engine*) = 0;

        /**
         * @brief Returns true if the game is online, false otherwise.
         * If it returns true, the engine will call the WaitForPlayers method before launching
         * the game.
         *
         */
        virtual bool IsOnLine(Engine*) = 0;

        /**
         * @brief Called when the game is online, before the game is launched.
         *
         */
        virtual void WaitConnect(Engine*) = 0;

        /**
         * @brief Is called to allow the game to load its first scene
         *
         */
        virtual void LoadFirstScene(Engine*) = 0;

        /**
         * @brief Pre-scene instantiation, called after each scene is loaded in
         * the engine, but before it is displayed.
         *
         */
        virtual void PreSceneInstantiationHook(Engine*, const std::string& sceneName) = 0;

        /**
         * @brief Any changes to the engine's pipeline must be done here.
         *
         */
        virtual void ModPipeline(Engine*) = 0;
    };

```

Each method from the IGame interface will be called at specific moments during the initialization of the game, and at runtime.

Lets go over the creation of a small game, called RType.

## Initializing the game folder

Below is the complete tree of the project's folder.

```
game_rtype/
├── assets
│   ├── animations
│   │   ├── dual.anim
│   │   ├── laser.anim
│   │   ├── micro-recon.anim
│   │   └── ship.anim
│   ├── meshes
│   │   ├── RedFighter.mtl
│   │   └── RedFighter.obj
│   ├── Muse
│   │   ├── deja_vu.ogg
│   │   ├── gas_gas_gas.ogg
│   │   ├── mario_jump.ogg
│   │   └── run_meme.ogg
│   ├── prefabs
│   │   ├── dual-ship
│   │   │  
│   │   └── ship
│   └── sprites
│       ├── doom_anniversary.png
│       ├── dual_ship.png
│       ├── laser.png
│       ├── micro_recon.png
│       ├── red-ship.png
│       └── ship.png
├── CMakeLists.txt
├── DistantPlayer.cpp
├── DistantPlayer.hpp
├── .engine
│   ├── config
│   │   └── base.cfg
│   └── scenes
│       ├── level1
│       └── menu
├── GameRoutine.cpp
├── GameRoutine.hpp
├── GameRtype.cpp
├── GameRtype.hpp
├── LobbyCoroutine.cpp
├── LobbyCoroutine.hpp
├── LocalPlayerController.cpp
├── LocalPlayerController.hpp
└── scripts
    ├── EnemyShipController.cpp
    ├── EnemyShipController.hpp
    ├── Level1Manager.cpp
    ├── Level1Manager.hpp
    ├── MenuScript.cpp
    ├── MenuScript.hpp
    ├── Ship.cpp
    └── Ship.hpp
```

Lets start by saying that the  ```.engine``` folder is perhaps the most important of all. It contains important information about the game's files, described in ```base.cfg```.

```
scenesSavePath="../../scenes/"
userScriptsPath="../../../scripts/"
assetRoot="../../../assets/"
game="../../../../build/game_rtype/libgame_target.so"
tmpBuildDir="../../../../"
```
Are indicated in order:
- Where the scenes of the game are saved.
- Where the scripts written by the user are. This can include additional mods later on.
- Where the assets are located.
- Where the compiled binary of the game is located.
- Where the engine will write the source code for compiling metadata.

All these path are relative to the base.cfg file.

**How to run the engine to use the game binary:**
Use the following command line for a server:
```bash
./build/ecs/run_engine --editor --config-dir=<your game name>/.engine/config --server-port=8080 --server-mode --no-graphics
```

For a client you can run:

```bash
./build/ecs/run_engine --editor --config-dir=game_rtype/.engine/config --server-port=8080 --server-ip=127.0.0.1
```
*The ```--no-graphics``` option can also be used on the client for testing purposes*.
*The ```--editor``` option will allow access to the CLI which can be used to send messages to the server using ```ssend <message>``` and to the clients using ```csend <message>```*

## Writting your IGame class

When first loading your game, the REngine will look for a function called ```create``` that will return an instance of your IGame derived class.

```C
extern "C" {
    std::shared_ptr<eng::IGame> create()
    {
        return std::make_shared<eng::RType>();
    }
}
```

Lets write this class. First, in the ```init``` function, you should put any code you need to be ran immediately when the game is loaded. The ```IsOnline``` function will return true to signify that the game requires a server and client to be running.

```C
 void RType::Init(eng::Engine* e)
{
    loadDependencies(e);
}

bool IsOnline(eng::Engine* e)
{
    return true;
}
```

When the game is ready, the engine will ask it what to do. This is the perfect time to load the first scene of your game.

```C
void RType::LoadFirstScene(eng::Engine* e)
{
    e->GetSceneManager().SwitchScene("menu");
}
```

The Wait connect function will give your game the opportunity to initialize its network functionalities.

```C
void RType::WaitConnect(eng::Engine* e)
{
    initServerConnection(e);
}


void RType::initServerConnection(Engine* e)
{
    if (!e->IsOptionSet(eng::Engine::Options::SERVER_MODE) && e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
        connectToServer(e);
    } else if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
        startServer(e);
    } else {
        std::cout << "Starting in single player mode" << std::endl;
    }
}

```
Setting the server up requires indicating which class will handle the requests coming from each client. This class will be instantiated once per client joining the server. Here, we are using the ```DistantPlayer``` class, which inherits from the IClient interface.

```C
void RType::startServer(Engine* e)
{
    try {
        e->GetServer().SetHandleRequest<DistantPlayer>();
        e->GetServer().Start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }
}
```

On the client side, it goes pretty much exactly the same, except that instead of a dedicated class we simply handle the requests with a lambda expression as we won't need to differentiate between several clients.

```C

void RType::connectToServer(Engine* e)
{
    std::string rawIP = e->GetOptionValue(eng::Engine::Options::SERVER_IP);
    std::string rawPort = e->GetOptionValue(eng::Engine::Options::SERVER_PORT);

    if (rawIP.empty() || rawPort.empty()) {
        std::cerr << "Invalid IP address, format is <ip>:<port>" << std::endl;
        exit(84);
    }
    if (rawPort.find_first_not_of("0123456789") != std::string::npos) {
        std::cerr << "Invalid port number" << std::endl;
        exit(84);
    }
    try {
        e->GetClient().SetServerAddress(rawIP, std::stoi(rawPort));
        e->GetClient().SetRequestHandler([](const serv::bytes& data) {
            // your code for the client goes here
        });
        e->GetClient().Start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }
}
```

Finally, before running the game, the engine will setup the render pipeline. This is the list of all the actions that needs to be performed in order to process one frame of the game. The list of the registered actions can be found in ```.pipinfo.txt``` once the engine has run at least once.

```
(-1000) [&]() { SYS.GetInputManager().PollEvents(); }

RTYPE CLIENT ROUTINE

(-600) []() { SYS.ForEach<Collider2D>([](Collider2D& collider) { collider.recalculateEdgesAndNormals(); }); }

(-500) []() { SYS.ForEach<RigidBody2D>([](RigidBody2D& r) { r._acceleration = { 0, 0 }; }); }

(-499) []() { SYS.ForEach<RigidBody2D>([](RigidBody2D& r) { if (r.SimulateGravity()) r._acceleration.y += 9.81; }); }

--- Update() ---

(100) []() { auto deltaTime = SYS.GetDeltaTime(); SYS.ForEach<RigidBody2D>([deltaTime](RigidBody2D& r) { r._velocity.x += r._acceleration.x * deltaTime; r._velocity.y += r._acceleration.y * deltaTime; }); }

(101) []() { auto deltaTime = SYS.GetDeltaTime(); SYS.ForEach<RigidBody2D>([deltaTime](RigidBody2D& r) { try { auto& transform = SYS.GetComponent<CoreTransform>(r._entityID); r.OverridePrevPosition({ transform.x, transform.y }); transform.x += r._velocity.x * deltaTime; transform.y += r._velocity.y * deltaTime; } catch (std::exception& e) { } }); }

```

The number on each instruction represents the priority of the action. The pipeline will be sorted with the lower values coming first, and the system's Update call on all component is at index zero.
We now need to insert the logic of our game's networking into the pipeline. As this is a simple game, let's just process the requests that have been received asynchronously since last frame for now.

```C
void RType::ModPipeline(Engine* e)
{
    // below is the logic for the server
    if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
        e->AddToPipeline([e, this]() {
            e->GetServer().CallHooks();
        },
            -899, "RTYPE SERVER ROUTINE");

    // below is the logic for the client
    } else if (e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
        e->AddToPipeline([e, this]() {
            e->GetClient().CallHook();
        },
            -899, "RTYPE CLIENT ROUTINE");
    }
}
```
## Adding some scene management routines

The game should now be able to start and run. But we are still lacking some more logic to allow distant players to join, wait for them, synchronize the start of the game... Lets make the game more interactive by using the ```States``` class of the engine. This class is a state machine that will keep track of the state of your game and run the logic of the appropriate state.

*Note:* We could also add a component with a script to manage the current scene directly in the scene itself. However, using the ```States``` class and the pipeline will allow for some cross scene behaviors which is much more powerful when dealing with more complex requirements for your game.

Lets start by adding a private member of type ```ecs::States``` to our game class. We will initialize it by indicating what is the first state it should be running. This will be different for the server and the clients, so lets put this in the network initialization.

```C
void RType::startServer(Engine* e)
{
    // ...
        e->GetServer().Start();

        // setting the state machine to start with the lobby of the game
        _stateMachine = ecs::States(std::make_shared<rtype::LobbyRoutineServer>(*e));

    } catch (const std::exception& e) {
    // ...
}

void RType::connectToServer(Engine* e)
{
    // ...
        e->GetClient().Start();

        // setting the state machine to start with the lobby of the game
        _stateMachine = ecs::States(std::make_shared<rtype::LobbyRoutineClient>(*e));

    } catch (const std::exception& e) {
    // ...
}

```

Lets add this to the pipeline. The ```Iter``` function of the ```ecs::States``` class will run the logic of the associated state. Here is the ```ModPipeline``` function after modification:

```C
void RType::ModPipeline(Engine* e)
{
    if (e->IsOptionSet(eng::Engine::Options::SERVER_MODE)) {
        e->AddToPipeline([e, this]() {
            e->GetServer().CallHooks();
            _stateMachine.Iter();
        },
            -899, "RTYPE SERVER ROUTINE");
    } else if (e->IsOptionSet(eng::Engine::Options::SERVER_IP) && e->IsOptionSet(eng::Engine::Options::SERVER_PORT)) {
        e->AddToPipeline([e, this]() {
            e->GetClient().CallHook();
            _stateMachine.Iter();
        },
            -899, "RTYPE CLIENT ROUTINE");
    }
}
```
### Cross frame behaviors with Coroutines

The ```LobbyRoutineClient``` and ```LobbyRoutineServer``` classes are instances of the ```IState``` interface.

```C
 /**
 * @brief An interface for a state of a state machine.
 *
 */
class IState {
public:
    virtual ~IState() = default;

    /**
     * @brief the enter function executes the state's logic.
     *
     */
    virtual void Enter() = 0;

    /**
     * @brief the exit function returns the next state of the state machine,
     * or nullptr if the state does not change. If the state changes, the
     * boolean passed as parameter is set to true.
     *
     * @return std::shared_ptr<IState>
     */
    virtual std::shared_ptr<IState> Exit(bool&) = 0;
};
```
It would be possible to implement the ```Enter``` function traditionally using some class members to keep track of the game state. However, that would lead to writing a lot more methods, creating more data to remember the state of the game, and quite a lot of logic statements to navigate the result. Lets make it lighter and more elegant by using another feature of the REngine, Coroutines.

Coroutines are a way or synchronously handling asynchronous behaviors, in the context of a game engine rendering frames in a loop. The idea is that the coroutine will be able to return like any other function, but will resume its execution where it stopped the next time it will be called. Lets look at the header for the ```LobbyRoutineServer``` class.

```C
 static constexpr unsigned int RTYPE_NB_PLAYERS = 4;

class LobbyRoutineServer : public ecs::IState {
public:
    LobbyRoutineServer(eng::Engine& engine);
    ~LobbyRoutineServer();

    // This will be called by the States class each frame thanks to what we did in ModPipeline
    void Enter() override;
    std::shared_ptr<ecs::IState> Exit(bool& changed) override;

private:
    // This function is the one that will be resumed each frame.
    serv::Coroutine run();

    /**
     * @brief waits for new players to connect and
     * returns true when the lobby is full.
     *
     */
    bool lobbyIsFull();

    unsigned int _nbPlayers;

    // This is a handle to the coroutine
    serv::Coroutine _routine;

    // A reference to the engine because on some OS, using the singleton pattern on the engine won't work in a different binary (the game is a dynamic library distinct from that of the engine itself)
    eng::Engine& _engine;
};
```

Lets look over the implementation of the routine for the lobby on the server side. The client follows the same logic mirrored.

```C
LobbyRoutineServer::LobbyRoutineServer(eng::Engine& engine)
    : _engine(engine)
    , _routine(run())   // This starts running the coroutine until it's first stop point, and stores it in _routine
{
}

LobbyRoutineServer::~LobbyRoutineServer()
{
}


void LobbyRoutineServer::Enter()
{
    if (!_routine.Done()) {
        _routine.Resume();
    }
}

std::shared_ptr<ecs::IState> LobbyRoutineServer::Exit(bool& changed)
{
    if (_routine.Done()) { // if this is true we should exit the lobby and start the game itself
        return std::shared_ptr<ecs::IState>(new GameRoutineServer(_engine)); // indicating to the States class that it should now run the game.
    }

    // else, no change should be made.
    changed = false;
    return nullptr;
}
```

Lets now look at the implementation of the coroutine itself. It will be divided in several parts:
- waiting for all the players to join the game
- sending the signal for loading the first level
- waiting for all clients to be ready (some computers are faster than others)
- sending the start signal so that the players can start the game

```C

bool LobbyRoutineServer::lobbyIsFull()
{
    if (DistantPlayer::Instances.size() != _nbPlayers) {
        _nbPlayers = DistantPlayer::Instances.size();
    }
    return _nbPlayers == RTYPE_NB_PLAYERS;
}

serv::Coroutine LobbyRoutineServer::run()
{
    while (!lobbyIsFull()) {
        co_await std::suspend_always {}; // will exit and resume from here every frame until all players have joined the lobby
    }
    std::cout << "Lobby is full, starting game..." << std::endl;
    for (auto& player : DistantPlayer::Instances) {
        player->SendClientLoadScene("level1");  // All clients should now start loading the first playable level
    }

    int playerReady = 0;    // now waiting for everyone to finish loading the scene
    while (playerReady != RTYPE_NB_PLAYERS) {
        playerReady = 0;
        for (auto& player : DistantPlayer::Instances) {
            if (player->GetAnswerFlag()) { // the flag is set to true by the DistantPlayer class if the client sends a response indicating that he is done loading
                playerReady++;
            }
        }
        co_await std::suspend_always {};  // will exit and resume from here every frame until all players have loaded the scene
    }
    for (auto& player : DistantPlayer::Instances) {
        player->ResetAnswerFlag(); // do not forget to reset the flag indicating that the answer has been received to not break the next synchronous action between clients.
    }
    // The routine is now done and the _routine.Done() method will return true
}
```

Note that the logic used here is closer to the logic that would have been used if this was implemented in a separate thread or an async function that in a component ran by the ECS, but it is still executed in a synchronous way meaning there is no need for handling thread safety.

## Editing scenes

With this, the game is now ready to run. However, there still isn't any scene to display. This section will go over how one can use the CLI editor to make a scene. Starting the engine with the ```--editor``` option starts the engine with a lower framerate, but an interactive console opens in the terminal.

*Note: in future versions, a web based graphical editor is planned.*

In the CLI, typing ```help``` will list the available commands.

```bash
CLI _> help
Available commands (54 in total):
	AddMusic : Add a music
	AddSound : Add a sound
	DeleteMusic : Delete a music
	DeleteSound : Delete a sound
	GetMusicTLength : Get the time length of a music
	GetMusicTPlayed : Get the time played of a music
	IsPlayingMusic : Check if a music is playing
	IsPlayingSound : Check if a sound is playing
	PauseMusic : Pause a music
	PauseSound : Pause a sound
	PlayMusic : Play a music
	...


```
The editor is context based. Actions are performed on the currently selected entity.
List the entities using the ```le``` command.

```bash
CLI _> le
Entities (1 in total)
	0 :
		CLI
		EditorMouseManager
		EntityExplorer

```

By default, there is only one entity listed, with the ID zero. This entity is owned by the system and cannot be manually modified so let's create a new one with ```ne```.

```bash
CLI _> ne
Created entity: 1
CLI _> le
Entities (2 in total)
	0 :
		CLI
		EditorMouseManager
		EntityExplorer
	1 :
		CoreTransform
CLI _>
```
You can see that the new entity has a component CoreTransform set by default. This is the default behavior for all entities. You can jump between entities using the ```ctxt``` command.

```bash
CLI _> ctxt 0
Set context to entity: 0
CLI _> ctxt 1
Set context to entity: 1
CLI _>
```
Lets add a component to our entity. There are two types of components, `Vanilla` and `User`. Vanilla components are already known by the engine and are highly optimized. User components are slower but can be added and modified at runtime.

To add a `Vanilla` component, use `add` followed by the name of the component.

```
CLI _> add RigidBody2D
CLI _> le
Entities (2 in total)
	0 :
		CLI
		EditorMouseManager
		EntityExplorer
	1 :
		CoreTransform
		RigidBody2D
CLI _>
```
The entity now has physics! Lets now add a component written by the user. The source code for this component should be written is the user scripts directory specified in the base.cfg file.

The engine can compile and reload the scripts if they are changed, even at runtime. To write such a script, you can use the following templates:

C++ file:

```C
#include "MyComponent.hpp"

MANAGED_RESOURCE(MyComponent)

void MyComponent::Start() {MyComponent}

void MyComponent::Update(int entityID)
{{

}}
```

H++ file:
```C
#pragma once

#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

serialize class MyComponent : public AUserComponent {{
    public:
        GENERATE_METADATA(MyComponent)
        MyComponent() = default;
        ~MyComponent() = default;

        void Start() override;
        void Update(int entityID) override;

    priate:
}};
```
The serialize keyword indicates to the engine that it should generate metadata about this class. This will enable automatic saving and reloading of the objects, as well as reflection in the code.
*More about the AUserComponent class at https://eliotjanvier.github.io/rengine.github.io/classAUserComponent.html*

Once your component's logic written, you can load it in the engine through the CLI and the ```usercpt-load``` command.

```
CLI _> usercpt-load Ship
CLI _> le
Entities (2 in total)
	0 :
		CLI
		EditorMouseManager
		EntityExplorer
	1 :
		CoreTransform
		Ship
		RigidBody2D
CLI _>
```

If you need to recompile the scripts, you can use the hotreload command. Be careful to save your scene first with ```save <name>``` as all unsaved changes will be lost!

If you wish to create only an entity to be reused later in another scene, or through a script, you can do so by saving it as a `prefab` using the prefab command suite:

```
prefab.list : List all the prefabs
prefab.load : Load a prefab
prefab.remove : Remove a prefab
prefab.save : Save the current entity as a prefab
```
```
CLI _> prefab.list
Prefabs root: .../rtype/game_rtype/assets/prefabs
Prefabs (4 in total)
	ship
	micro-recon
	dual-ship
	laser
```
Save your entity by setting it as the context with `ctxt` and then use `prefab.save <name>` to save it. You can later load it in your scene with `prefab.load <name>`.
If you wish to load this entity through a script, the `ResourceManager` can do so:

```C
// Loading the prefab and getting its entity id
int player = SYS.GetResourceManager().LoadPrefab("ship");
// Getting the user component we added earlier through the cli
Ship* ship = GetUComponent(player, Ship);
// doing some operations, like assigning a controller to the entity
ship->Possess(player, std::make_shared<LocalPlayerController>());
```