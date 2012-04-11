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

// tell header that we are in the libidx scope
#define LIBIDX

#ifndef __WINDOWS__
#include <inttypes.h>
#endif

#include "image.h"
#include "idxops.h"
#include "idx.h"
#include "stl.h"

using namespace std;

namespace ebl {

  bool collide_rect(int x1, int y1, int w1, int h1,
		    int x2, int y2, int w2, int h2) {
    int x,y,w,h;
    x = max(x1,x2);
    y = max(y1,y2);
    w = min(x1+w1,x2+w2)-x;
    h = min(y1+h1,y2+h2)-y;
    if (w>0 && h>0)
      return true;
    else
      return false;
  }

  double common_area(int x1, int y1, int w1, int h1,
		     int x2, int y2, int w2, int h2) {
    int x,y,w,h;
    x = max(x1,x2);
    y = max(y1,y2);
    w = min(x1+w1,x2+w2)-x;
    h = min(y1+h1,y2+h2)-y;
    if((w <= 0)||(h <= 0)) return 0;
    else return (double)((w*h)/(w1*h1));

  }

  ////////////////////////////////////////////////////////////////
  // interpolation

  void image_interpolate_bilin(ubyte* background, ubyte *pin,
			       int indimi, int indimj, int inmodi, int inmodj,
			       int ppi, int ppj,
			       ubyte* out, int outsize) {
    int li0, lj0;
    register int li1, lj1;
    int deltai, ndeltai;
    int deltaj, ndeltaj;
    register ubyte *pin00;
    register ubyte *v00, *v01, *v10, *v11;
    li0 = ppi >> 16;
    li1 = li0+1;
    deltai = ppi & 0x0000ffff;
    ndeltai = 0x00010000 - deltai;
    lj0 = ppj  >> 16;
    lj1 = lj0+1;
    deltaj = ppj & 0x0000ffff;
    ndeltaj = 0x00010000 - deltaj;
    pin00 = (ubyte*)(pin) + inmodi * li0 + inmodj * lj0;
    if ((li1>0)&&(li1<indimi)) {
      if ((lj1>0)&&(lj1<indimj)) {
	v00 = (pin00);
	v01 = (pin00+inmodj);
	v11 = (pin00+inmodi+inmodj);
	v10 = (pin00+inmodi);
      } else if (lj1==0) {
	v00 = background;
	v01 = (pin00+inmodj);
	v11 = (pin00+inmodi+inmodj);
	v10 = background;
      } else if (lj1==indimj) {
	v00 = (pin00);
	v01 = background;
	v11 = background;
	v10 = (pin00+inmodi);
      } else {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = background;
      }
    } else if (li1==0) {
      if ((lj1>0)&&(lj1<indimj)) {
	v00 = background;
	v01 = background;
	v11 = (pin00+inmodi+inmodj);
	v10 = (pin00+inmodi);
      } else if (lj1==0) {
	v00 = background;
	v01 = background;
	v11 = (pin00+inmodi+inmodj);
	v10 = background;
      } else if (lj1==indimj) {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = (pin00+inmodi);
      } else {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = background;
      }
    } else if (li1==indimi) {
      if ((lj1>0)&&(lj1<indimj)) {
	v00 = (pin00);
	v01 = (pin00+inmodj);
	v11 = background;
	v10 = background;
      } else if (lj1==0) {
	v00 = background;
	v01 = (pin00+inmodj);
	v11 = background;
	v10 = background;
      } else if (lj1==indimj) {
	v00 = (pin00);
	v01 = background;
	v11 = background;
	v10 = background;
      } else {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = background;
      }
    } else {
      v00 = background;
      v01 = background;
      v11 = background;
      v10 = background;
    }
    if (outsize >= 1)
      *out = (ndeltaj * (( *v10*deltai + *v00*ndeltai )>>16) +
	      deltaj  * (( *v11*deltai + *v01*ndeltai )>>16))>>16;
    if (outsize >= 2)
      *(out + 1) = (ndeltaj * (( v10[1]*deltai + v00[1]*ndeltai )>>16) +
		    deltaj  * (( v11[1]*deltai + v01[1]*ndeltai )>>16))>>16;
    if (outsize >= 3)
      *(out + 2) = (ndeltaj * (( v10[2]*deltai + v00[2]*ndeltai )>>16) +
		    deltaj  * (( v11[2]*deltai + v01[2]*ndeltai )>>16))>>16;
    if (outsize >= 4)
      *(out + 3) = (ndeltaj * (( v10[3]*deltai + v00[3]*ndeltai )>>16) +
		    deltaj  * (( v11[3]*deltai + v01[3]*ndeltai )>>16))>>16;
    if (outsize >= 5) {
      eblerror("not implemented for more than 4 channels ("
	       << outsize << " channels in " << indimi << "x" << indimj
	       << " image)");
    }
  }

