#!/usr/bin/python
import subprocess
import sys
import os

project_configuration=sys.argv[1]
project_platform=sys.argv[2]
game_path=sys.argv[3]
script_path=sys.argv[4]
scriptName=sys.argv[5]
VSSE=sys.argv[6]


# if project_configuration == "Release":
# 	configuration_dependant = "/GL /O2 /Oi /LD /Zi"
# else:
# 	configuration_dependant = "/ZI /LDd"

#run_command = VSInstallDir +"\\VC\\Auxiliary\\Build\\vcvarsall.bat x" + project_platform
#run_command += "&& cl "+configuration_dependant+" /MP
#
#run_command += "/Fd"+game_path+"/Scripts\\Intermediate\\"+scriptName+".pdb /showIncludes /nologo -I"+VSSE+" -I"+VSSE+"/Plugins -I"+VSSE+"/Extern/glm -I"+VSSE+"/Extern/spdLog/include -I"+VSSE+"/PIL "+script_path

run_command = "g++ -c -fPIC " + script_path + " -o " + game_path + "/Scripts/Intermediate/Objects/"+scriptName+".o" " -I"+VSSE+" -I"+VSSE+"/Plugins -I"+VSSE+"/Extern/glm -I"+VSSE+"/Extern/spdLog/include -I"+VSSE+"/PIL -std=c++17"
os.system(run_command)




#