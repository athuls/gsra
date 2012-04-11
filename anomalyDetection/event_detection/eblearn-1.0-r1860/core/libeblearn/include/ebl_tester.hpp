/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun, Pierre Sermanet, Koray Kavukcuoglu *
 *   yann@cs.nyu.edu, pierre.sermanet@gmail.com, koray@cs.nyu.edu *
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

using namespace std;

namespace ebl {

  template <class T>
  ModuleTester<T>::ModuleTester() {
    this->out = stdout;
    this->acc_thres = 1e-6;
    this->rrange = 2;
    this->jac_fprop = idx<T>(1,1);
    this->jac_bprop = idx<T>(1,1);
    this->jac_fprop_param = idx<T>(1,1);
    this->jac_bprop_param = idx<T>(1,1);

    kk = new idx<T>(100,100);
    dynamic_init_drand();
  }

  template <class T>
  ModuleTester<T>::ModuleTester(FILE* out, T thres, T rrange) {
    this->out = out;
    this->acc_thres = thres;
    this->rrange = rrange;
  }

  template <class T>
  ModuleTester<T>::ModuleTester(T thres, T rrange) {
    this->out = stdout;
    this->acc_thres = thres;
    this->rrange = rrange;
  }

  template <class T>
  ModuleTester<T>::~ModuleTester() {
    delete kk;
  }

  template <class T>
  idx<T> ModuleTester<T>::test_jacobian(module_1_1<T> 
					     &module, bbstate_idx<T> &in, 
					     bbstate_idx<T> &out) {
    forget_param_linear fp(2,0.5);

    // just to resize states
    module.fprop(in,out);
    // randomize parametes if there are any
    module.forget(fp);
    // clear all input and output
    in.clear();
    in.clear_dx();
    in.clear_ddx();
    out.clear();
    out.clear_dx();
    out.clear_ddx();

    // randomize input for fprop
    randomize_idx(in.x);

    get_jacobian_fprop(module,in,out,jac_fprop);
    get_jacobian_bprop(module,in,out,jac_bprop);

    return get_errs(jac_fprop,jac_bprop);
  }

  template <class T>
  void ModuleTester<T>::test_jacobian_param(parameter<bbstate_idx<T> > &p, 
					    module_1_1<T> &module,
					    bbstate_idx<T> &in, bbstate_idx<T> &out)
  {
    forget_param_linear fp(2,0);

    // just to resize states
    module.fprop(in,out);
    // randomize parametes if there are any
    module.forget(fp);
    // clear all input and output
    in.clear();
    in.clear_dx();
    in.clear_ddx();
    out.clear();
    out.clear_dx();
    out.clear_ddx();

    // randomize input for fprop
    randomize_idx(p.x);
    randomize_idx(in.x);

    get_jacobian_fprop_param(p,module,in,out,jac_fprop_param);
    in.clear_dx();
    get_jacobian_bprop_param(p,module,in,out,jac_bprop_param);

    report_err(jac_fprop_param,jac_bprop_param,"jacobian param");
  }

  template <class T>
  void ModuleTester<T>::
  get_jacobian_fprop(module_1_1<T> &module, bbstate_idx<T> &in,
		     bbstate_idx<T> &out, idx<T>& jac) {
    bbstate_idx<T> sina = in.make_copy(); //x-small
    bbstate_idx<T> sinb = in.make_copy(); //x+small
    bbstate_idx<T> souta = out.make_copy(); //f(x-small)
    bbstate_idx<T> soutb = out.make_copy(); //f(x+small)
    double small = 1e-6;
    int cnt = 0;
    // clear out jacobian matrix
    jac.resize(in.size(), out.size());
    idx_clear(jac);
    idx_aloop3(sx, in.x, T, sxa, sina.x, T, sxb, sinb.x, T) {
      idx_copy(in.x, sina.x);
      idx_copy(in.x, sinb.x);
      // perturb
      *sxa = *sx - small;
      *sxb = *sx + small;
      module.fprop(sina, souta);
      module.fprop(sinb, soutb);
      idx_sub(soutb.x, souta.x, soutb.x);
      idx<T> j = jac.select(0, cnt);
      idx_dotc(soutb.x, 1.0 / (2 * small), j);
      cnt++;
    }
  }

