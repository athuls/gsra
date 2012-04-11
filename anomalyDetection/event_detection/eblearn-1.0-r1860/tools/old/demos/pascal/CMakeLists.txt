################################################################################
#
# CMake configuration for the pascal demo project
#
# Author(s):
#   Pierre Sermanet, pierre.sermanet@gmail.com, New York University
#
################################################################################

include_directories(include)
include_directories(${LIBEBLEARN_INCLUDE_DIR})
include_directories(${LIBEBLEARNTOOLS_INCLUDE_DIR})
include_directories(${LIBIDX_INCLUDE_DIR})
IF (QT_FOUND)
  include_directories(${LIBIDXGUI_INCLUDE_DIR})
  include_directories(${LIBEBLEARNGUI_INCLUDE_DIR})
ENDIF (QT_FOUND)

# compile executable: pascal_detect
################################################################################
add_executable (pascal_detect
  src/detect.cpp
  )
  
# link executable with external libraries
################################################################################
target_link_libraries (pascal_detect eblearn idx eblearntools)
IF (QT_FOUND) #---------------------------------------------------------------
  target_link_libraries (pascal_detect idxgui)
  target_link_libraries (pascal_detect eblearngui)
ENDIF (QT_FOUND) #------------------------------------------------------------
LINK_BOOST(pascal_detect)