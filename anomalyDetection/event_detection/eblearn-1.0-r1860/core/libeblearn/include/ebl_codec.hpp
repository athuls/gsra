/***************************************************************************
 *   Copyright (C) 2008 by Yann LeCun, Pierre Sermanet *
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

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // codec

  template <class T>
  codec<T>::codec(module_1_1<T>	&encoder_,
		  ebm_2<T>	&enc_cost_,
		  double	weight_energy_enc_,
		  ebm_1<T>	&z_cost_,
		  double	weight_energy_z_,
		  module_1_1<T>	&decoder_,
		  ebm_2<T>	&dec_cost_,
		  double	weight_energy_dec_,
		  gd_param      &infp_)
    : encoder(encoder_), enc_out(1, 1, 1), enc_cost(enc_cost_),
      weight_energy_enc(weight_energy_enc_), enc_energy(),
      z(1, 1, 1), z_cost(z_cost_),
      weight_energy_z(weight_energy_z_), z_energy(),
      decoder(decoder_), dec_out(1, 1, 1), dec_cost(dec_cost_),
      weight_energy_dec(weight_energy_dec_), dec_energy(),
      infp(infp_) {
  }

  template <class T>
  codec<T>::~codec() {
  }

  template <class T>
  void codec<T>::fprop(fstate_idx<T> &in1, fstate_idx<T> &in2,
		       fstate_idx<T> &energy) {
    // initialize z with a simple one-pass fprop through whole machine
    fprop_one_pass(in1, in2, energy);
  }
  
  template <class T>
  void codec<T>::bprop(fstate_idx<T> &in1, fstate_idx<T> &in2,
		       fstate_idx<T> &energy) {
    // do gradient descent to find optimal code z
    bprop_optimal_code(in1, in2, energy, infp);
    // bprop through all modules
    bprop_one_pass(in1, in2, energy);
  }

  template <class T>
  void codec<T>::bbprop(fstate_idx<T> &in1, fstate_idx<T> &in2,
			fstate_idx<T> &energy) {
    enc_out.clear_ddx();
    dec_out.clear_ddx();
    // initialize all energy 2nd derivatives with global energy derivative
    // so that we minimize the global cost function
    idx_dotc(energy.ddx, weight_energy_dec, dec_energy.ddx);
    idx_dotc(energy.ddx, weight_energy_enc, enc_energy.ddx);
    idx_dotc(energy.ddx, weight_energy_z, z_energy.ddx);
    // bprop through cost modules
    z_cost.bbprop(z, z_energy);
    enc_cost.bbprop(enc_out, z, enc_energy);
    dec_cost.bbprop(dec_out, in2, dec_energy);
    // bprop through encoder/decoder
    decoder.bbprop(z, dec_out);
    encoder.bbprop(in1, enc_out);
  }

  template <class T>
  void codec<T>::forget(forget_param_linear &fp) {
    encoder.forget(fp);
    decoder.forget(fp);
    enc_cost.forget(fp);
    dec_cost.forget(fp);
    z_cost.forget(fp);
    normalize();
  }

  template <class T>
  void codec<T>::normalize() {
    decoder.normalize();
  }

  // simple one-pass forward propagation
  template <class T>
  void codec<T>::fprop_one_pass(fstate_idx<T> &in1, fstate_idx<T> &in2, 
				fstate_idx<T> &energy) {
    encoder.fprop(in1, enc_out);
    // let the enc-cost produce its best guess
    // for what the code should be. If the cost
    // is an l2-distance, this will simply 
    // copy :enc-out:x into :z:x
    // There is no need to do an fprop of
    // the encoder afterward, because infer2 does it.
    enc_cost.infer2_copy(enc_out, z, enc_energy);
    // compute cost penalty
    z_cost.fprop(z, z_energy);
    // fprop through decoder
    decoder.fprop(z, dec_out);
    // fprop through decoder cost.
    dec_cost.fprop(dec_out, in2, dec_energy);
    // add up energy terms
    energy.clear();
    idx_dotcacc(enc_energy.x, weight_energy_enc, energy.x);
    idx_dotcacc(dec_energy.x, weight_energy_dec, energy.x);
    idx_dotcacc(z_energy.x, weight_energy_z, energy.x);
  }
  
  // simple one-pass backward propagation
  template <class T>
  void codec<T>::bprop_one_pass(fstate_idx<T> &in1, fstate_idx<T> &in2, 
				fstate_idx<T> &energy) {
    enc_out.clear_dx();
    dec_out.clear_dx();
    // initialize all energy derivatives with global energy derivative
    // so that we minimize the global cost function
    idx_dotc(energy.dx, weight_energy_dec, dec_energy.dx);
    idx_dotc(energy.dx, weight_energy_enc, enc_energy.dx);
    idx_dotc(energy.dx, weight_energy_z, z_energy.dx);
    // bprop through cost modules
    z_cost.bprop(z, z_energy);
    enc_cost.bprop(enc_out, z, enc_energy);
    dec_cost.bprop(dec_out, in2, dec_energy);
    // bprop through encoder/decoder
    decoder.bprop(z, dec_out);
    encoder.bprop(in1, enc_out);
  }
  
  // multiple-pass bprop on the decoder only to find the optimal code z
  template <class T>
  void codec<T>::bprop_optimal_code(fstate_idx<T> &in1, fstate_idx<T> &in2, 
				    fstate_idx<T> &energy, gd_param &infp) {
    z.clear_dx();
    bprop(in1, in2, energy); // bprop once to initialize energy
    gd_param temp_ip(infp.eta, 0, 0, 0, 0, 0, 0, 0, 0); 
    double old_energy = energy.x.get() + 1;
    int cnt = 0;
    while ((cnt < infp.n)
	   && check_code_threshold(z, infp)
	   && (old_energy > energy.x.get())) {
      old_energy = energy.x.get();
      z.clear_dx();
      // bprop through decoder /////////////////////////////////////
      dec_out.clear_dx();
      enc_out.clear_dx();
      idx_dotc(energy.dx, weight_energy_dec, dec_energy.dx);
      idx_dotc(energy.dx, weight_energy_z, z_energy.dx);
      z_cost.bprop(z, z_energy);
      enc_cost.bprop(enc_out, z, enc_energy);
      dec_cost.bprop(dec_out, in2, dec_energy);
      decoder.bprop(z, dec_out);
      z.update_gd(temp_ip);
      // now fprop through decoder /////////////////////////////////
      decoder.fprop(z, dec_out);
      z_cost.fprop(z, z_energy);
      enc_cost.fprop(enc_out, z, enc_energy);
      dec_cost.fprop(dec_out, in2, dec_energy);
      // add up energy terms ///////////////////////////////////////
      idx_dotcacc(enc_energy.x, weight_energy_enc, energy.x);
      idx_dotcacc(dec_energy.x, weight_energy_dec, energy.x);
      idx_dotcacc(z_energy.x, weight_energy_z, energy.x);
      cnt++;
      if ((cnt % (int) infp.anneal_period) == 0)
	temp_ip.eta *= infp.anneal_value;
    }
    /* TODO: for logging
       (:nr-iter-infer:x cnt) 
       (:exit-condition:x
       (if (>= cnt :ip:n) 1 (if (< old-energy (:energy:x)) 3 2)))
       (==> logger log-optimal this z)
    */
  }

  template <class T>
  bool codec<T>::check_code_threshold(fstate_idx<T> &z, gd_param &infp) {
    return idx_l2norm(z.dx) > infp.gradient_threshold;
  }

  ////////////////////////////////////////////////////////////////
  // codec_lone

  template <class T>
  codec_lone<T>::codec_lone(module_1_1<T> &encoder_,
			    module_1_1<T> &decoder_,
			    double weight_energy_enc_,
			    double weight_energy_z_,
			    double	weight_energy_dec_,
			    double thres,
			    gd_param &infp_)
    : codec<T>(encoder_, enc_cost_l2, weight_energy_enc_,
	       z_cost_l1, weight_energy_z_,
	       decoder_, dec_cost_l2, weight_energy_dec_, infp),
      z_cost_l1(thres) {
  }

  template <class T>
  codec_lone<T>::~codec_lone() {
  }

} // end namespace ebl
