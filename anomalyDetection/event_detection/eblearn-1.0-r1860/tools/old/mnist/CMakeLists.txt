################################################################################
#
# CMake configuration for the mnist demo project
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

# copy scripts to bin
################################################################################
SET(FILE_TO_BIN 
  src/mnist_meta.conf
)

FOREACH(NAME ${FILE_TO_BIN})
  EXEC_PROGRAM("cp" ARGS "\"${CMAKE_CURRENT_SOURCE_DIR}/${NAME}\""
		  "\"${BINARY_OUTPUT_PATH}\"" OUTPUT_VARIABLE OUT)
  MESSAGE(STATUS "Copied demos/mnist/${NAME} to bin directory.")
ENDFOREACH(NAME)

# compile executable
################################################################################
add_executable (mnist_train
  src/train.cpp
  )
  
# link executable with external libraries
################################################################################
target_link_libraries (mnist_train eblearn idx eblearntools)
IF (QT_FOUND) #---------------------------------------------------------------
  target_link_libraries (mnist_train idxgui)
  target_link_libraries (mnist_train eblearngui)
ENDIF (QT_FOUND) #------------------------------------------------------------
		
# compile executable
################################################################################
add_executable (mnist_reco
  src/reco.cpp
  )
  
# link executable with external libraries
################################################################################
target_link_libraries (mnist_reco eblearn idx)
IF (QT_FOUND) #---------------------------------------------------------------
  target_link_libraries (mnist_reco idxgui)
  target_link_libraries (mnist_reco eblearngui)
ENDIF (QT_FOUND) #------------------------------------------------------------
LINK_MAGICKPP(mnist_reco)
  
