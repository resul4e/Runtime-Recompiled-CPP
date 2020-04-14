import sys
import subprocess
import os

project_configuration=sys.argv[1]
project_platform=sys.argv[2]
game_path=sys.argv[3]
DLL_ID=sys.argv[4]
RootBinaryDir=sys.argv[5]
EngineSourceDir=sys.argv[6]

objects_to_link = ""
bin_path = game_path+"/Scripts/bin/"+project_configuration
library_path = RootBinaryDir+"/bin/"+project_configuration
library_includes = RootBinaryDir+"/Engine/Core/"+project_configuration+"/Core.lib "+RootBinaryDir+"/Engine/Plugins/Rendering/"+project_configuration+"/Rendering.lib "+ \
	RootBinaryDir+"/Engine/Plugins/Input/"+project_configuration+"/Input.lib "+RootBinaryDir+"/Engine/Plugins/Transform/"+project_configuration+"/Transform.lib "+ \
	RootBinaryDir+"/Engine/Plugins/Audio/"+project_configuration+"/Audio.lib "#+\
#RootBinaryDir+"/Engine/Plugins/Physics/"+project_configuration+"/Physics.lib "

for i in range(7, len(sys.argv)):
    objects_to_link += game_path+"/Scripts/Intermediate/Objects/"+project_configuration+"/"+ sys.argv[i] + ".obj "

if project_configuration == "Release":
	configuration_dependant = " /DEBUG:NONE"
else:
	configuration_dependant = " /DEBUG /PDB:"+game_path+"\\Scripts\\Intermediate\\Scripts"+DLL_ID+".pdb /PDBALTPATH:"+game_path+"/Scripts/Intermediate/Scripts"+DLL_ID+".pdb"

#finds the installed visual studio versions and uses it to compile
VSInstallDir = ""
VSInstallDirByte = ""
VSInstallDirByte = subprocess.check_output(EngineSourceDir+"/Extern/VSWhere/vswhere.exe -property installationPath")
VSInstallDir = VSInstallDirByte.decode("utf-8")
VSInstallDir = VSInstallDir.rstrip()

run_command = VSInstallDir+"\\VC\\Auxiliary\\Build\\vcvarsall.bat x" + project_platform
run_command +="&& LINK /MACHINE:X"+project_platform+configuration_dependant+ " /IMPLIB:"+bin_path+"/Scripts.lib"
run_command += " /LIBPATH:"+bin_path+" /OUT:"+bin_path+"/Scripts"+DLL_ID+".dll /DLL /INCREMENTAL:NO " + library_includes+ " " + objects_to_link


subprocess.run(run_command)