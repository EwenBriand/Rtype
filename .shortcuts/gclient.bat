@REM equivalent to this shell script but for windows
@REM #!/bin/bash

@REM ./build/ecs/run_engine --config-dir=game_rtype/.engine/config --server-port=8080 --server-ip=127.0.0.1

@REM set the path to the engine
set engine_path=build\ecs\run_engine.exe

@REM set the path to the config directory
set config_dir=game_rtype\.engine\config_win

@REM set the port to use
set port=8080

@REM run the engine
%engine_path% --config-dir=%config_dir% --server-port=%port% --server-ip=127.0.0.1