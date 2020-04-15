import subprocess
import sys
import os

project_configuration=sys.argv[1]
game_path=sys.argv[2]

if not os.path.isdir(game_path+"/Scripts/Intermediate/Objects/" + project_configuration):
	os.makedirs(game_path+"/Scripts/Intermediate/Objects/" + project_configuration)

if not os.path.isdir(game_path+"/Scripts/bin/" + project_configuration):
	os.makedirs(game_path+"/Scripts/bin/" + project_configuration)
