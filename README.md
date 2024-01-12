# Welcome to the REngine
- official doc available here: https://eliotjanvier.github.io/rengine.github.io/
- tutorials available here: https://eliotjanvier.github.io/rengine-tutorials/

The REngine is the result of a school project at Epitech, called RType. It consists in a small game engine, implementing the ECS design pattern.
For more information, please contact eliot.janvier@epitech.eu.

# V1.0 is here!

## Whats in it

In the 1.0 version, you will find a (mostly) feature complete game engine, with a customizable render pipeline, easy to expand library of components, including colliders, rigid bodies, animations... and the much awaited feature, metadata generation for the user's custom classes to allow for *reflection*. You will also find a server and a client, which you can se to create your own networked games. We also provide a demo project, the RType game but multiplayer.
The engine can be modded by adding dynamic libraries in a plugin folder inside the game folder, along with a .yaml file to provide some configuration.
An example can be found in game_rtype/plugins/LagChecker.

# How to build
If you wish to build from source, run the following command from the root of the repository:
```bash
python3 fast build
```
You can regenerate metadata in case of any missing file using  `./metadata/build/generate_metadata .` if you have tried using fast build once and it failed. Else, you will have to manually build the `generate_metadata` script using the CMakeLists.txt in the metadata folder.

# How to launch

You can launch the game engine in different modes (server, client, or solo) with a selected game configuration.

1. Navigate to the root directory of the repository in your terminal.
2. Run the Python script with the command `python3 launcher.py`.
3. The script will detect available game configurations and list them.
4. Enter the index of the game configuration you want to select.
5. Enter the mode you want to launch the game in. You can choose between 'server', 'client', or 'solo'.
    - 'server': Launches the game in server mode. The game will run without graphics and wait for clients to connect.
    - 'client': Launches the game in client mode. The game will connect to the server at IP 127.0.0.1.
    - 'solo': Launches the game in solo mode. The game will run locally without connecting to a server.
6. The game will then launch with the selected configuration and mode.

Here is a brief description of the command line options used by the game engine:

- `--config-dir=<path>`: Specifies the path to the game configuration directory.
- `--server-port=<port>`: Specifies the port the server should listen on or the client should connect to.
- `--server-mode`: Launches the game in server mode.
- `--no-graphics`: Runs the game without graphics. This is used when launching the game in server mode.
- `--server-ip=<ip>`: Specifies the IP address of the server to connect to. This is used when launching the game in client mode.

# Who are we ?

We are a team of 3rd year students at Epitech. This is a school project, done in a limited time.

# How can I participate ?

1. Choose an issue that you wish to complete.
2. Create a branch for the issue, and do your magic. Some tips:
    - Push using `python3 fast push` to automatically follow the commit norm.
    - Use snake case. Public methods take a capital for the first letter, private member's name start by `_` or `m_`.
3. Create a pull request.
4. Ask for a review from one of the members of the team. He will merge it to main or request for changes!