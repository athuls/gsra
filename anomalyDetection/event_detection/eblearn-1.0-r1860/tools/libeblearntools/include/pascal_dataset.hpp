/***************************************************************************
 *   Copyright (C) 2009 by Pierre Sermanet   *
 *   pierre.sermanet@gmail.com   *
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

#ifndef PASCAL_DATASET_HPP_
#define PASCAL_DATASET_HPP_

#include <algorithm>

#include "xml_utils.h"
#include "tools_utils.h"

#ifdef __BOOST__
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
using namespace boost;
#endif

using namespace std;

namespace ebl {
  
  //////////////////////////////////////////////////////////////////////////////
  // constructors & initializations

  template <class Tdata>
  pascal_dataset<Tdata>::pascal_dataset(const char *name_, const char *inroot_,
					bool ignore_diff, bool ignore_trunc,
					bool ignore_occl,
					const char *annotations,
					const char *ignore_path)
    : dataset<Tdata>(name_, inroot_),
      min_aspect_ratio(-1), max_aspect_ratio(-1), minborders(),
      max_jitter_match(1.0) {
    // initialize pascal-specific members
    // if (inroot_) {
    //   annroot = inroot;
    //   annroot += "/Annotations/"; // look for xml files in annotations
    //   imgroot = inroot;
    //   imgroot += "JPEGImages/"; // image directory
    // }
    imgroot = inroot;
    if (annotations && strcmp(annotations, ""))
      annroot = annotations;
    else
      eblerror("expected annotations folder, please specify with -annotations");
    if (ignore_path && strcmp(ignore_path, ""))
      ignore_root = ignore_path;
    ignore_difficult = ignore_diff;
    ignore_truncated = ignore_trunc;
    ignore_occluded = ignore_occl;
#ifndef __XML__ // return error if xml not enabled
    eblerror("XML libraries not available, install libxml++ and recompile");
#endif /* __XML__ */
    extension = XML_PATTERN;
    cout << "Image search extension pattern: " << extension << endl;
  }

  template <class Tdata>
  pascal_dataset<Tdata>::~pascal_dataset() {
  }

  ////////////////////////////////////////////////////////////////
  // data extraction

  template <class Tdata>
  bool pascal_dataset<Tdata>::extract() {
    this->init_preprocessing();
#ifdef __BOOST__
#ifdef __XML__    
    if (!allocated && !strcmp(save_mode.c_str(), DATASET_SAVE))
      return false;
    cout << "Extracting samples from PASCAL files into dataset..." << endl;
    // adding data to dataset using all xml files in annroot
    path p(annroot);
    if (!exists(p))
      eblerror("Annotation path " << annroot << " does not exist.");
    xtimer.start();
    processed_cnt = 0;
    // find all xml files recursively
    list<string> *files = find_fullfiles(annroot, XML_PATTERN, NULL, true,
					 true);
    if (!files || files->size() == 0)
      eblerror("no xml files found in " << annroot << " using file pattern "
	       << XML_PATTERN);
    cout << "Found " << files->size() << " xml files." << endl;
    for (list<string>::iterator i = files->begin(); i != files->end(); ++i) {
      this->process_xml(*i);
      processed_cnt++;
      if (full())
	break;
    }
    cout << "Extracted " << data_cnt << " elements into dataset." << endl;
    cout << "Extraction time: " << xtimer.elapsed() << endl;
    print_stats();
    if (files) delete files;
#endif /* __XML__ */
#endif /* __BOOST__ */
    return true;
  }

  template <class Tdata>
  void pascal_dataset<Tdata>::set_min_aspect_ratio(float minar) {
    cout << "Setting minimum aspect ratio to " << minar << endl;
    min_aspect_ratio = minar;
  }

  template <class Tdata>
  void pascal_dataset<Tdata>::set_max_aspect_ratio(float maxar) {
    cout << "Setting maximum aspect ratio to " << maxar << endl;
    max_aspect_ratio = maxar;
  }

  template <class Tdata>
  void pascal_dataset<Tdata>::set_minborders(idxdim &d) {
    minborders = d;
    cout << "Setting minimum distances allowed to image borders to " 
	 << minborders << endl;
  }

  template <class Tdata>
  void pascal_dataset<Tdata>::set_max_jitter_match(float match) {
    max_jitter_match = match;
    cout << "Setting maximum jitter match between jittered rects and other "
	 << "objects in an image to " << max_jitter_match << endl;
  }

  ////////////////////////////////////////////////////////////////
  // data

