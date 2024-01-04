import os
import subprocess
import sys
import platform

dir_path = os.path.dirname(os.path.realpath(__file__))
lib_path = os.path.join(dir_path, 'bin')

def is_lib_path_set():
    if platform.system() == 'Windows':
        return lib_path in os.getenv('PATH', '')
    else:
        return lib_path in os.getenv('LD_LIBRARY_PATH', '')

if platform.system() == 'Windows':
    if not is_lib_path_set():
        os.environ['PATH'] = lib_path + ';' + os.getenv('PATH', '')
else:
    if not is_lib_path_set():
        os.environ['LD_LIBRARY_PATH'] = lib_path + ':' + os.getenv('LD_LIBRARY_PATH', '')

cpp_executable = os.path.join(dir_path, 'bin', 'run_engine')
if platform.system() == 'Windows':
    cpp_executable += '.exe'

subprocess.call([cpp_executable] + sys.argv[1:])