################################################################################
#
# CMake configuration for sp_doc_classifier project
#
# Author(s):
#   Cyril Poulet, cyril.poulet@centraliens.net, New York University
#
################################################################################

include_directories (include)
include_directories(${LIBIDX_INCLUDE_DIR})
include_directories(${LIBSPIDX_INCLUDE_DIR})
include_directories(${LIBEBLEARN_INCLUDE_DIR})

# compile executable
################################################################################
add_executable (sp_doc_classifier
  src/gzstream.cpp
  src/preprocess.cpp
  src/spEbm.cpp
  src/spex.cpp
  src/vectors.cpp
  src/main.cpp
  )

# link executable with external libraries
################################################################################
target_link_libraries (sp_doc_classifier idx spidx)
target_link_libraries (sp_doc_classifier eblearn)
target_link_libraries (sp_doc_classifier z)

