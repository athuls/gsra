################################################################################
#
# CMake configuration for libeblearn_tools project
#
# Author(s):
#   Pierre Sermanet, pierre.sermanet@gmail.com, New York University
#
################################################################################

# add include directories
################################################################################
include_directories (include)
include_directories (${LIBIDX_INCLUDE_DIR})
include_directories (${LIBIDXGUI_INCLUDE_DIR})
include_directories (${LIBEBLEARN_INCLUDE_DIR})
include_directories (${LIBEBLEARNGUI_INCLUDE_DIR})
SET(EBLEARN_TOOLS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

# compile library
################################################################################
add_library (eblearntools SHARED
  src/configuration.cpp
  src/gdb.cpp
  src/tools_utils.cpp
  src/opencv.cpp
  src/metaparser.cpp
  src/job.cpp
  src/mpijob.cpp
  src/sort.cpp
  src/thread.cpp
  src/netconf.cpp
  src/xml_utils.cpp
  src/dataset.cpp
  src/pascal_dataset.cpp
#  src/vfw.cpp
  )
# link executable with external libraries
target_link_libraries (eblearntools eblearn idx)
LINK_OPENCV(eblearntools cv)
LINK_OPENCV(eblearntools highgui)
LINK_OPENCV(eblearntools cxcore)
LINK_BOOST(eblearntools system)
LINK_BOOST(eblearntools filesystem)
LINK_BOOST(eblearntools regex)
LINK_BOOST(eblearntools mpi)
LINK_BOOST(eblearntools serialization)
LINK_QT(eblearntools idxgui)
LINK_QT(eblearntools eblearngui)
LINK_KINECT(eblearntools)
LINK_XML(eblearntools)
LINK_PTHREAD(eblearntools)
