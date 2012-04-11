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

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // supervised_trainer_gui

  template <class Tnet, class Tdata, class Tlabel>  
  supervised_trainer_gui<Tnet, Tdata, Tlabel>::
  supervised_trainer_gui(const char *title_, bool scroll_)
    : _st(NULL), _ds(NULL), _last_ds(NULL), 
      datasource_wid(-1), datasource_wid2(-1), 
      datasource_wid3(-1), datasource_wid4(-1),
      internals_wid(-1), internals_wid2(-1), internals_wid3(-1),
      scroll(scroll_), scroll_added(false), pos(0), dsgui(NULL) {
    if (title_) {
      title0 = title_;
      title0 << ": ";
    }
  }

  template <class Tnet, class Tdata, class Tlabel>  
  supervised_trainer_gui<Tnet, Tdata, Tlabel>::~supervised_trainer_gui() {
    if (w)
      w->replace_scroll_box_with_copy(this);
  }
  
  template <class Tnet, class Tdata, class Tlabel>  
  void supervised_trainer_gui<Tnet, Tdata, Tlabel>::
  display_datasource(supervised_trainer<Tnet, Tdata, Tlabel> &st,
		     labeled_datasource<Tnet, Tdata, Tlabel> &ds,
		     infer_param &infp,
		     unsigned int nh, unsigned int nw, unsigned int h0, 
		     unsigned int w0, double zoom, int wid, const char *title,
		     bool scrolling) {
    // copy parameters
    _st = &st;
    _infp = &infp;
    _nh = nh;
    _nw = nw;
    _h0 = h0;
    _w0 = w0;
    if (!dsgui)
      dsgui = new labeled_datasource_gui<Tnet, Tdata, Tlabel>(false);
    // do a deep copy of dataset only when necessary
    if (scroll && !scrolling && (_last_ds != &ds)) {
      if (_ds)
	delete _ds;
      _ds = new labeled_datasource<Tnet, Tdata, Tlabel>(ds);
      dsgui->_ds = _ds;
    }
    _last_ds = &ds;
    // init datasource
    st.init(ds);
    vector<string*> lblstr;
    class_datasource<Tnet,Tdata,Tlabel> *cds =
      dynamic_cast<class_datasource<Tnet,Tdata,Tlabel>*>(&ds);
    if (cds)
      lblstr = cds->get_label_strings();
    idxdim d = ds.sample_dims();
    _h1 = h0 + nh * (d.dim(0) + 1) * 3;
    _w1 = w0 + nw * (d.dim(1) + 1) * 3;
    _zoom = zoom;
    // if no window given, create a new one or reuse previous one
    datasource_wid = (wid >= 0) ? wid : 
      ((datasource_wid >= 0) ? datasource_wid :
       new_window((title ? title : "Supervised Trainer")));
    select_window(datasource_wid);
    if (scroll && !scroll_added) {
      gui.add_scroll_box((scroll_box*) this);
      scroll_added = true;
    }
    disable_window_updates();
    if (wid == -1) // clear only if we created the window
      clear_window();
    // top left coordinates of datasets display 1 and 2
    unsigned int w01 = nh * (d.dim(1) + 2) + 5, h01 = h0 + 35;
    unsigned int w02 = (nh * (d.dim(1) + 2) + 5) * 2, h02 = h0 + 35;
    // working variables
    unsigned int h1 = h01, w1 = w01, nh1 = 0;
    unsigned int h2 = h02, w2 = w02, i2 = 0;
    bool correct;

    // display top
    gui << set_colors(255, 0, 0, 255, 255, 255, 255, 127) << gui_only();
    gui << at(h0, w0) << ds.name();
    gui << black_on_white();
    gui << at(h0 + 17, w0) << "Groundtruth";
    gui << at(h0 + 17, w01) << "Correct & incorrect answers";
    gui << at(h0 + 17, w02) << "Incorrect only";
    gui << white_on_transparent();

    // 0. display dataset with groundtruth labels
    dsgui->display(ds, nh, nw, h0 + 35, w0, zoom, datasource_wid, NULL, true);

    // loop on nh * nw first samples
    gui << white_on_transparent();
    // loop to reach pos
    ds.seek_begin();
    for (unsigned int p = 0; p < pos; ++p)
      ds.next(); // FIXME add a seek(p) method to ds
    for (unsigned int i = 0; (i < ds.size()) && (i2 < nh * nw); ++i) {
      // test sample
      ds.fprop_label_net(*st.label);
      correct = st.test_sample(ds, *st.label, *st.answers, infp);
      ds.next();
      idx<Tnet> m = st.machine.in1.x.select(0, 0);

      // 1. display dataset with incorrect and correct answers
      if (nh1 < nh) {
	draw_matrix_frame(m, (correct?0:128), 0, 0, h1, w1, zoom, zoom);
	if ((lblstr.size() > 0) && (lblstr[(int)st.answers->x.gget()]))
	  gui << at(h1 + 2, w1 + 2)
	      << (lblstr[(int)st.answers->x.gget()])->c_str();
	w1 += d.dim(1) + 2;
	if (((i + 1) % nw == 0) && (i > 1)) {  
	  w1 = w01;
	  h1 += d.dim(0) + 2;
	  nh1++;
	}
      }
      // 2. display first nh * nw incorrect answers
      if (!correct) {
	draw_matrix_frame(m, (correct?0:128), 0, 0, h2, w2, zoom, zoom);
	if ((lblstr.size() > 0) && (lblstr[(int)st.answers->x.gget()]))
	  gui << at(h2 + 2, w2 + 2)
	      << (lblstr[(int)st.answers->x.gget()])->c_str();
	w2 += d.dim(1) + 2;
	if (((i2 + 1) % nw == 0) && (i2 > 1)) {  
	  w2 = w02;
	  h2 += d.dim(0) + 2;
	}
	i2++;
      }
    }
    enable_window_updates();
  }

  template <class Tnet, class Tdata, class Tlabel>  
  void supervised_trainer_gui<Tnet, Tdata, Tlabel>::
  display_internals(supervised_trainer<Tnet, Tdata, Tlabel> &st,
		    labeled_datasource<Tnet, Tdata, Tlabel> &ds,
		    infer_param &infp, gd_param &args,
		    unsigned int ninternals, unsigned int display_h0, 
		    unsigned int display_w0, double display_zoom, 
		    int wid, const char *title) {
    if (ninternals == 0) return ;
    internals_wid = (wid >= 0) ? wid : 
      ((internals_wid >= 0) ? internals_wid :
       new_window((title ? 
		       title : "Supervised Trainer: internal fprop states")));
    internals_wid2 = (wid >= 0) ? wid : 
      ((internals_wid2 >= 0) ? internals_wid2 :
       new_window((title ? 
		       title : "Supervised Trainer: internal bprop states")));
    internals_wid3 = (wid >= 0) ? wid : 
      ((internals_wid3 >= 0) ? internals_wid3 :
       new_window((title ? 
		       title : "Supervised Trainer: internal bbprop states")));
    // freeze and clear display updates
    select_window(internals_wid);
    disable_window_updates();
    clear_window();
    select_window(internals_wid2);
    disable_window_updates();
    clear_window();
    select_window(internals_wid3);
    disable_window_updates();
    clear_window();
    // prepare dataset
    st.init(ds);
    unsigned int wfdisp = 0, hfdisp = 0;
    unsigned int wfdisp2 = 0, hfdisp2 = 0;
    unsigned int wfdisp3 = 0, hfdisp3 = 0;
    
    // display first ninternals samples
    trainable_module_gui mg;
    bbstate_idx<Tnet> input(ds.sample_dims());
    bbstate_idx<Tlabel> label(ds.label_dims());
    for (unsigned int i = 0; (i < ds.size()) && (i < ninternals); ++i) {
      // prepare input
      ds.fprop(input, label);
      // // fprop and bprop
      // //st.test_sample(*st.input, st.label, infp);
      // // TODO: display is influencing learning, remove influence
      // //      st.learn_sample(*st.input, st.label.get(), args);
      // st.machine.fprop(*st.input, st.label, st.energy);
      // st.param.clear_dx();
      // st.machine.bprop(*st.input, st.label, st.energy);
      // st.param.clear_ddx();
      // st.machine.bbprop(*st.input, st.label, st.energy);
      
      // display fprop
      mg.display_fprop(st.machine, input, label, st.energy, 
		       hfdisp, wfdisp, display_zoom, (Tnet) -1.0, (Tnet) 1.0,
		       true, internals_wid);
      // display bprop
      select_window(internals_wid2);
      mg.display_bprop(st.machine, input, label, st.energy, 
      		       hfdisp2, wfdisp2, display_zoom, (Tnet) -1.0, (Tnet) 1.0,
		       true, internals_wid2);
      // display bprop
      select_window(internals_wid3);
      mg.display_bbprop(st.machine, input, label, st.energy, 
			hfdisp3, wfdisp3, display_zoom, (Tnet) -.01, (Tnet) .01,
			true, internals_wid3);
      ds.next(); // next sample
      hfdisp += 10;
      hfdisp2 += 10;
      hfdisp3 += 10;
    }
    // unfreeze display updates
    select_window(internals_wid);
    enable_window_updates();
    select_window(internals_wid2);
    enable_window_updates();
    select_window(internals_wid3);
    enable_window_updates();
  }  

  template <class Tnet, class Tdata, class Tlabel>  
  void supervised_trainer_gui<Tnet, Tdata, Tlabel>::
  display_correctness(bool incorrect, bool up,
		      supervised_trainer<Tnet, Tdata, Tlabel> &st,
		      labeled_datasource<Tnet, Tdata, Tlabel> &ds,
		      infer_param &infp,
		      unsigned int nh, unsigned int nw,
		      bool print_raw_outputs, bool draw_all_jitter,
		      unsigned int h0, unsigned int w0, double zoom, int wid,
		      const char *title, bool scrolling) {
    // copy parameters
    _st = &st;
    _infp = &infp;
    _nh = nh;
    _nw = nw;
    _h0 = h0;
    _w0 = w0;
    if (!dsgui)
      dsgui = new labeled_datasource_gui<Tnet, Tdata, Tlabel>(false);
    // do a deep copy of dataset only when necessary
    if (scroll && !scrolling && (_last_ds != &ds)) {
      if (_ds)
	delete _ds;
      _ds = new labeled_datasource<Tnet, Tdata, Tlabel>(ds);
      dsgui->_ds = _ds;
    }
    _last_ds = &ds;
    // init datasource
    st.init(ds);
    class_datasource<Tnet,Tdata,Tlabel> *cds =
      dynamic_cast<class_datasource<Tnet,Tdata,Tlabel>*>(&ds);
    // find out sample size
    idxdim d = ds.sample_dims();
    _h1 = h0 + nh * (d.dim(0) + 1) * 3;
    _w1 = w0 + nw * (d.dim(1) + 1) * 3;
    _zoom = zoom;
    string corname = (incorrect ? "incorrect" : "correct");
    title1 = title0;
    title1 << "Incorrect classifications, lowest energies first (pid "
	   << pid() << ")";
    title2 = title0;
    title2 << "Incorrect classifications, highest energies first (pid "
	   << pid() << ")";
    title3 = title0;
    title3 << "Correct classifications, lowest energies first (pid "
	   << pid() << ")";
    title4 = title0;
    title4 << "Correct classifications, highest energies first (pid "
	   << pid() << ")";
    // if no window given, create a new one or reuse previous one
    if (incorrect) { // incorrect window
      if (up) {
	datasource_wid2 = (wid >= 0) ? wid : 
	  ((datasource_wid2 >= 0) ? datasource_wid2 :
	   new_window((title ? title : title1.c_str())));
	select_window(datasource_wid2);
      } else {
	datasource_wid3 = (wid >= 0) ? wid : 
	  ((datasource_wid3 >= 0) ? datasource_wid3 :
	   new_window((title ? title : title2.c_str())));
	select_window(datasource_wid3);
      }
    } else { // correct window
      if (up) {
	datasource_wid = (wid >= 0) ? wid : 
	  ((datasource_wid >= 0) ? datasource_wid :
	   new_window((title ? title : title3.c_str())));
	select_window(datasource_wid);
      } else {
	datasource_wid4 = (wid >= 0) ? wid : 
	  ((datasource_wid4 >= 0) ? datasource_wid4 :
	   new_window((title ? title : title4.c_str())));
	select_window(datasource_wid4);
      }
    }
    if (scroll && !scroll_added) {
      gui.add_scroll_box((scroll_box*) this);
      scroll_added = true;
    }
    disable_window_updates();
    if (wid == -1) // clear only if we created the window
      clear_window();
    // top left coordinates of datasets display 1 and 2
    unsigned int w01 = w0, h01 = h0 + 20;
    unsigned int h1 = h01, w1 = w01, nh1 = 0;

    // get the indices of incorrect or correct samples ordered by their energies
    // this assumes that all samples have already been tested for correctness
    intg tally = idx_sum<intg>(ds.correct);
    if (incorrect) tally = ds.correct.dim(0) - tally;
    idx<intg> indices(tally);
    idx<double> energies(tally);
    intg inc = 0, i = 0;
    idx_bloop2(correct, ds.correct, ubyte, energy, ds.energies, double) {
      if (correct.get() == !incorrect) { // found correct or incorrect sample
	indices.set(i, inc);
	energies.set(energy.get(), inc);
	inc++;
      }
      i++; // data iterator
    }
    if (inc != indices.dim(0))
      eblerror("expected " << indices.dim(0) << " " << corname
	       << " samples but found " << inc);

    // display top
    gui << set_colors(255, 0, 0, 255, 255, 255, 255, 127) << gui_only();
    gui << at(h0, w0) << ds.name() << ": ";
    gui << black_on_white();
    if (inc == 0) {
      gui << "no " << corname << " samples." << endl;
      enable_window_updates();
      return ;
    }
    gui << indices.dim(0) << " / " << ds.correct.dim(0) << " "
	<< corname << " samples with energies max: "
	<< (double) idx_max(energies) << " min: " << (double) idx_min(energies);
    gui << white_on_transparent();

    // sort indices by energy
    if (up)
      idx_sortup(energies, indices);
    else
      idx_sortdown(energies, indices);
    
    // loop on nh * nw first samples
    Tlabel answer, bgid = -1;
    if (cds)
      cds->get_class_id("bg");
    bbstate_idx<Tnet> input(ds.sample_dims());
    bbstate_idx<Tlabel> label(ds.label_dims());
    bbstate_idx<Tnet> jitt(1, 1);
    for (unsigned int i = 0; (i < indices.dim(0)) && (i < nh * nw); ++i) {
      idx<Tnet> raw = ds.raw_outputs.select(0, indices.get(i));
      idx<Tnet> answers = ds.answers.select(0, indices.get(i));
      idx<Tnet> target = ds.targets.select(0, indices.get(i));
      answer = (Tlabel) answers.get(0);
      ds.select_sample(indices.get(i));
      ds.fprop(input, label);
      idx<Tnet> m = input.x.shift_dim(0, 2);

      // 1. display dataset with incorrect and correct answers
      if (nh1 < nh) {
	draw_matrix(m, h1, w1, zoom, zoom);
	ostringstream s;
	s.precision(2);
	if (cds)
	  s << cds->get_class_name((int)answer) << " ";
	s << energies.get(i);
	gui << at(h1 + 2, w1 + 2) << s.str().c_str();
	// print raw outputs
	if (print_raw_outputs) {
	  for (uint a = 0; a < raw.dim(0); ++a) {
	    s.str(""); s << raw.get(a);
	    gui << at(h1 + 17 + a * 15, w1 + 2) << s.str().c_str();
	  }
	} else { // print outputs answers
	  for (uint a = 0; a < answers.dim(0); ++a) {
	    s.str(""); s << answers.get(a);
	    gui << at(h1 + 17 + a * 15, w1 + 2) << s.str().c_str();
	  }
	}
	// print target info
	//ds.fprop_jitter(jitt);
	for (uint a = 0; a < target.dim(0); ++a) {
	  s.str(""); s << target.gget(a);
	  gui << at(h1 + 17 + a * 15, w1 + m.dim(1) - 35) << s.str().c_str();
	}
	// print correct info when incorrect
	if (incorrect && cds) {
	  s.str("");
	  s << "(" << cds->get_class_name((int) label.x.get(0)) << ")";
	  gui << at(h1 + d.dim(0) - 15, w1 + 2) << s.str().c_str();
	}
	// draw scale box if not a background class
	if (answer != bgid && answers.dim(0) > 2) {
	  float scale = answers.gget(2), hoff = 0, woff = 0;
	  rect<float> r;
	  if (scale > 0) {
	    scale = 1 / scale;
	    if (answers.dim(0) == 5) {
	      hoff = answers.gget(3) * m.dim(0);
	      woff = answers.gget(4) * m.dim(0);
	    }
	    r = rect<float>(h1 + hoff, w1 + woff, m.dim(0), m.dim(1));
	    r.scale_centered(scale, scale);
	    draw_box(r, 0, 0, 255);
	    // draw all groundtruth
	    if (draw_all_jitter) {
	      ds.fprop_jitter(jitt);
	      idx_bloop1(ji, jitt.x, Tnet) {
		scale = ji.gget(0); // TODO: fix hardcoded offset
		if (scale != 0) {
		  scale = 1 / scale;
		  hoff = ji.gget(1) * m.dim(0);// TODO: fix hardcoded offset
		  woff = ji.gget(2) * m.dim(0);// TODO: fix hardcoded offset
		  r = rect<float>(h1 + hoff, w1 + woff, m.dim(0), m.dim(1));
		  r.scale_centered(scale, scale);
		draw_box(r, 0, 255, 0);
		}
	      }
	    }
	  }
	  // draw groundtruth box
	  scale = target.gget(2); // TODO: fix hardcoded offset
	  if (scale != 0) {
	    scale = 1 / scale;
	    hoff = target.gget(3) * m.dim(0);// TODO: fix hardcoded offset
	    woff = target.gget(4) * m.dim(0);// TODO: fix hardcoded offset
	    r = rect<float>(h1 + hoff, w1 + woff, m.dim(0), m.dim(1));
	    r.scale_centered(scale, scale);
	    draw_box(r, 255, 0, 0);
	  }
	}
	// if ((ds.lblstr) && (ds.lblstr->at(st.answer.x.get())))
	//   gui << at(h1 + 2, w1 + 2)
	//       << (ds.lblstr->at(st.answer.x.get()))->c_str();
	w1 += m.dim(1) + 2;
	if (((i + 1) % nw == 0) && (i > 1)) {  
	  w1 = w01;
	  h1 += m.dim(0) + 2;
	  nh1++;
	}
      }
    }
    enable_window_updates();
  }

  ////////////////////////////////////////////////////////////////
  // inherited methods to implement for scrolling capabilities

  template<class Tnet, class Tdata, class Tlabel>
  void supervised_trainer_gui<Tnet, Tdata, Tlabel>::display_next() {
    if (next_page()) {
      pos = MIN(_ds->size(), pos + _nh * _nw);

      display_datasource(*_st, *_ds, *_infp, _nh, _nw, _h0, _w0, _zoom,
			 -1, NULL, true);
    }
  }

  template<class Tnet, class Tdata, class Tlabel>
  void supervised_trainer_gui<Tnet, Tdata, Tlabel>::display_previous() {
    if (previous_page()) {
      pos = std::max((uint) 0, pos - _nh * _nw);
      display_datasource(*_st, *_ds, *_infp, _nh, _nw, _h0, _w0, _zoom,
			 -1, NULL, true);
    }
  }

  template<class Tnet, class Tdata, class Tlabel>
  unsigned int supervised_trainer_gui<Tnet, Tdata, Tlabel>::max_pages() {
    return dsgui->max_pages();
  }

  template<class Tnet, class Tdata, class Tlabel>
  supervised_trainer_gui<Tnet, Tdata, Tlabel>* 
  supervised_trainer_gui<Tnet, Tdata, Tlabel>::copy() {
    //  scroll_box0* supervised_trainer_gui<Tnet, Tdata, Tlabel>::copy() {
    cout << "supervsed_trainer_gui::copy."<<endl;
    supervised_trainer_gui<Tnet, Tdata, Tlabel> *stcopy = 
      new supervised_trainer_gui<Tnet, Tdata, Tlabel>(*this);
    stcopy->dsgui = dsgui->copy();
    stcopy->_ds = _ds;
    stcopy->_last_ds = _last_ds;
    stcopy->_st = _st;
    return stcopy;
  }

} // end namespace ebl
