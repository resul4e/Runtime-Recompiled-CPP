import sys
import subprocess
import os

project_configuration=sys.argv[1]
project_platform=sys.argv[2]
game_path=sys.argv[3]
SharedObjectID=sys.argv[4]
RootBinaryDir=sys.argv[5]
EngineSourceDir=sys.argv[6]

objects_to_link = ""
bin_path = game_path+"/Scripts/bin/"
library_path = RootBinaryDir+"/bin/"
library_includes = library_path+"/libCore.so "+library_path+"/libRendering.so "+ \
	library_path+"/libInput.so "+library_path+"/libTransform.so "+ \
	library_path+"/libAudio.so "+ library_path+"/libPhysics.so "

for i in range(7, len(sys.argv)):
    objects_to_link += game_path+"/Scripts/Intermediate/Objects/"+ sys.argv[i] + ".o "

#if project_configuration == "Release":
#	configuration_dependant = " /DEBUG:NONE"
#else:
#	configuration_dependant = " /DEBUG /PDB:"+game_path+"\\Scripts\\Intermediate\\Scripts"+SharedObjectID+".pdb /PDBALTPATH:"+game_path+"/Scripts/Intermediate/Scripts"+SharedObjectID+".pdb"
#
##finds the installed visual studio versions and uses it to compile
#VSInstallDir = ""
#VSInstallDirByte = ""
#VSInstallDirByte = subprocess.check_output(EngineSourceDir+"/Extern/VSWhere/vswhere.exe -property installationPath")
#VSInstallDir = VSInstallDirByte.decode("utf-8")
#VSInstallDir = VSInstallDir.rstrip()
#
#run_command = VSInstallDir+"\\VC\\Auxiliary\\Build\\vcvarsall.bat x" + project_platform
#run_command +="&& LINK /MACHINE:X"+project_platform+configuration_dependant+ " /IMPLIB:"+bin_path+"/Scripts.lib"
#run_command += " /LIBPATH:"+bin_path+" /OUT:"+bin_path+"/Scripts"+SharedObjectID+".dll /DLL /INCREMENTAL:NO " + library_includes+ " " + objects_to_link
#
#
#subprocess.run(run_command)

run_command = "g++ -shared -o " +bin_path+"/libScripts"+SharedObjectID+".so " + objects_to_link

os.system(run_command)