  template <class T>
  void ModuleTester<T>::
  get_jacobian_fprop_param(parameter<bbstate_idx<T> > &p, module_1_1<T> &module,
			   bbstate_idx<T> &in, bbstate_idx<T> &out, idx<T>& jac)
  {
    bbstate_idx<T> souta = out.make_copy(); //f(x-small)
    bbstate_idx<T> soutb = out.make_copy(); //f(x+small)
    double small = 1e-6;
    int cnt = 0;
    // clear out jacobian matrix
    idx_clear(jac);
    {
      idx_aloop1(px, p.x, T){
	// perturb
	*px = *px - small;
	module.fprop(in, souta);
	*px = *px + 2*small;
	module.fprop(in, soutb);
	*px = *px - small;
	idx_sub(soutb.x,souta.x,soutb.x);
	idx<T> j = jac.select(0,cnt);
	idx_dotc(soutb.x,1.0/(2*small),j);
	cnt++;
      }
    }
  }

  template <class T>
  void ModuleTester<T>::
  get_jacobian_bprop(module_1_1<T> &module, bbstate_idx<T> &in,
		     bbstate_idx<T> &out, idx<T>& jac) {
    jac.resize(in.size(), out.size());
    idx_clear(jac);
    int cnt = 0;
    idx_aloop1(dx, out.dx,T) {
      idx_clear(out.dx);
      idx_clear(in.dx);
      *dx = 1.0;
      module.bprop(in, out);
      idx<T> j = jac.select(1, cnt);
      idx_copy(in.dx, j);
      cnt++;
    }
  }

  template <class T>
  void ModuleTester<T>::
  get_jacobian_bprop_param(parameter<bbstate_idx<T> > &p, module_1_1<T> &module,
			   bbstate_idx<T> &in, bbstate_idx<T> &out, idx<T>& jac)
  {
  }

  template <class T>
  idx<T> ModuleTester<T>::get_errs(idx<T>& a, idx<T>& b) {
    T maxdist;
    T totdist = idx_sqrdist(a,b);
    // max distance
    idx_sub(a,b,a);
    idx_abs(a,a);
    maxdist = idx_max(a);
    idx<T> errs(2);
    errs.set(maxdist, 0);
    errs.set(totdist, 1);
    return errs;
  }

  template <class T>
  void ModuleTester<T>::report_err(idx<T>& a, idx<T>& b, const char* msg) {
    idx<T> errs = get_errs(a, b);
    stringstream ss(stringstream::in | stringstream::out);
    // report results
    ss << "Max " << msg << " distance";
    fprintf(this->out,"%-40s = %-15g %15s\n",ss.str().c_str(), errs.get(0),
	    ((errs.get(0) < this->acc_thres)?"OK":"NOT OK"));
    ss.str("");
    ss << "Total " << msg << "distance";
    fprintf(this->out,"%-40s = %-15g %15s\n",ss.str().c_str(), errs.get(1),
	    ((errs.get(1) < this->acc_thres)?"OK":"NOT OK"));
    fflush(this->out);
  }

  template <class T>
  void ModuleTester<T>::randomize_idx(idx<T>& m)
  {
    idx_aloop1(v,m,T) {
      *v = drand(this->rrange);
    }
  }

  template <class T>
  T ModuleTester<T>::get_acc_thres() const
  {
    return acc_thres;
  }

  template <class T>
  void ModuleTester<T>::set_acc_thres(T acc_thres)
  {
    this->acc_thres = acc_thres;
  }

  template <class T>
  T ModuleTester<T>::get_rrange() const
  {
    return rrange;
  }

  template <class T>
  void ModuleTester<T>::set_rrange(T rrange)
  {
    this->rrange = rrange;
  }

  template <class T>
  FILE* ModuleTester<T>::get_out() const
  {
    return out;
  }

  template <class T>
  void ModuleTester<T>::set_out(FILE* out)
  {
    this->out = out;
  }


  ////////////////////////////////////////////////////////////////

