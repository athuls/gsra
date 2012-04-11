/***************************************************************************
 *   Copyright (C) 2010 by Pierre Sermanet *
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

#include <jni.h>
#include <android/bitmap.h>

#include "libidx.h"
#include "libeblearn.h"

using namespace ebl;

extern "C" {

  typedef float t_net;
  
  JNIEXPORT jint JNICALL
  Java_com_eblearn_eblearn_detect(JNIEnv* env, jobject obj, jobject bitmap,
				  jobject fd_sys1, jlong off1,
				  jobject fd_sys2, jlong off2,
				  jobject fd_sys3, jlong off3, jobject jbb,
				  jfloat threshold) {
    ////////////////////////////////////////////////////////////////////////////
    // read weight matrix from resources
    idx<t_net> weights;
    idx<ubyte> classnames;
    //    configuration conf;
    
    if (!fd_sys1 || !fd_sys1 || !fd_sys1) {
      cerr << "null file descriptor: " << fd_sys1 << " " << fd_sys2
	   << " " << fd_sys3 << endl;
      return -1;
    }
    jclass fdClass = env->FindClass("java/io/FileDescriptor");
    if (fdClass) { 
      jfieldID fid = env->GetFieldID(fdClass, "descriptor", "I");
      if (fid) {
	jint fd1 = env->GetIntField(fd_sys1, fid),
	  fd2 = env->GetIntField(fd_sys2, fid),
	  fd3 = env->GetIntField(fd_sys3, fid);
	int dupfd1 = dup(fd1), dupfd2 = dup(fd2), dupfd3 = dup(fd3);
	FILE* fp;
	// weights
	fp = fdopen(fd1, "rb"); 
	if (!fp) {
	  cerr << "Could not open resource file descriptor " << fp << endl;
	  return -1; }
	fseek(fp, off1, SEEK_SET);
	weights = load_matrix<t_net>(fp);
	cout << "Loaded weight matrix: " << weights
	     << " (min: " << idx_min(weights)
	     << ", max: " << idx_max(weights) << ")" << endl;
	// class names
	fp = fdopen(fd2, "rb"); 
	if (!fp) {
	  cerr << "Could not open resource file descriptor " << fp << endl;
	  return -1; }
	fseek(fp, off2, SEEK_SET);
	classnames = load_matrix<ubyte>(fp);
	cout << "Loaded class names matrix: " << classnames << endl;
	// // configuration
	// fp = fdopen(fd3, "rb"); 
	// if (!fp) { LOGI("Could not open resource file descriptor " << fp);
	//   return -1; }
	// fseek(fp, off3, SEEK_SET);
	// weights = load_matrix<float>(fp);
	// LOGI("Loaded weight matrix: " << weights);
      } 
    }
    ////////////////////////////////////////////////////////////////////////////
    // read image
    AndroidBitmapInfo info;
    void *pixels;
    int ret;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
      cerr << "AndroidBitmap_getInfo() failed ! error=" << ret << endl;
      return -1; }
    cout << "Image height " << info.height << " width " << info.width
	 << " format: " << info.format << " stride: " << info.stride << endl;
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
      LOGE("Bitmap format is not ARGB_8888 !");
      return -1;
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
      cerr << "AndroidBitmap_lockPixels() failed ! error=" << ret << endl;
      return -1; }
    typedef ubyte intype;
    idx<ubyte> im(info.height, info.width, 3);
    ubyte *ptr = (ubyte*)im.idx_ptr();
    intype *p = (intype*) pixels, v;
    for (size_t i = 0; i < info.height * info.width; ++i) {
      *(ptr++) = *(p++); // r
      *(ptr++) = *(p++); // g
      *(ptr++) = *(p++); // b
      p++; // a
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    cout << "Created idx image " << im << " (range: " << (int) idx_min(im)
	 << ", " << (int) idx_max(im) << ")" << endl;
    // string fname = "/sdcard/nens.mat";
    // im = load_matrix<ubyte>(fname);
    //      eblerror("failed to load " << fname);
    
    ////////////////////////////////////////////////////////////////////////////
    // start detection
    uint norm_size = 7, input_max = 300;
    float scaling = 1.3, min_scale = 1.0, max_scale = 1.0,
      bbhfactor = 1, bbwfactor = 1, bbh_overlap = .67, bbw_overlap = 0,
      hzpad = .5, wzpad = .5;
    uint net_ih = 32, net_iw = 32, net_c1h = 5, net_c1w = 5,
      net_s1h = 2, net_s1w = 2, net_c2h = 5, net_c2w = 5,
      net_s2h = 2, net_s2w = 2, noutputs = classnames.dim(0);
    bool absnorm = true, color = false, mirror = false, use_tanh = true,
      use_shrink = false, pruning = true;
    parameter<t_net, fstate_idx<t_net> > theparam;
    // build net
    lenet_cscsc<t_net, fstate_idx<t_net> >
      net(theparam, net_ih, net_iw, net_c1h, net_c1w, net_s1h, net_s1w, net_c2h,
	  net_c2w, net_s2h, net_s2w, noutputs, absnorm, color, mirror, use_tanh,
	  use_shrink);
    // load net
    theparam.load_x(weights);
    // build preprocessing
    module_1_1<t_net, fstate_idx<t_net> >* pp = NULL;
    if (im.dim(2) == 1) // grayscale input
      pp = new y_to_yp_module<t_net, fstate_idx<t_net> >(norm_size);
    else if (im.dim(2) == 3) // color input
      pp = new rgb_to_yp_module<t_net, fstate_idx<t_net> >(norm_size);
    else {
      cerr << "image format not supported: " << im << endl;
      return -1;
    }
    // build detector
    detector<t_net, fstate_idx<t_net> > detect(net, classnames, pp, norm_size);
    detect.set_resolutions(scaling, max_scale, min_scale);
    fstate_idx<t_net> input(1,1,1), output(1,1,1);
    detect.set_mem_optimization(input, output);
    detect.set_max_resolution(input_max);
    detect.set_pruning(pruning);
    detect.set_bbox_factors(bbhfactor, bbwfactor);
    detect.set_bbox_overlaps(bbh_overlap, bbw_overlap);
    detect.set_zpads(hzpad, wzpad);
    detect.set_silent();
    // detection
    cout << "Detection threshold: " << threshold << endl;
    vector<bbox*> &bb = detect.fprop(im, threshold);
    cout << "Detection finished" << endl;
    detect.pretty_bboxes_short(bb);

    jclass cls = env->GetObjectClass(jbb);
    jmethodID mid = env->GetMethodID(cls, "add", "(FIIII)V");
    if (mid == 0) {
      eblerror("failed to get add method from JNI");
      return -1;
    }
    bbox *b;
    for (size_t i = 0; i < bb.size(); ++i) {
      b = bb[i];
      env->CallVoidMethod(jbb, mid, b->confidence, b->h0, b->w0, b->height,
			  b->width);
    }
    
    return (jint) bb.size();
  }

}

