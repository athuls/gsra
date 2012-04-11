################################################################################
#
# CMake configuration for libeblearn project
#
# Author(s):
#   Pierre Sermanet, pierre.sermanet@gmail.com, New York University
#
################################################################################


# set include directories
################################################################################
include_directories (include)
SET(LIBIDX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

# compile library
################################################################################
add_library (idx SHARED
  src/idxops.cpp  
  src/idx.cpp
  src/idxIO.cpp
  src/ippops.cpp
  src/color_spaces.cpp
  src/image.cpp  
  src/imageIO.cpp  
  src/numerics.cpp
  src/utils.cpp
  src/srg.cpp
  src/stl.cpp
  src/matlab.cpp
  )

# link
################################################################################

#TODO: temporary
#TARGET_LINK_LIBRARIES(idx mat)


IF(CBLAS_FOUND)
  TARGET_LINK_LIBRARIES(idx ${CBLAS_LIBRARIES})
ENDIF(CBLAS_FOUND)

IF(IPP_FOUND)
  TARGET_LINK_LIBRARIES(idx ${IPP_LIBRARIES})
ENDIF(IPP_FOUND)

IF (Magick++_FOUND)
  TARGET_LINK_LIBRARIES(idx ${Magick++_LIBRARY})
ENDIF (Magick++_FOUND)

# configuration
################################################################################
CONFIGURE_FILE("include/config.h.in" 
  "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h")