#ifdef __XML__ // disable some derived methods if XML not available
#ifdef __BOOST__

  template <class Tdata>
  bool pascal_dataset<Tdata>::included(const string &class_name,
				       uint difficult, uint truncated,
				       uint occluded) {
    return dataset<Tdata>::included(class_name)
      && !(ignore_difficult && difficult)
      && !(ignore_truncated && truncated) 
      && !(ignore_occluded && occluded);
  }

  template <class Tdata>
  bool pascal_dataset<Tdata>::count_samples() {
    total_difficult = 0;
    total_truncated = 0;
    total_occluded = 0;
    total_ignored = 0;
    total_samples = 0;
    string xmlpath;
    path p(annroot);
    if (!exists(p))
      eblerror("Annotation path " << annroot << " does not exist.");
    // find all xml files recursively
    list<string> *files = find_fullfiles(annroot, XML_PATTERN, NULL, false,
					 true);
    if (!files || files->size() == 0)
      eblerror("no xml files found in " << annroot << " using file pattern "
	       << XML_PATTERN);
    cout << "Found " << files->size() << " xml files." << endl;
    for (list<string>::iterator i = files->begin(); i != files->end(); ++i) {
      xmlpath = *i;
      // parse xml
      try {
	DomParser parser;
	parser.parse_file(xmlpath);
	if (parser) {
	  // initialize root node and list
	  const Node* pNode = parser.get_document()->get_root_node();
	  Node::NodeList list = pNode->get_children();
	  // parse all objects in image
	  for(Node::NodeList::iterator iter = list.begin();
	      iter != list.end(); ++iter) {
	    if (!strcmp((*iter)->get_name().c_str(), "object")) {
	      // check for difficult flag in object node
	      Node::NodeList olist = (*iter)->get_children();
	      count_sample(olist);
	    }
	  }
	}
      } catch (const std::exception& ex) {
	cerr << "Xml exception caught: " << ex.what() << endl;
	return false;
      }
    }
    if (files) delete files;
    cout << "Found: " << total_samples << " samples, including ";
    cout << total_difficult << " difficult, " << total_truncated;
    cout << " truncated and " << total_occluded << " occluded." << endl;
    ignore_difficult ? cout << "Ignoring" : cout << "Using";
    cout << " difficult samples." << endl;
    ignore_truncated ? cout << "Ignoring" : cout << "Using";
    cout << " truncated samples." << endl;
    ignore_occluded ? cout << "Ignoring" : cout << "Using";
    cout << " occluded samples." << endl;
    total_samples = total_samples - total_ignored;
    return true;
  }

  template <class Tdata>
  void pascal_dataset<Tdata>::count_sample(Node::NodeList &olist) {
    uint difficult = 0, truncated = 0, occluded = 0;
    string obj_classname, pose;
    bool pose_found = false;
    Node::NodeList::iterator oiter;
      
    for(oiter = olist.begin(); oiter != olist.end(); ++oiter) {
      if (!strcmp((*oiter)->get_name().c_str(), "difficult"))
	difficult = xml_get_uint(*oiter);
      else if (!strcmp((*oiter)->get_name().c_str(), "truncated"))
	truncated = xml_get_uint(*oiter);
      else if (!strcmp((*oiter)->get_name().c_str(), "occluded"))
	occluded = xml_get_uint(*oiter);
      else if (!strcmp((*oiter)->get_name().c_str(), "name"))
	xml_get_string(*oiter, obj_classname);
      else if (!strcmp((*oiter)->get_name().c_str(), "pose")) {
	xml_get_string(*oiter, pose);
	pose_found = true;
      }
    }
    
    ////////////////////////////////////////////////////////////////
    // object
    if (!usepartsonly) {
      // add object's class to dataset
      if (included(obj_classname, difficult, truncated, occluded)) {
	if (usepose && pose_found) { // append pose to class name
	  obj_classname += "_";
	  obj_classname += pose;
	}
	if (included(obj_classname, difficult, truncated, occluded))
	  this->add_class(obj_classname);
      }
    }
    // increment samples numbers
    total_samples++;
    if (difficult) total_difficult++;
    if (truncated) total_truncated++;
    if (occluded) total_occluded++;
    if ((difficult && ignore_difficult)
	|| (truncated && ignore_truncated)
	|| (occluded && ignore_occluded))
      total_ignored++;
    
    ////////////////////////////////////////////////////////////////
    // parts
    if (useparts || usepartsonly) {
      string part_classname;
      
      // add part's class to dataset
      for(oiter = olist.begin();oiter != olist.end(); ++oiter) {
	if (!strcmp((*oiter)->get_name().c_str(), "part")) {
	  // get part's name
	  Node::NodeList plist = (*oiter)->get_children();
	  for(Node::NodeList::iterator piter = plist.begin();
	      piter != plist.end(); ++piter) {
	    if (!strcmp((*piter)->get_name().c_str(), "name")) {
	      xml_get_string(*piter, part_classname);
	      // found a part and its name, add it
	      if (included(part_classname, difficult, truncated, occluded)) {
		if (usepose && pose_found) { // append pose to class name
		  part_classname += "_";
		  part_classname += pose;
		}
		if (dataset<Tdata>::included(part_classname)) {
		  this->add_class(part_classname);
		  // increment samples numbers
		  this->total_samples++;
		  if (difficult) total_difficult++;
		  if (truncated) total_truncated++;
		  if (occluded) total_occluded++;
		  if ((difficult && ignore_difficult)
		      || (truncated && ignore_truncated)
		      || (occluded && ignore_occluded))
		    total_ignored++;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  ////////////////////////////////////////////////////////////////
  // process xml

  template <class Tdata>
  bool pascal_dataset<Tdata>::process_xml(const string &xmlfile) {
    string image_filename, image_fullname, folder;
    int height = -1, width = -1, depth = -1;
    rect<int> *cropr = NULL;

    // get image's properties
    if (!get_properties(xmlfile, image_filename, image_fullname, folder, height,
			width, depth, objects, &cropr, false))
      return false;
    // get ignored boxes if present
    if (!ignore_root.empty()) {
      string bname = ebl::basename(xmlfile.c_str());
      string dname = ebl::dirname(image_filename.c_str());
      string xml, filename, fullname, folder;
      int h = -1, w = -1, d = -1;
      xml << ignore_root << "/" << dname << "/" << bname;
      if (file_exists(xml)) {
	if (!get_properties(xml, filename, fullname, folder,
			    h, w, d, objects, &cropr, true))
	  return false;
	// check that ignored properties matches original properties
	if (fullname.compare(image_fullname) || (height != h) || (width != w))
	  eblerror("mistmatch between orignal image (" << image_fullname << ", "
		   << height << "x" << width << ") and ignored infos ("
		   << fullname << ", " << h << "x" << w << ")");
      } else // ignore file doesnt exist
	cerr << "warning: ignore xml not found: " << xml << endl;
    }
    // process objects
    process_objects(objects, height, width, image_fullname,
		    image_filename, cropr);
    // delete all objects
    for (uint i = 0; i < objects.size(); ++i)
      delete objects[i];
    objects.clear();
    if (cropr) delete cropr;
    return true;
  }

  ////////////////////////////////////////////////////////////////

  template <class Tdata>
  bool pascal_dataset<Tdata>::
  get_properties(const string &xmlfile, string &image_filename,
		 string &image_fullname, string &folder, int &height,
		 int &width, int &depth, vector<object*> &objs,
		 rect<int> **cropr, bool ignore) {
    int cxmin, cymin, cxmax, cymax; // crop bbox
    // parse xml file
    try {
      DomParser parser;
      //    parser.set_validate();
      parser.parse_file(xmlfile);
      if (parser) {
	// initialize root node and list
	const Node* pNode = parser.get_document()->get_root_node();
	Node::NodeList list = pNode->get_children();
	// get image filename and folder
	for(Node::NodeList::iterator iter = list.begin();
	    iter != list.end(); ++iter) {
	  if (!strcmp((*iter)->get_name().c_str(), "filename")) {
	    xml_get_string(*iter, image_filename);
	  } else if (!strcmp((*iter)->get_name().c_str(), "folder")) {
	    xml_get_string(*iter, folder);
	  } else if (!strcmp((*iter)->get_name().c_str(), "size")) {
	    Node::NodeList blist = (*iter)->get_children();
	    for(Node::NodeList::iterator biter = blist.begin();
		biter != blist.end(); ++biter) {
	      // get sizes
	      if (!strcmp((*biter)->get_name().c_str(), "height"))
		height = xml_get_int(*biter);
	      if (!strcmp((*biter)->get_name().c_str(), "width"))
		width = xml_get_int(*biter);
	      if (!strcmp((*biter)->get_name().c_str(), "depth"))
		depth = xml_get_int(*biter);
	    }
	  } else if (!strcmp((*iter)->get_name().c_str(), "crop")) {
	    Node::NodeList blist = (*iter)->get_children();
	    for(Node::NodeList::iterator biter = blist.begin();
		biter != blist.end(); ++biter) {
	      // save xmin, ymin, xmax and ymax
	      if (!strcmp((*biter)->get_name().c_str(), "xmin"))
		cymin = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "ymin"))
		cxmin = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "xmax"))
		cymax = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "ymax"))
		cxmax = xml_get_int(*biter);
	    }
	    *cropr = new rect<int>(cymin, cxmin, cymax - cymin, cxmax - cxmin);
	  }
	}
	image_fullname = imgroot;
	if (!folder.empty())
	  image_fullname << "/" << folder << "/";
	image_fullname << image_filename;
	get_objects(list, objs, *cropr, ignore);
      }
    } catch (const std::exception& ex) {
      cerr << "error: Xml exception caught: " << ex.what() << endl;
      return false;
    } catch (const char *err) {
      cerr << "error: " << err << endl;
      return false;
    }
    return true;
  }
  
  template <class Tdata>
  void pascal_dataset<Tdata>::
  get_objects(Node::NodeList &list, vector<object*> &objs, rect<int> *cropr,
	      bool ignore) {
    uint n = 0;
    // parse all objects in image
    for(Node::NodeList::iterator iobjs = list.begin();
	iobjs != list.end(); ++iobjs) {
      if (!strcmp((*iobjs)->get_name().c_str(), "object")) {
	// add a new object
	objs.push_back(new object(n++));
	object &o = *(objs.back());
	o.ignored = ignore;
	// get object attributes
	uint ipart = 0;
	Node::NodeList list = (*iobjs)->get_children();
	for(Node::NodeList::iterator iter = list.begin();
	    iter != list.end(); ++iter) {
	  // parse bounding box
	  if (!strcmp((*iter)->get_name().c_str(), "bndbox")) {
	    Node::NodeList blist = (*iter)->get_children();
	    int xmin = -1, ymin = -1, xmax = -1, ymax = -1;
	    for(Node::NodeList::iterator biter = blist.begin();
		biter != blist.end(); ++biter) {
	      // save xmin, ymin, xmax and ymax
	      if (!strcmp((*biter)->get_name().c_str(), "xmin"))
		xmin = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "ymin"))
		ymin = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "xmax"))
		xmax = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "ymax"))
		ymax = xml_get_int(*biter);
	    }
	    // set bbox
	    o.set_rect(xmin, ymin, xmax, ymax);
	    // adjusting coordinates if cropping image
	    if (cropr) {
	      o.h0 -= cropr->h0;
	      o.w0 -= cropr->w0;
	    }
	  }
	  // parse visible bounding box
	  else if (!strcmp((*iter)->get_name().c_str(), "visible")) {
	    Node::NodeList blist = (*iter)->get_children();
	    int vxmin = -1, vymin = -1, vxmax = -1, vymax = -1;
	    for(Node::NodeList::iterator biter = blist.begin();
		biter != blist.end(); ++biter) {
	      // save xmin, ymin, xmax and ymax
	      if (!strcmp((*biter)->get_name().c_str(), "xmin"))
		vxmin = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "ymin"))
		vymin = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "xmax"))
		vxmax = xml_get_int(*biter);
	      else if (!strcmp((*biter)->get_name().c_str(), "ymax"))
		vymax = xml_get_int(*biter);
	    }
	    o.set_visible(vxmin, vymin, vxmax, vymax);
	    // adjusting coordinates if cropping image
	    if (cropr) {
	      o.visible->h0 -= cropr->h0;
	      o.visible->w0 -= cropr->w0;
	    }
	  }
	  // parse centroid
	  else if (!strcmp((*iter)->get_name().c_str(), "center")) {
	    try {
	      Node::NodeList blist = (*iter)->get_children();
	      int x = -1, y = -1;
	      for(Node::NodeList::iterator biter = blist.begin();
		  biter != blist.end(); ++biter) {
		if (!strcmp((*biter)->get_name().c_str(), "x"))
		  x = xml_get_uint(*biter);
		else if (!strcmp((*biter)->get_name().c_str(), "y"))
		  y = xml_get_uint(*biter);
	      }
	      o.set_centroid(x, y);
	      // adjusting coordinates if cropping image
	      if (cropr) {
		o.centroid->first -= cropr->h0;
		o.centroid->second -= cropr->w0;
	      }
	    } catch(ebl::eblexception &e) { 
	      cerr << "warning: no centroid found." << endl; }
	  } // else get object class name
	  else if (!strcmp((*iter)->get_name().c_str(), "name"))
	    xml_get_string(*iter, o.name);
	  else if (!strcmp((*iter)->get_name().c_str(), "difficult"))
	    o.difficult = xml_get_uint(*iter);
	  else if (!strcmp((*iter)->get_name().c_str(), "truncated"))
	    o.truncated = xml_get_uint(*iter);
	  else if (!strcmp((*iter)->get_name().c_str(), "occluded"))
	    o.occluded = xml_get_uint(*iter);
	  else if (!strcmp((*iter)->get_name().c_str(), "pose")) {
	    xml_get_string(*iter, o.pose);
	  }
	  ////////////////////////////////////////////////////////////////
	  // parts
	  else if (!strcmp((*iter)->get_name().c_str(), "part")) {
	    Node::NodeList plist = (*iter)->get_children();
	    for(Node::NodeList::iterator piter = plist.begin();
		piter != plist.end(); ++piter) {
	      // add new part object
	      o.parts.push_back(new object(ipart++));
	      object &p = *(o.parts.back());
	      // parse bounding box
	      if (!strcmp((*piter)->get_name().c_str(), "bndbox")) {
		Node::NodeList blist = (*piter)->get_children();
		int xmin = -1, ymin = -1, xmax = -1, ymax = -1;
		for(Node::NodeList::iterator biter = blist.begin();
		    biter != blist.end(); ++biter) {
		  // save xmin, ymin, xmax and ymax
		  if (!strcmp((*biter)->get_name().c_str(), "xmin"))
		    xmin = xml_get_uint(*biter);
		  else if (!strcmp((*biter)->get_name().c_str(), "ymin"))
		    ymin = xml_get_uint(*biter);
		  else if (!strcmp((*biter)->get_name().c_str(), "xmax"))
		    xmax = xml_get_uint(*biter);
		  else if (!strcmp((*biter)->get_name().c_str(), "ymax"))
		    ymax = xml_get_uint(*biter);
		}
		// set bbox
		p.set_rect(xmin, ymin, xmax, ymax);
		// adjusting coordinates if cropping image
		if (cropr) {
		  p.h0 -= cropr->h0;
		  p.w0 -= cropr->w0;
		}
	      } // else get object class name
	      else if (!strcmp((*piter)->get_name().c_str(), "name")) {
		xml_get_string(*piter, p.name);
	      }
	    }
	  }
	}
      }
    }
  }
  
  ////////////////////////////////////////////////////////////////
  // process all objects

  template <class Tdata>
  bool pascal_dataset<Tdata>::
  process_objects(const vector<object*> &objs, int height, int width,
		  const string &image_fullname, const string &image_filename,
		  const rect<int> *cropr) {
    idx<Tdata> *img = NULL;
    // process all objects
    for (iobj = 0; iobj < objs.size(); ++iobj) {
      object &o = *(objs[iobj]);
      if (!o.ignored) { // only process non-ignored objects
	if (!usepartsonly) {
	  // check that minimum visibility ratio is respected
	  if (minvisibility > 0.0 && o.visible) {
	    float match = o.match(*o.visible);
	    if (match < minvisibility) {
	      cout << "Rejecting object " << o.id << " from " 
		   << image_filename << " because visible ratio ("
		   << match << ") is lower than "
		   << "limit (" << minvisibility << ")" << endl;
	      continue ; // skip this object
	    }
	  }
	  // check that minimum aspect ratio is respected
	  float ar = o.width / (float) o.height;
	  if (min_aspect_ratio >= 0.0 && ar < min_aspect_ratio) {
	    cout << "Rejecting object " << o.id << " from " << image_filename 
		 << " because aspect ratio of " << o << " (" << ar 
		 << ") is lower than " << "limit (" << min_aspect_ratio << ")" 
		 << endl;
	    continue ; // skip this object	  
	  }
	  if (max_aspect_ratio >= 0.0 && ar > max_aspect_ratio) {
	    cout << "Rejecting object " << o.id << " from " << image_filename 
		 << " because aspect ratio of " << o << " (" << ar 
		 << ") is bigger than " << "limit (" << max_aspect_ratio << ")" 
		 << endl;
	    continue ; // skip this object
	  }
	  // check that object is larger than minimum size
	  if (o.height < mindims.dim(0) || o.width < mindims.dim(1)) {
	    cout << "Rejecting object " << o.id << " from " 
		 << image_filename << " because object's size ("
		 << o << ") is smaller than minimum size "
		 << "(" << mindims << ")" << endl;
	    continue ; // skip this object
	  }
	  // check that bbox is not below image border distances
	  if (!minborders.empty()) {
	    if (height < 0 || width < 0) { // we don't know the image sizes
	      eblerror("image sizes not found in xml");
	    } // from now on assume all images have the same size (TODO)
	    int hmax = height - minborders.dim(0);
	    int wmax = width - minborders.dim(1);
	    if (o.h0 < (int) minborders.dim(0) || o.w0 < minborders.dim(1)
		|| o.h0 + o.height > hmax || o.w0 + o.width > wmax) {
	      cout << "Rejecting object " << o.id << " from " 
		   << image_filename << " because (cropped) bbox " << o
		   << " is closer to image borders (" << height << "x" << width
		   << ") than allowed (" << minborders << ")" << endl;
	      continue ; // skip this object
	    }
	  }
	  // process image  
	  if (included(o.name, o.difficult, o.truncated, o.occluded)) {
	    // load image if not already loaded
	    if (!img) {
	      // check that image exists
	      if (!file_exists(image_fullname)) {
		cerr << "Error: image not found " << image_fullname << endl;
		return false;
	      }
	      // load image
	      idx<Tdata> im = load_image<Tdata>(image_fullname);
	      // check that image matches expected sizes
	      if (im.dim(0) != height || im.dim(1) != width) {
		cerr << "Error: expected image of size " << height << "x" << width
		     << " but got " << im << " in " << image_fullname << endl;
		return false;
	      }
	      // crop it
	      if (cropr) {
		im = im.narrow(0, cropr->height, cropr->h0);
		im = im.narrow(1, cropr->width, cropr->w0);
	      }
	      img = new idx<Tdata>(im);
	    }
	    // remove jitters that overlap with other objects
	    if (max_jitter_match > 0)
	      remove_jitter_matches(objs, iobj, max_jitter_match);
	    // extract object from image
	    process_image(*img, o, o.name, o.difficult, 
			  image_filename, o.centroid, o.visible, cropr);
	  }
	}
      }
    }
    if (img) delete img;
    return true;
  }
  
  ////////////////////////////////////////////////////////////////
  // process object's image

  template <class Tdata>
  void pascal_dataset<Tdata>::
  process_image(idx<Tdata> &img, const rect<int> &r,
		string &obj_class, uint difficult, const string &image_filename,
		pair<int,int> *center, const rect<int> *visr,
		const rect<int> *cropr) {
    t_label label = this->get_label_from_class(obj_class);
    add_data(img, label, &obj_class, image_filename.c_str(), &r, center,
	     visr, cropr, &objects);
  }

  ////////////////////////////////////////////////////////////////
