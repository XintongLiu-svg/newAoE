# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\newAOE_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\newAOE_autogen.dir\\ParseCache.txt"
  "newAOE_autogen"
  )
endif()