  void image_interpolate_bilin(float* background, float *pin,
			       int indimi, int indimj,
			       int inmodi, int inmodj,
			       int ppi, int ppj,
			       float* out, int outsize) {
    int li0, lj0;
    register int li1, lj1;
    float deltai, ndeltai;
    float deltaj, ndeltaj;
    register float *pin00;
    register float *v00, *v01, *v10, *v11;
    li0 = ppi >> 16;
    li1 = li0+1;
    deltai = 0.0000152587890625 * (float)(ppi & 0x0000ffff);
    ndeltai = 1.0 - deltai;
    lj0 = ppj  >> 16;
    lj1 = lj0+1;
    deltaj = 0.0000152587890625 * (float)(ppj & 0x0000ffff);
    ndeltaj = 1.0 - deltaj;
    pin00 = (float*)(pin) + inmodi * li0 + inmodj * lj0;
    if ((li1>0)&&(li1<indimi)) {
      if ((lj1>0)&&(lj1<indimj)) {
	v00 = (pin00);
	v01 = (pin00+inmodj);
	v11 = (pin00+inmodi+inmodj);
	v10 = (pin00+inmodi);
      } else if (lj1==0) {
	v00 = background;
	v01 = (pin00+inmodj);
	v11 = (pin00+inmodi+inmodj);
	v10 = background;
      } else if (lj1==indimj) {
	v00 = (pin00);
	v01 = background;
	v11 = background;
	v10 = (pin00+inmodi);
      } else {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = background;
      }
    } else if (li1==0) {
      if ((lj1>0)&&(lj1<indimj)) {
	v00 = background;
	v01 = background;
	v11 = (pin00+inmodi+inmodj);
	v10 = (pin00+inmodi);
      } else if (lj1==0) {
	v00 = background;
	v01 = background;
	v11 = (pin00+inmodi+inmodj);
	v10 = background;
      } else if (lj1==indimj) {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = (pin00+inmodi);
      } else {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = background;
      }
    } else if (li1==indimi) {
      if ((lj1>0)&&(lj1<indimj)) {
	v00 = (pin00);
	v01 = (pin00+inmodj);
	v11 = background;
	v10 = background;
      } else if (lj1==0) {
	v00 = background;
	v01 = (pin00+inmodj);
	v11 = background;
	v10 = background;
      } else if (lj1==indimj) {
	v00 = (pin00);
	v01 = background;
	v11 = background;
	v10 = background;
      } else {
	v00 = background;
	v01 = background;
	v11 = background;
	v10 = background;
      }
    } else {
      v00 = background;
      v01 = background;
      v11 = background;
      v10 = background;
    }
    if (outsize >= 1)
      *out = ndeltaj * (*v10*deltai + *v00*ndeltai) +
	     deltaj  * (*v11*deltai + *v01*ndeltai);
    if (outsize >= 2)
      *(out + 1) = ndeltaj * (v10[1]*deltai + v00[1]*ndeltai) +
	           deltaj  * (v11[1]*deltai + v01[1]*ndeltai);
    if (outsize >= 3)
      *(out + 2) = ndeltaj * (v10[2]*deltai + v00[2]*ndeltai) +
		   deltaj  * (v11[2]*deltai + v01[2]*ndeltai);
    if (outsize >= 4)
      *(out + 3) = ndeltaj * (v10[3]*deltai + v00[3]*ndeltai) +
		   deltaj  * (v11[3]*deltai + v01[3]*ndeltai);
    if (outsize >= 5) {
      eblerror("not implemented for more than 4 channels ("
	       << outsize << " channels in " << indimi << "x" << indimj
	       << " image)");
    }
  }

  /////////////////////////////////////////////////////////////////

  void image_rotscale_rect(int w, int h, double cx, double cy,
			   double angle, double coeff, idx<intg> &wh,
			   idx<double> &cxcy){
    double sa = sin(0.017453292*angle);
    double ca = cos(0.017453292*angle);
    double x1 = 0;
    double y1 = 0;
    double cw = coeff * w;
    double ch = coeff * h;
    double x2 = cw * ca;
    double y2 = cw * sa;
    double x4 = -(ch * sa);
    double y4 = ch * ca;
    double x3 = x2 + x4;
    double y3 = y2 + y4;
    double dcx = coeff * ( cx*ca - cy*sa);
    double dcy = coeff * ( cx*sa + cy*ca);
    double lx = min(min(x1, x2), min(x3, x4));
    double ly = min(min(y1, y2), min(y3, y4));
    double rx = max(max(x1, x2), max(x3, x4));
    double ry = max(max(y1, y2), max(y3, y4));
    wh.set((intg)(1 + rx - lx), 0);
    wh.set((intg)(1 + ry - ly), 1);
    cxcy.set(dcx - lx, 0);
    cxcy.set(dcy - ly, 1);
  }

} // end namespace ebl
