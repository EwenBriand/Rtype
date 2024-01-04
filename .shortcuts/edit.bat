@REM equivalent to this shell script but for windows
@REM #!/bin/bash

@REM ./build/ecs/run_engine --editor --config-dir=game_rtype/.engine/config

@REM set the path to the engine
set engine_path=build\ecs\run_engine.exe

@REM set the path to the config directory
set config_dir=game_rtype/.engine/config

@REM set editor mode
set editor_mode=--editor

@REM run the engine
%engine_path% --config-dir=%config_dir% %editor_mode%