  template <class T>
  void Jacobian_tester<T>::test(module_1_1<T> &module) {
    int insize = 16;
    bbstate_idx<T> in(insize, 1, 1);
    bbstate_idx<T> out(insize, 1, 1);

    //init
    dseed(2);  // 2 is chosen randomly... feel free to change it
    module.fprop(in, out); // used to resize the outputs
    { idx_bloop1( i, in.x, T)
	{ idx_bloop1 (ii, i, T)
	    { idx_bloop1( iii, ii, T)
		{ iii.set(drand(2)); }
	    }
	}
    }
    { idx_bloop1( o, out.x, T)
	{ idx_bloop1 (oo, o, T)
	    { idx_bloop1( ooo, oo, T)
		{ ooo.set(drand(2)); }
	    }
	}
    }

    // check the Jacobian
    int ndim_in = in.x.nelements();
    int ndim_out = in.x.nelements();
    // used to store the jacobian calculated via bprop
    idx<T> jac_fprop(ndim_in, ndim_out); 
    //  used to store the jacobian calculated via prturbations
    idx<T> jac_bprop(ndim_in, ndim_out); 

    // creation of jac_fprop
    module.fprop(in, out);
    int cnt = 0;
    { idx_bloop1(o, out.x, T)
	{ idx_bloop1(oo, o, T)
	    { idx_bloop1(ooo, oo, T)
		{
		  out.clear_dx();
		  in.clear_dx();
		  ooo.set(1);
		  module.bprop(in, out);
		  idx<T> bla = jac_bprop.select(1, cnt);
		  idx_copy(in.dx, bla);
		  cnt++;
		}
	    }
	}
    }

    // creation of jac_bprop
    cnt = 0;
    double small = pow(10.0, -6);
    bbstate_idx<T> in1(in.x.dim(0), in.x.dim(1), in.x.dim(2));
    bbstate_idx<T> in2(in.x.dim(0), in.x.dim(1), in.x.dim(2));
    bbstate_idx<T> out1(1, 1, 1);
    bbstate_idx<T> out2(1, 1, 1);
    for(int d1 = 0; d1 < in.x.dim(0); d1++){
      for(int d2 = 0; d2 < in.x.dim(1); d2++){
	for(int d3 = 0; d3 < in.x.dim(2); d3++){
	  idx_copy(in.x, in1.x);
	  idx_copy(in.x, in2.x);
	  in1.x.set(in1.x.get( d1, d2, d3) + small, d1, d2, d3);
	  in2.x.set(in2.x.get( d1, d2, d3) - small, d1, d2, d3);
	  module.fprop(in1, out1);
	  module.fprop(in2, out2);
	  idx<T> sub(new srg<T>(), out1.x.spec);
	  idx<T> dot(new srg<T>(), out1.x.spec);
	  idx_sub(out1.x, out2.x, sub);
	  idx_dotc(sub, 0.5/small, dot);
	  idx<T> bla2 = jac_fprop.select(0, cnt);
	  idx_copy(dot, bla2);
	  cnt++;
	}
      }
    }

    // comparison
    printf("Jacobian error: %8.7e\n", idx_sqrdist(jac_fprop, jac_bprop));
  }

  ////////////////////////////////////////////////////////////////////////

