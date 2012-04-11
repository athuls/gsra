/***************************************************************************
 *   Copyright (C) 2008 by Pierre Sermanet *
 *   pierre.sermanet@gmail.com *
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

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include <math.h>

namespace ebl {
  
  ////////////////////////////////////////////////////////////////
  // rect

  template <typename T>
  rect<T>::rect(T h0_, T w0_, T height_, T width_)
    : h0(h0_), w0(w0_), height(height_), width(width_) {
  }
  
  template <typename T>
  rect<T>::rect() : h0(0), w0(0), height(0), width(0) {
  }
  
  template <typename T>
  rect<T>::rect(const rect<T> &r)
  : h0(r.h0), w0(r.w0), height(r.height), width(r.width) {
  }

  template <typename T>
  rect<T>::~rect() {
  }

  template <typename T>
  T rect<T>::h1() const {
    return h0 + height;
  }

  template <typename T>
  T rect<T>::w1() const {
    return w0 + width;
  }

  template <typename T>
  float rect<T>::hcenter() const {
    return h0 + height / (float) 2.0;
  }

  template <typename T>
  float rect<T>::wcenter() const {
    return w0 + width / (float) 2.0;
  }

  template <typename T>
  T rect<T>::area() const {
    return height * width;
  }
  
  template <typename T>
  float rect<T>::center_distance(const rect<T> &r) const {
    float a = r.hcenter() - hcenter();
    float b = r.wcenter() - wcenter();
    return sqrt(a * a + b * b);
  }

  template <typename T>
  float rect<T>::center_hdistance(const rect<T> &r) const {
    return fabs(hcenter() - r.hcenter()) / height;
  }

  template <typename T>
  float rect<T>::center_wdistance(const rect<T> &r) const {
    return fabs(wcenter() - r.wcenter()) / width;
  }

  template <typename T>
  T rect<T>::intersection_area(const rect<T> &r) const {
    // check height intersection exists
    T nh0 = (std::max)(h0, r.h0);
    T nh1 = (std::min)(h0 + height, r.h0 + r.height);
    if (nh0 >= nh1) // empty intersection
      return (T) 0;
    // check width intersection exists
    T nw0 = (std::max)(w0, r.w0);
    T nw1 = (std::min)(w0 + width, r.w0 + r.width);
    if (nw0 >= nw1) // empty intersection
      return (T) 0;
    // intersection exists
    return (nh1 - nh0)  * (nw1 - nw0);
  }
  
  template <typename T>
  T rect<T>::union_area(const rect<T> &r) const {
    return area() + r.area() - intersection_area(r);
  }
  
  template <typename T>
  float rect<T>::match(const rect<T> &r) const {
    return intersection_area(r) / (float) union_area(r);
  }

  template <typename T>
  float rect<T>::min_match(const rect<T> &r) {
    float a1 = (float) area(), a2 = (float) r.area();
    return intersection_area(r) / std::min(a1, a2);
  }

  template <typename T>
  bool rect<T>::overlap(const rect<T> &r) const {
    if (((h0 <= r.h0 + r.height) && (h0 + height >= r.h0)) &&
	((w0 <= r.w0 + r.width) && (w0 + width >= r.w0)))
      return true;
    return false;
  }

  template <typename T>
  float rect<T>::overlap_ratio(const rect<T> &r) const {
    T area1 = height * width;
    T inter = intersection_area(r);
    return inter / (float) area1;
  }
  
  template <typename T>
  bool rect<T>::min_overlap(const rect<T> &r, float minarea) const {
    T area1 = height * width;
    T area2 = r.height * r.width;
    T inter = intersection_area(r);
    if ((inter / (float) (std::min)(area1, area2)) > minarea)
      return true;
    return false;
  }
  
  template <typename T>
  float rect<T>::min_overlap(const rect<T> &r) const {
    T area1 = height * width;
    T area2 = r.height * r.width;
    T inter = intersection_area(r);
    return inter / (float) (std::min)(area1, area2);
  }
  
  template <typename T>
  bool rect<T>::min_overlap(const rect<T> &r, float hmin, float wmin) const {
    if (((h0 <= r.h0 + r.height) && (h0 + height >= r.h0)) &&
	((w0 <= r.w0 + r.width) && (w0 + width >= r.w0))) {
      // there is overlap, now check how much is authorized.
      T hoverlap = std::min(h0 + height, r.h0 + r.height)
	- std::min(h0, r.h0);
      T woverlap = std::min(w0 + width, r.w0 + r.width) - std::max(w0, r.w0);
      float hratio = hoverlap / (float) std::min(height, r.height);
      float wratio = woverlap / (float) std::min(width, r.width);
      if (hratio >= hmin || wratio >= wmin)
	return true;
    }
    return false;
  }
  
  template <typename T>
  bool rect<T>::is_within(const rect<T> &r) {
    if (((h0 >= r.h0) && (h0 + height <= r.h0 + r.height)) &&
	((w0 >= r.w0) && (w0 + width <= r.w0 + r.width)))
      return true;
    return false;
  }
  
  template <typename T>
  void rect<T>::scale_centered(float sh, float sw) {
    T addh = (T) (height * ((float)sh - (float)1.0));
    T addw = (T) (width * ((float)sw - (float)1.0));
    h0 -= (T) (addh / (float)2);
    w0 -= (T) (addw / (float)2);
    height += addh;
    width += addw;
  }
  
  template <typename T>
  void rect<T>::scale_width(float woverh) {
    float addw = height * woverh - width;
    w0 -= (T) addw/2;
    width += (T) addw;
  }

  template <typename T>
  float rect<T>::radius() {
    float a = h0 - hcenter();
    float b = w0 - wcenter();
    return sqrt(a * a + b * b);
  }
  
  template <typename T>
  std::string& operator<<(std::string& out, rect<T>& r) {
    out << "rect:<(" << r.h0 << "," << r.w0 << ")," << r.height;
    out << "x" << r.width << ">";
    return out;
  }

  template <typename T>
  std::string& operator<<(std::string& out, const rect<T>& r) {
    out << "rect:<(" << r.h0 << "," << r.w0 << ")," << r.height;
    out << "x" << r.width << ">";
    return out;
  }

  template <typename T>
  std::ostream& operator<<(std::ostream& out, rect<T>& r) {
    out << "rect:<(" << r.h0 << "," << r.w0 << ")," << r.height;
    out << "x" << r.width << ">";
    return out;
  }

  template <typename T>
  std::ostream& operator<<(std::ostream& out, const rect<T>& r) {
    out << "rect:<(" << r.h0 << "," << r.w0 << ")," << r.height;
    out << "x" << r.width << ">";
    return out;
  }

  template <typename T>
  rect<T> rect<T>::operator/(double d) {
    rect<T> newr((T) (h0 / d), (T) (w0 / d),
	      (T) (height / d), (T) (width / d));
    return newr;
  }

  template <typename T>
  rect<T> rect<T>::operator*(double d) {
    rect<T> newr((T) (h0 * d), (T) (w0 * d),
	      (T) (height * d), (T) (width * d));
    return newr;
  }

  template <typename T> template <typename T2>
  rect<T> rect<T>::operator=(rect<T2> &r2) {
    h0 = (T) r2.h0;
    w0 = (T) r2.w0;
    height = (T) r2.height;
    width = (T) r2.width;
    return *this;
  }

  template <typename T>
  void rect<T>::shift(T h, T w) {
    h0 += h;
    w0 += w;
  }

} // end namespace ebl

#endif /* GEOMETRY_HPP_ */
