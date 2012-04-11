#ifndef DEFINES_IDXGUI_H_
#define DEFINES_IDXGUI_H_

#include "defines.h"

#ifdef LIBIDXGUI // we are inside the library
#define IDXGUIEXPORT EXPORT
#else // we are outside
#define IDXGUIEXPORT IMPORT
#endif

#endif /* DEFINES_IDXGUI_H_*/
