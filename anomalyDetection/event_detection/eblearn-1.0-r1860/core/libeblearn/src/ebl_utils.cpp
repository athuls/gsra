/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun and Pierre Sermanet *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com *
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

#ifdef __WINDOWS__
//#include <Winsock2.h>
//#include <windows.h>
#include <time.h>
#else
#include <sys/time.h>
//#include <unistd.h>
#endif

#include "ebl_utils.h"

#ifdef __DUMP_STATES__
uint dump_count = 0;
std::string dump_prefix;
#endif

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // table functions
  
  idx<intg> full_table(intg a, intg b, intg a0, intg b0) {
    if (a <= a0 || b <= b0)
      eblerror("a and b have to be greater than a0 and b0");
    idx<intg> m((a - a0) * (b - b0), 2);
    intg p = 0;
    for (intg j = b0; j < b; ++j) {
      for (intg i = a0; i < a; ++i) {
	m.set(i, p, 0);
	m.set(j, p, 1);
	p++;
      }
    }
    return m;
  }

  idx<intg> one2one_table(intg n) {
    idx<intg> m(n, 2);
    for (intg i = 0; i < n; ++i) {
      m.set(i, i, 0);
      m.set(i, i, 1);
    }
    return m;
  }

  idx<intg> concat_tables(idx<intg> &t0, idx<intg> &t1) {
    if (t0.dim(0) == 0 || t1.dim(0) == 0)
      eblerror("expecting tables dimension 0 to be at least 1, but found: "
	       << t0 << " and " << t1);
    idx<intg> t(t0.dim(0) + t1.dim(0), 2);
    idx<intg> tmp = t.narrow(0, t0.dim(0), 0);
    idx_copy(t0, tmp);
    tmp = t.narrow(0, t1.dim(0), t0.dim(0));
    idx_copy(t1, tmp);
    return t;
  }

  idx<intg> random_table(intg a, intg b, intg fanin, intg a0, intg b0) {
    if (a <= a0 || b <= b0)
      eblerror("a and b have to be greater than a0 and b0");
    idx<intg> tbl((b - b0) * fanin, 2);
    idx<intg> fi(a - a0);
    intg frcntr = 0, tocntr = b0;
    intg nfi = (a - a0) / fanin; // number of distinct fanin chunks
    //intg rfi = (a - a0) % fanin; // number of remaining a maps
    idx<intg> totbl = tbl.select(1, 1);
    idx<intg> frtbl = tbl.select(1, 0);
    // part of fi that covers distinct chunks
    idx<intg> fitbl = fi.narrow(0, nfi * fanin, 0);
    idx<intg> ufrtbl = frtbl.unfold(0, fanin, fanin);
    idx<intg> utotbl = totbl.unfold(0, fanin, fanin);
    idx<intg> ufitbl = fitbl.unfold(0, fanin, fanin);
    // fill fi with 1..n indicices
    for (intg i = a0; i < a; ++i)
      fi.set(i % a, i - a0);
    // create a random permutation of these indices
    idx_shuffle(fi, 0);
    // start filling frtbl
    for (intg i = b0; i < b; ++i) { // fro each unit in target map
      idx<intg> tmp = ufitbl.select(0, frcntr);
      idx<intg> tmp2 = ufrtbl.select(0, i - b0);
      idx_copy(tmp, tmp2);
      frcntr++;
      if (frcntr == nfi) { // reset fi
	idx_shuffle(fi, 0);
	frcntr = 0;
      }
    }
    idx_bloop1(to, utotbl, intg) {
      idx_fill(to, tocntr);
      tocntr++;
    }
    return tbl;
  }

  idx<intg> yuv_table0(intg yend, intg uend, intg vend) {
    // table 0
    idx<intg> table0;
    if (yend > 0) {
      idx<intg> t0 = full_table(1, yend); // Y channel
      idx<intg> t1 = full_table(2, uend, 1, yend); // U channel
      table0 = concat_tables(t0, t1);
    } else // ignore y channel
      table0 = full_table(2, uend, 1, yend);
    // V channel
    idx<intg> t2 = full_table(3, vend, 2, uend);
    return concat_tables(table0, t2);
  }
  
  idx<intg> uv_table0(intg uend, intg vend) {
    idx<intg> t1 = full_table(1, uend); // U channel
    idx<intg> t2 = full_table(2, vend, 1, uend);
    return concat_tables(t1, t2);
  }
  
  idx<intg> yuv_table1(intg yend, intg uend, intg vend,
		       intg p0, intg p1, intg p2, intg fanin_y,
		       intg fanin_yuv, intg fanin_uv) {
    dynamic_init_drand();
    if (fanin_y == 0 && fanin_yuv == 0 && fanin_uv == 0)
      eblerror("at least 1 fanin variable must be non zero");
    // table 1
    idx<intg> t3, t4, t5, table1;
    bool t1_defined = false;
    if (fanin_y != 0)
      t3 = random_table(yend, p0, fanin_y);
    if (fanin_yuv != 0)
      t4 = random_table(vend, p1, fanin_yuv, 0, p0);
    if (fanin_uv != 0)
      t5 = random_table(vend, p2, fanin_uv, yend, p1);
    if (fanin_y != 0) {
      table1 = t3;
      t1_defined = true;
    }
    if (fanin_yuv != 0) {
      if (t1_defined)
	table1 = concat_tables(table1, t4);
      else {
	table1 = t4;
	t1_defined = true;
      }
    }
    if (fanin_uv != 0) {
      if (t1_defined)
	table1 = concat_tables(table1, t5);
      else {
	table1 = t5;
	t1_defined = true;
      }
    }
    return table1;
  }
  
} // end namespace ebl
