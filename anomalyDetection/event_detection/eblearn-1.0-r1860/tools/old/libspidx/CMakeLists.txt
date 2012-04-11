################################################################################
#
# CMake configuration for libspidx project
#
# Author(s):
#   Cyril Poulet, cyril.poulet@centraliens.net, New York University
#
################################################################################

# set include directories
################################################################################
include_directories (include)
include_directories(${LIBIDX_INCLUDE_DIR})
SET(LIBSPIDX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

# compile library
################################################################################
add_library (spidx SHARED
		     src/spBlas.cpp 
		     src/spIdx.cpp    
		     src/libspidx.cpp  
		     )

