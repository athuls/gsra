################################################################################
#
# CMake configuration for tools
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

# compile executable: train
################################################################################
add_executable (train src/train.cpp)  
# link executable with external libraries
target_link_libraries (train eblearn idx eblearntools)
LINK_QT(train idxgui)
LINK_QT(train eblearngui)
		
# compile executable: dsfprop
################################################################################
add_executable (dsfprop src/dsfprop.cpp)  
# link executable with external libraries
target_link_libraries (dsfprop eblearn idx eblearntools)
LINK_QT(dsfprop idxgui)
LINK_QT(dsfprop eblearngui)
LINK_MAGICKPP(dsfprop)
		
# compile executable: imfprop
################################################################################
add_executable (imfprop src/imfprop.cpp)  
# link executable with external libraries
target_link_libraries (imfprop eblearn idx eblearntools)
LINK_QT(imfprop idxgui)
LINK_QT(imfprop eblearngui)
LINK_MAGICKPP(imfprop)
		
# compile executable: stdetect
################################################################################
add_executable (stdetect src/stdetect.cpp)
# link executable with external libraries
target_link_libraries (stdetect eblearn idx eblearntools)
LINK_QT(stdetect idxgui)
LINK_QT(stdetect eblearngui)
LINK_BOOST(stdetect system)
LINK_BOOST(stdetect filesystem)
LINK_BOOST(stdetect regex)
LINK_MAGICKPP(stdetect)

# compile executable: detect
################################################################################
add_executable (detect src/detect.cpp)  
# link executable with external libraries
target_link_libraries (detect eblearn idx eblearntools)
LINK_QT(detect idxgui)
LINK_QT(detect eblearngui)
LINK_BOOST(detect system)
LINK_BOOST(detect filesystem)
LINK_BOOST(detect regex)
LINK_MAGICKPP(detect)

# compile executable: mpidetect
################################################################################
IF (MPI_FOUND AND Boost_SERIALIZATION_FOUND AND Boost_MPI_FOUND)
IF (WINDOWS) # do not compile this project under windows
  MESSAGE("__ WARNING: Not generating mpidetect project under Windows")
ELSE (WINDOWS) # do not compile this project under windows
  
add_executable (mpidetect src/mpidetect.cpp)  
# link executable with external libraries
target_link_libraries (mpidetect eblearn idx eblearntools)
LINK_QT(mpidetect idxgui)
LINK_QT(mpidetect eblearngui)
LINK_BOOST(mpidetect system)
LINK_BOOST(mpidetect filesystem)
LINK_BOOST(mpidetect regex)
LINK_BOOST(mpidetect serialization)
LINK_BOOST(mpidetect mpi)
#IF (MPI_FOUND)
#  target_link_libraries (mpidetect pmpich++) # mpi_cxx)
#ENDIF (MPI_FOUND)    

ENDIF (WINDOWS) # do not compile this project under windows
ELSE (MPI_FOUND AND Boost_SERIALIZATION_FOUND AND Boost_MPI_FOUND)
  MESSAGE("__ WARNING: mpidetect will not be compiled because of missing MPI libraries (optional).")
ENDIF (MPI_FOUND AND Boost_SERIALIZATION_FOUND AND Boost_MPI_FOUND)

# # compile executable: track
# ################################################################################
# add_executable (track
#   src/track.cpp
#   )
  
# # link executable with external libraries
# target_link_libraries (track eblearn idx eblearntools)
# LINK_QT(track idxgui)
# LINK_QT(track eblearngui)
# LINK_BOOST(track system)
# LINK_BOOST(track filesystem)
# LINK_BOOST(track regex)

# compile executable: dscompile
################################################################################
add_executable (dscompile src/dscompile.cpp)
# link executable with external libraries
target_link_libraries (dscompile idx eblearn eblearntools)
LINK_QT(dscompile idxgui)
LINK_BOOST(dscompile system)
LINK_BOOST(dscompile filesystem)
LINK_BOOST(dscompile regex)
LINK_MAGICKPP(dscompile)

