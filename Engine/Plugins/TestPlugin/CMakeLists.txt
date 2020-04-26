project(TestPlugin)

include_directories(BEFORE "../../PluginCore")
include_directories(BEFORE "../../Core")
include_directories(BEFORE "../../PIL")

#create the test plugin
add_library(TestPlugin SHARED
  "TestPlugin.h"
  "TestPlugin.cpp")


target_link_libraries(TestPlugin "PluginCore")

#add all testing related projects into one visual studio filter
set_target_properties("TestPlugin" PROPERTIES FOLDER Plugins/Tests)