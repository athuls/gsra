################################################################################
#
# CMake configuration for facenet project
#
# Author(s):
#   Cyril Poulet, cyril.poulet@centraliens.net, New York University
#
################################################################################

include_directories (include)
include_directories(${LIBEBLEARN_INCLUDE_DIR})
include_directories(${LIBIDX_INCLUDE_DIR})
include_directories(${EBLEARN_GUI_INCLUDE_DIR})

FIND_PACKAGE(Qt4)
IF (QT_FOUND) #-----------------------------------------------------------------

  # add include directories
  ################################################################################
  include_directories (${QT_INCLUDE_DIR})
  include_directories (${QT_QTGUI_INCLUDE_DIR})
  include_directories (${QT_QTCORE_INCLUDE_DIR})
  
  # compile executable
  ################################################################################
  add_executable (facenet
    src/facenet.cpp
    src/main.cpp
    )
  
  # link executable with external libraries
  ################################################################################
  target_link_libraries (facenet eblearn idx)
  target_link_libraries (facenet eblearn_gui)

  
ENDIF (QT_FOUND) #--------------------------------------------------------------
