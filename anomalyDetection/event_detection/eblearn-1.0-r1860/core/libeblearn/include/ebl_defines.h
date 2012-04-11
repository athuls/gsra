/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com *
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Redistribution under a license not approved by the Open Source
 *       Initiative (http://www.opensource.org) must display the
 *       following acknowledgement in all advertising material:
 *        This product includes software developed at the Courant
 *        Institute of Mathematical Sciences (http://cims.nyu.edu).
 *     * The names of the authors may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ThE AUTHORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#ifndef EBL_DEFINES_H
#define EBL_DEFINES_H

#include "defines.h"

#ifdef __DUMP_STATES__

#ifdef __ANDROID__
#define DUMP_ROOT "/sdcard/"
#else
#define DUMP_ROOT "";
#endif

#include "stl.h"

extern uint dump_count;
extern std::string dump_prefix;

#define RESET_DUMP() dump_count = 0;
#define DUMP_PREFIX(s) { dump_prefix.clear(); dump_prefix << s << "_"; }

#define DUMP(mat, fname) {						\
    string n = DUMP_ROOT;						\
    n << dump_prefix << (dump_count < 10? "0":"") << dump_count++ << "_" \
      << fname << "_" << mat << ".mat";					\
    if (save_matrix(mat, n))						\
      cout << "Dumped " << n << " (min: " << idx_min(mat)		\
	   << " max: " << idx_max(mat) << ")" <<endl;			\
    else								\
      cerr << "Failed to dump " << n << endl;				\
  }

#endif /* DUMP_STATES */

#define err_not_implemented() {						\
    eblerror("member function not implemented for this class"); }

//! see numerics.h for description
extern bool drand_ini;

#define check_drand_ini() {					      \
    if (!drand_ini) printf("You have not initialized random sequence. \
Please call init_drand(time(NULL)) before using this function !\n"); }

#endif /* EBL_DEFINES */