#endif /* __XML__ */
#endif /* __BOOST__ */
  
  template <class Tdata>
  void pascal_dataset<Tdata>::compute_random_jitter() {
    // recompute all possible jitters
    dataset<Tdata>::compute_random_jitter();
    // remove jitters overlapping with other boxes given current box
    remove_jitter_matches(objects, iobj, max_jitter_match);
  }

  template <class Tdata>
  void pascal_dataset<Tdata>::
  remove_jitter_matches(const vector<object*> &objs,
			uint iobj, float max_match) {
    rect<int> ri = *objs[iobj];
    // apply w/h ratio to object
    if (this->bbox_woverh > 0)
      ri.scale_width(this->bbox_woverh);
    // find ratio between current box and target
    float ratio = std::max(ri.height / (float) this->height,
			   ri.width / (float) this->width);
    DEBUG("jitter ratio is " << ratio);
    // check overlaps with other objects
    vector<jitter>::iterator i = random_jitter.begin();
    for ( ; i != random_jitter.end(); ++i) {
      rect<int> rijitt = i->get_rect(ri, ratio);
      for (uint j = 0; j < objs.size(); ++j) {
	rect<int> rj = *objs[j];
	// scale object the same way jitter has been scaled
	rj.scale_centered(i->s, i->s);
 	// apply w/h ratio to object
	if (this->bbox_woverh > 0)
	  rj.scale_width(this->bbox_woverh);
	if (rj.match(rijitt) > max_match) {
	  // there is a match, remove this jitter
	  // note: erasing in a vector is not efficient because it has to move
	  // all elements, but this should be rare enough to be better than
	  // creating a new vector to which we add all elements.
	  cout << "(removing jitter) match with jitter is more than "
	       << max_match << ": " << rj << " and " << rijitt
	       << " match by " << rj.match(rijitt) << endl;
	  i = random_jitter.erase(i);
	  if (i == random_jitter.end())
	    return ;
	  break ; // stop looking for match
	}
      }
    }
  }
  
} // end namespace ebl

#endif /* PASCAL_DATASET_HPP_ */