  template <class T>
  void Bbprop_tester<T>::test(module_1_1<T> &module){

    int insize = 16;
    bbstate_idx<T> in(insize, 1, 1);
    bbstate_idx<T> out(insize, 1, 1);

    //init
    dseed(2);  // 2 is chosen randomly... feel free to change it
    module.fprop(in, out); // used to resize the outputs
    { idx_bloop1( i, in.x, T)
	{ idx_bloop1 (ii, i, T)
	    { idx_bloop1( iii, ii, T)
		{ iii.set(drand(2)); }
	    }
	}
    }
    { idx_bloop1( o, out.x, T)
	{ idx_bloop1 (oo, o, T)
	    { idx_bloop1( ooo, oo, T)
		{ ooo.set(drand(2)); }
	    }
	}
    }

    module.fprop(in, out);
    module.bprop(in, out);
    module.bbprop(in, out);

    // used to store the bbprop calculated via perturbation
    idx<T> bbprop_p(in.x.dim(0), in.x.dim(1), in.x.dim(2)); 

    // creation of bbprop_p
    int cnt = 0;
    double small = pow(10.0, -6);
    bbstate_idx<T> in1(in.x.dim(0), in.x.dim(1), in.x.dim(2));
    bbstate_idx<T> in2(in.x.dim(0), in.x.dim(1), in.x.dim(2));
    bbstate_idx<T> out1( 1, 1, 1);
    bbstate_idx<T> out2( 1, 1, 1);
    for(int d1 = 0; d1 < in.x.dim(0); d1++){
      for(int d2 = 0; d2 < in.x.dim(1); d2++){
	for(int d3 = 0; d3 < in.x.dim(2); d3++){
	  idx_copy(in.x, in1.x);
	  idx_copy(in.x, in2.x);
	  in1.x.set(in1.x.get( d1, d2, d3) + small, d1, d2, d3);
	  in2.x.set(in2.x.get( d1, d2, d3) - small, d1, d2, d3);
	  module.fprop(in1, out1);
	  module.fprop(in2, out2);
	  // here we calculate a in aX²+bX+c as a model for the 3 points 
	  // calculated via
	  // fprop(...), fprop(...+small) and fprop(...-small). the second 
	  // derivative is then 2*a
	  idx<T> ad(new srg<T>(), out1.x.spec);
	  idx<T> sub(new srg<T>(), out1.x.spec);
	  idx<T> dot(new srg<T>(), out1.x.spec);
	  idx<T> dot2(new srg<T>(), out1.x.spec);
	  idx_add(out1.x, out2.x, ad);
	  idx_dotc(out.x, 2, dot);
	  idx_sub(ad, dot, sub);
	  idx_dotc(sub, 1/small, dot2);
	  bbprop_p.set(dot2.get( d1, d2, d3), d1, d2, d3);
	  cnt++;
	}
      }
    }

    // comparison
    printf("bbprop error: %8.7e \n", idx_sqrdist(in.ddx, bbprop_p));
  }

  ////////////////////////////////////////////////////////////////////////

  template <class T>
  void Bprop_tester<T>::test(module_1_1<T> &module){

    int insize = 16;
    bbstate_idx<T> in(insize, 1, 1);
    bbstate_idx<T> out(insize, 1, 1);

    //init
    dseed(2);  // 2 is chosen randomly... feel free to change it
    module.fprop(in, out); // used to resize the outputs
    { idx_bloop1( i, in.x, T)
	{ idx_bloop1 (ii, i, T)
	    { idx_bloop1( iii, ii, T)
		{ iii.set(drand(2)); }
	    }
	}
    }
    { idx_bloop1( o, out.x, T)
	{ idx_bloop1 (oo, o, T)
	    { idx_bloop1( ooo, oo, T)
		{ ooo.set(drand(2)); }
	    }
	}
    }

    // used to store the bbprop calculated via perturbation
    idx<T> bprop_p(in.x.dim(0), in.x.dim(1), in.x.dim(2)); 

    // creation of bprop_p
    int cnt = 0;
    double small = pow(10.0, -6);
    bbstate_idx<T> in1(in.x.dim(0), in.x.dim(1), in.x.dim(2));
    bbstate_idx<T> in2(in.x.dim(0), in.x.dim(1), in.x.dim(2));
    bbstate_idx<T> out1(1, 1, 1);
    bbstate_idx<T> out2(1, 1, 1);
    for(int d1 = 0; d1 < in.x.dim(0); d1++){
      for(int d2 = 0; d2 < in.x.dim(1); d2++){
	for(int d3 = 0; d3 < in.x.dim(2); d3++){
	  idx_copy(in.x, in1.x);
	  idx_copy(in.x, in2.x);
	  in1.x.set(in1.x.get( d1, d2, d3) + small, d1, d2, d3);
	  in2.x.set(in2.x.get( d1, d2, d3) - small, d1, d2, d3);
	  module.fprop(in1, out1);
	  module.fprop(in2, out2);

	  idx<T> sub(new srg<T>(), out1.x.spec);
	  idx<T> dot(new srg<T>(), out1.x.spec);
	  idx_sub(out1.x, out2.x, sub);
	  idx_dotc(sub, 0.5/small, dot);
	  bprop_p.set(dot.get( d1, d2, d3), d1, d2, d3);
	  cnt++;
	}
      }
    }

    printf("Bprop error : %8.7e \n", idx_sqrdist(in.dx, bprop_p));
  }

} // end namespace ebl
