################################################################################
#
# CMake configuration for eblearn_tester project
#
# Author(s):
#   Pierre Sermanet, pierre.sermanet@gmail.com, New York University
#
################################################################################

# add include directories
################################################################################
include_directories (include)
include_directories(${LIBEBLEARN_INCLUDE_DIR})
include_directories(${LIBIDX_INCLUDE_DIR})
include_directories(${LIBEBLEARNTOOLS_INCLUDE_DIR})
IF (QT_FOUND)
  include_directories(${LIBIDXGUI_INCLUDE_DIR})
  include_directories(${LIBEBLEARNGUI_INCLUDE_DIR})
ENDIF (QT_FOUND)

IF (CPPUNIT_FOUND) # compile only if cppunit is present
  
  # compile executable
  ##############################################################################
  add_executable (tester
    src/idxIO_test.cpp
    src/idxops_test.cpp
    src/idxops_test2.cpp
    src/ippops_test.cpp
    src/detector_test.cpp
    src/ClusterTest.cpp
    src/datasource_test.cpp
    src/ebl_basic_test.cpp
    src/ebl_preprocessing_test.cpp
    src/IdxIteratorsTest.cpp
    src/IdxTest.cpp
    src/image_test.cpp
    src/ebl_machines_test.cpp
    src/gui3d_test.cpp
    src/main.cpp
    src/MyBriefTestProgressListener.cpp
    src/MyTextOutputter.cpp
    )

  # link executable with external libraries
  ##############################################################################
  IF (NOT WINDOWS)
    target_link_libraries (tester dl)
  ENDIF (NOT WINDOWS)
  target_link_libraries (tester eblearn idx)
  target_link_libraries (tester eblearntools)
  target_link_libraries (tester ${CPPUNIT_LIBRARY})
  IF (QT_FOUND) #---------------------------------------------------------------
    target_link_libraries (tester idxgui)
    target_link_libraries (tester eblearngui)
  ENDIF (QT_FOUND) #------------------------------------------------------------
  LINK_MAGICKPP(tester)
  
ELSE (CPPUNIT_FOUND)
  MESSAGE("Error: CppUnit required to build tester")
  
ENDIF (CPPUNIT_FOUND) #---------------------------------------------------------
 
