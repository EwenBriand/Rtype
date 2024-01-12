#!/usr/bin/env python3
import sys
import os

BLUE = '\033[94m'
YELLOW = '\033[93m'
ENDC = '\033[0m'

def detect_games():
    base_cfgs = []
    for root, dirs, files in os.walk(os.path.dirname(os.path.realpath(__file__))):
        for file in files:
            if file == 'base.cfg':
                base_cfgs.append(root)
    return base_cfgs

def choose_game(configs):
    while True:
        try:
            selected_index = int(input(f"{BLUE}Please enter the index of the game you want to select: {ENDC}"))
            if 0 <= selected_index < len(configs):
                return configs[selected_index]
            else:
                print("Invalid index. Please try again.")
        except ValueError:
            print("Invalid input. Please enter a number.")

def choose_mode():
    while True:
        mode = input(f"{BLUE}Please enter the mode you want to launch the game in (server/client/solo): {ENDC}")
        if mode.lower() in ['server', 'client', 'solo']:
            return mode.lower()
        else:
            print("Invalid mode. Please try again.")

def launch_game(mode, game):
    base_command = os.path.join(os.path.dirname(os.path.realpath(__file__)), "bin", "run_engine") + " --config-dir={} --server-port=8080".format(game)
    if mode == "server":
        base_command += " --server-mode --no-graphics"
    elif mode == "client":
        base_command += " --server-ip=127.0.0.1"
    os.system(base_command)

if __name__ == '__main__':
    bin_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'bin')
    bin_name = 'run_engine'

    sys.path.append(bin_path)

    os.chdir(os.path.dirname(os.path.realpath(__file__)))

    configs = detect_games()
    print('Detected games:')
    for i, config in enumerate(configs):
        print(f'{i}: {config}')

    selected_game = choose_game(configs)
    print(f"{YELLOW}You selected: {selected_game}{ENDC}")

    selected_mode = choose_mode()
    print(f"{YELLOW}You selected: {selected_mode} mode{ENDC}")

    launch_game(selected_mode, selected_game)