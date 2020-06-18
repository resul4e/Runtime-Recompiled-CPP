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

#finds the installed visual studio versions and uses one of them to compile
VSInstallDir = ""
VSInstallDirByte = ""
VSInstallDirByte = subprocess.check_output(VSSE+"/Extern/VSWhere/vswhere.exe -property installationPath")
VSInstallDir = VSInstallDirByte.decode("utf-8")
VSInstallDir = VSInstallDir.rstrip()

if VSInstallDir == "":
    print("error no installation of visual studios found")
    sys.exit()

if project_configuration == "Release":
	configuration_dependant = "/GL /O2 /Oi /LD /Zi"
else:
	configuration_dependant = "/ZI /LDd"

run_command = VSInstallDir +"\\VC\\Auxiliary\\Build\\vcvarsall.bat x" + project_platform
run_command += "&& cl /std:c++17 "+configuration_dependant+" /MP /Fo"+game_path+"/Scripts/Intermediate/Objects/"+project_configuration+"/ "

#temp: cause we can't use this on linux or whatever.
run_command += "/D \"WIN32\" /D \"_WINDOWS\" "

run_command += "/Fd"+game_path+"/Scripts\\Intermediate\\"+scriptName+".pdb /c /showIncludes /nologo /I"+VSSE+" /I"+VSSE+"/Plugins /I"+VSSE+"/Extern/glm /I"+VSSE+"/Extern/spdLog/include /I"+VSSE+"/PIL "+script_path

subprocess.run(run_command)