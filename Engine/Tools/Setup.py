import subprocess
import sys
import os

project_configuration=sys.argv[1]
project_platform=sys.argv[2]
game_path=sys.argv[3]

if not os.path.isdir(game_path+"/Intermediate/Objects/" + project_configuration + "/"+ project_platform):
	os.makedirs(game_path+"/Intermediate/Objects/" + project_configuration + "/"+ project_platform)

if not os.path.isdir(game_path+"/bin/" + project_platform+ "/"+ project_configuration):
	os.makedirs(game_path+"/bin/" + project_platform+ "/"+ project_configuration)
