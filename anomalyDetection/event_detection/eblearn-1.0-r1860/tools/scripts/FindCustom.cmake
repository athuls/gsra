# Custom cmake configuration
# Put here paths to manually installed libraries if necessary
################################################################################

################################################################################
# Intel IPP
################################################################################
# SET(IPP_FOUND TRUE)
# SET(IPP_INCLUDE_DIR "/home/pierre/ipp/include/")
# SET(IPP_LIBRARIES_DIR "/home/pierre/ipp/sharedlib/")

################################################################################
#SET(Boost_FOUND TRUE)
#SET(Boost_INCLUDE_DIRS "$ENV{HOME}/installed/boost/include")
#SET(Boost_LIBRARY_DIRS "$ENV{HOME}/installed/boost/lib")
#SET(Boost_MINOR_VERSION 35)

 # SET(CPPUNIT_FOUND TRUE)
 # SET(CPPUNIT_INCLUDE_DIR "C:/Users/pierre/Downloads/cppunit-1.12.1/include")
 # SET(CPPUNIT_LIBRARY "C:/Users/pierre/Downloads/cppunit-1.12.1/lib/cppunitd.lib")
 # MESSAGE(STATUS "Found CPPUNIT")

IF ($ENV{USEMPI})
  SET(MPI_FOUND TRUE)
#  SET(MPI_INCLUDE_PATH "/share/apps/mvapich/1.1-2009-12-05/intel/include/")
#  SET(MPI_LIBRARIES "/share/apps/mvapich/1.1-2009-12-05/intel/lib/")
#  SET(MPI_INCLUDE_PATH "/share/apps/openmpi/1.3.3/intel/include/")
#  SET(MPI_LIBRARIES "/share/apps/openmpi/1.3.3/intel/lib/")
set(MPI_INCLUDE_PATH "/share/apps/openmpi/1.4.3/intel/include")
set(MPI_LIBRARIES "/share/apps/openmpi/1.4.3/intel/lib")
set(Boost_INCLUDE_DIRS "/share/apps/boost/1.44.0/intel/all/include")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}) 

ENDIF ($ENV{USEMPI})

IF ($ENV{HPCNYU})
IF ($ENV{USEMAGICKPP})

IF ($ENV{HPCUSQ})
  set(IMVERSION "6.6.2-0")
ELSE ($ENV{HPCUSQ})
  set(IMVERSION "6.6.7-7")
ENDIF ($ENV{HPCUSQ})

  set(Magick++_LIBRARY "Magick++")
  set(Magick++_LIBRARIES_DIR "/share/apps/imagemagick/${IMVERSION}/gnu/lib")
  set(Magick++_INCLUDE_DIR "/share/apps/imagemagick/${IMVERSION}/gnu/include/ImageMagick/")
  set(Magick++_FOUND TRUE)
  include_directories(${Magick++_INCLUDE_DIR})
  link_directories(${Magick++_LIBRARIES_DIR})
  MESSAGE(STATUS "Magick++ includes: ${Magick++_INCLUDE_DIR}")
  MESSAGE(STATUS "Magick++ libraries: ${Magick++_LIBRARIES_DIR}")
  MESSAGE(STATUS "Magick++ library: ${Magick++_LIBRARY}")
  MESSAGE(STATUS "Magick++ Found.")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__MAGICKPP__")  
ENDIF ($ENV{USEMAGICKPP})
ENDIF ($ENV{HPCNYU})