# compile executable: dssplit
################################################################################
add_executable (dssplit src/dssplit.cpp)
# link executable with external libraries
target_link_libraries (dssplit eblearn idx eblearntools)
LINK_QT(dssplit idxgui)
LINK_QT(dssplit eblearngui)
LINK_BOOST(dssplit system)
LINK_BOOST(dssplit filesystem)
LINK_BOOST(dssplit regex)
LINK_MAGICKPP(dssplit)

# compile executable: dsmerge
################################################################################
add_executable (dsmerge src/dsmerge.cpp)
# link executable with external libraries
target_link_libraries (dsmerge eblearn idx eblearntools)
LINK_QT(dsmerge idxgui)
LINK_QT(dsmerge eblearngui)
LINK_BOOST(dsmerge system)
LINK_BOOST(dsmerge filesystem)
LINK_BOOST(dsmerge regex)
LINK_MAGICKPP(dsmerge)

# compile executable: dsdisplay
################################################################################
add_executable (dsdisplay src/dsdisplay.cpp)
# link executable with external libraries
target_link_libraries (dsdisplay eblearn idx eblearntools)
LINK_QT(dsdisplay idxgui)
LINK_QT(dsdisplay eblearngui)
LINK_BOOST(dsdisplay system)
LINK_BOOST(dsdisplay filesystem)
LINK_BOOST(dsdisplay regex)

# compile executable: matshow
################################################################################
# tell compiler to build WinMain application, not a console app
IF (WINDOWS AND NOT QT_FOUND) # do not compile this project under windows
  MESSAGE("__ WARNING: Not generating matshow project because of missing QT")
ELSE (WINDOWS AND NOT QT_FOUND) # do not compile this project under windows

IF (WINDOWS)
#  SET(WINTYPE WIN32)
#  MESSAGE(STATUS "Building matshow as a windowed application (not console)")
ENDIF (WINDOWS)

add_executable (matshow ${WINTYPE} src/matshow.cpp)
# link executable with external libraries
target_link_libraries (matshow idx eblearntools)
LINK_QT(matshow idxgui)
LINK_QT(matshow eblearngui)
LINK_MAGICKPP(matshow)

ENDIF (WINDOWS AND NOT QT_FOUND)

# compile executable: capture
################################################################################
add_executable (capture src/capture.cpp)
# link executable with external libraries
target_link_libraries (capture idx eblearntools)
LINK_QT(capture idxgui)

# compile executable: metarun
################################################################################
IF (WINDOWS) # do not compile this project under windows
  MESSAGE("__ WARNING: Not generating metarun project under Windows")
ELSE (WINDOWS) # do not compile this project under windows
add_executable (metarun
  src/metarun.cpp
  )
# link executable with external libraries
target_link_libraries (metarun idx eblearntools)
#LINK_BOOST(metarun system)
#LINK_BOOST(metarun filesystem)
#LINK_BOOST(metarun regex)
ENDIF (WINDOWS) # do not compile this project under windows

# compile executable: metaparse
################################################################################
add_executable (metaparse src/metaparse.cpp)
# link executable with external libraries
target_link_libraries (metaparse eblearntools)
LINK_BOOST(metaparse system)
LINK_BOOST(metaparse filesystem)
LINK_BOOST(metaparse regex)

# compile executable: maketable
################################################################################
add_executable (maketable src/maketable.cpp)
# link executable with external libraries
target_link_libraries (maketable idx eblearn)

# compile executable: videoffinder
################################################################################
# IF (OpenCV_FOUND) #-------------------------------------------------------------
# add_executable (videoffinder
#   src/video_features_finder.cpp
#   src/similar_patches.cpp
#   )

# # link executable with external libraries
# ################################################################################
# target_link_libraries (videoffinder eblearn idx)
# IF (QT_FOUND)
#   target_link_libraries (videoffinder idxgui)
# ENDIF (QT_FOUND)
# IF (OpenCV_FOUND)
#   target_link_libraries (videoffinder cv highgui cxcore)
# ENDIF (OpenCV_FOUND)

# ELSE (OpenCV_FOUND)
#   MESSAGE(STATUS "___WARNING___: videoffinder won't be compiled because of missing OpenCV libraries.")
# ENDIF (OpenCV_FOUND) #----------------------------------------------------------
