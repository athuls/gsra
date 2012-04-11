/***************************************************************************
 *   Copyright (C) 2010 by Pierre Sermanet   *
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

#ifndef TOOLS_UTILS_H_
#define TOOLS_UTILS_H_

#include <list>
#include <string>
#include <vector>
#include <map>

#include "defines.h"
#include "idx.h"

using namespace std;

namespace ebl {

  ////////////////////////////////////////////////////////////////
  // directory utilities

  typedef pair<string, string> stringpair;
  typedef list<stringpair> files_list;

  inline bool less_than(const stringpair& b1, const stringpair& b2);
  
  //! Returns a list of pairs of root directory and filename of all files
  //! found recursively in directory 'dir'. The files are found using
  //! the IMAGE_PATTERN regular expression by default.
  //! If the directory does not exists, it returns NULL.
  //! The user is responsible for deleting the returned list.
  //! \param fl A file list where new found files will be apprended if not null.
  //!           If null, a new list is allocated. This is used by the recursion.
  //! \param pattern The regular expression describing the file name pattern.
  //!           The default pattern matches images extensions.
  //! \param randomize If true, randomize the returned list.
  EXPORT files_list *find_files(const string &dir,
				const char *pattern = IMAGE_PATTERN,
				files_list *fl = NULL, bool sort = true,
				bool recursive = true, bool randomize = false);
  
  //! Returns a list of string of full paths to files recursively found in
  //! directory dir and matching the pattern. The files are found using
  //! the IMAGE_PATTERN regular expression by default.
  //! If the directory does not exists, it returns NULL.
  //! The user is responsible for deleting the returned list.
  //! \param fl A file list where new found files will be apprended if not null.
  //!           If null, a new list is allocated. This is used by the recursion.
  //! \param pattern The regular expression describing the file name pattern,
  //!           e.g. ".*[.](mat|MAT)".
  //!           The default pattern matches images extensions.
  //! \param randomize If true, randomize the returned list.
  //! \param finddir If true, include directories in results.
  EXPORT list<string> *find_fullfiles(const string &dir,
				      const char *pattern = IMAGE_PATTERN,
				      list<string> *fl = NULL, 
				      bool sort = true,
				      bool recursive = true, 
				      bool randomize = false,
				      bool finddir = false);
  
  //! Counts recursively the number of files matching the pattern (default is
  //! an image extension pattern) in directory 'dir'.
  //! \param pattern The regular expression describing the file name pattern.
  //!           The default pattern matches images extensions.
  EXPORT uint count_files(const string &dir, 
			  const char *pattern = IMAGE_PATTERN);

  //! Convert a string to a bool. Throws a const char * exception
  //! upon failure.
  EXPORT bool string_to_bool(const string &s);
  //! Convert a string to an int. Throws a const char * exception
  //! upon failure.
  EXPORT int string_to_int(const string &s);
  //! Convert a string to an int. Throws a const char * exception
  //! upon failure.
  EXPORT int string_to_int(const char *s);
  //! Convert a string to an intg. Throws a const char * exception
  //! upon failure.
  EXPORT intg string_to_intg(const string &s);
  //! Convert a string to an intg. Throws a const char * exception
  //! upon failure.
  EXPORT intg string_to_intg(const char *s);
  //! Convert a string to an unsigned int. Throws a const char * exception
  //! upon failure.
  EXPORT uint string_to_uint(const string &s);
  //! Convert a string to an unsigned int. Throws a const char * exception
  //! upon failure.
  EXPORT uint string_to_uint(const char *s);
  //! Convert a string to an float. Throws a const char * exception
  //! upon failure.
  EXPORT float string_to_float(const string &s);
  //! Convert a string to an float. Throws a const char * exception
  //! upon failure.
  EXPORT float string_to_float(const char *s);
  //! Convert a string to an double. Throws a const char * exception
  //! upon failure.
  EXPORT double string_to_double(const string &s);
  //! Convert a string to an double. Throws a const char * exception
  //! upon failure.
  EXPORT double string_to_double(const char *s);

  //! Convert a string containing a list of uint separated by commas, e.g.
  //! "1,2,3,4" into a list of uints.
  EXPORT list<uint> string_to_uintlist(const string &s);
  //! Convert a string containing a list of uint separated by commas, e.g.
  //! "1,2,3,4" into a list of uints.
  EXPORT list<uint> string_to_uintlist(const char *s);
  //! Convert a string containing a list of uint separated by commas, e.g.
  //! "1,2,3,4" into a vector of uints.
  //! \param sep Specifies the separating character, comma by default.
  EXPORT vector<uint> string_to_uintvector(const string &s, char sep = ',');
  //! Convert a string containing a list of uint separated by commas, e.g.
  //! "1,2,3,4" into a vector of uints.
  //! \param sep Specifies the separating character, comma by default.
  EXPORT vector<uint> string_to_uintvector(const char *s, char sep = ',');

  //! Convert a string containing a list of numbers separated by 'sep', e.g.
  //! "1x2x3x4" into an idx<T>.
  //! \param sep Specifies the separating character, 'x' by default.
  template <typename T>
  EXPORT idx<T> string_to_idx(const char *s, char sep = ',');
  //! Convert the number in s into a value of type T and return it.
  template <typename T>
  EXPORT T string_to_number(const char *s);  
  //! Convert a string containing a list of uint separated by 'sep', e.g.
  //! "1x2x3x4" into an idxdim.
  //! \param sep Specifies the separating character, 'x' by default.
  EXPORT idxdim string_to_idxdim(const char *s, char sep = 'x');
  //! Convert a string containing a list of float separated by 'sep', e.g.
  //! "1.5x2x3x4.5" into an fidxdim.
  //! \param sep Specifies the separating character, 'x' by default.
  EXPORT fidxdim string_to_fidxdim(const char *s, char sep = 'x');
  //! Convert a string containing a list of uint separated by 'sep', e.g.
  //! "1x2x3x4" into an idxdim.
  //! \param sep Specifies the separating character, 'x' by default.
  EXPORT idxdim string_to_idxdim(const string &s, char sep = 'x');
  //! Convert a string containing a list of dimension strings separated by 
  //! commas, e.g. "10x10,20x20" into a vector of idxdim.
  //! \param vecsep Specifies the vector separating character, ',' by default.
  //! \param dimsep Specifies the dim separating character, 'x' by default.
  EXPORT vector<idxdim> 
    string_to_idxdimvector(const char *s, char vecsep = ',', char dimsep = 'x');
  //! Convert a string containing a list of float dimension strings separated by
  //! commas, e.g. "1.5x1.5,2.5x2.5" into a vector of fidxdim.
  //! \param vecsep Specifies the vector separating character, ',' by default.
  //! \param dimsep Specifies the dim separating character, 'x' by default.
  EXPORT vector<fidxdim>
    string_to_fidxdimvector(const char *s, char vecsep = ',',
			    char dimsep = 'x');

  //! Convert a string containing a list of strings separated by commas, e.g.
  //! "errors,2,toto,4" into a list of strings.
  //! \param sep Specifies the separating character, ',' by default.
  EXPORT list<string> string_to_stringlist(const string &s, char sep = ',');
  //! Convert a string containing a list of strings separated by commas, e.g.
  //! "errors,2,toto,4" into a list of strings.
  //! \param sep Specifies the separating character, ',' by default.
  EXPORT list<string> string_to_stringlist(const char *s, char sep = ',');
  //! Convert a string containing a list of strings separated by commas, e.g.
  //! "errors,2,toto,4" into a vector of strings.
  //! \param sep Specifies the separating character, ',' by default.
  EXPORT vector<string> string_to_stringvector(const string &s, char sep = ',');
  //! Convert a string containing a list of strings separated by commas, e.g.
  //! "errors,2,toto,4" into a vector of strings.
  //! \param sep Specifies the separating character, ',' by default.
  EXPORT vector<string> string_to_stringvector(const char *s, char sep = ',');
  //! Convert a string containing a list of double separated by commas, e.g.
  //! "1,2,3.0,4.0" into a vector of doubles.
  EXPORT vector<double> string_to_doublevector(const string &s);
  //! Convert a string containing a list of double separated by commas, e.g.
  //! "1,2,3.0,4.0" into a vector of doubles.
  EXPORT vector<double> string_to_doublevector(const char *s);

  //! Convert a map to a string representation.
  template <typename T1, typename T2>
    EXPORT string map_to_string(map<T1,T2> &m);  
  //! Convert a map to a string representation, printing a new line between
  //! each variables/value pair.
  template <typename T1, typename T2>
    EXPORT string map_to_string2(map<T1,T2> &m);  
  //! Convert a string list to a string representation.
  EXPORT string stringlist_to_string(list<string> &l);  
  //! Convert an idx of string to a vector of strings.
  EXPORT vector<string> ubyteidx_to_stringvector(idx<ubyte> &u);
  //! Replace all occurences of 's1' by 's2' in s and return the result.
  EXPORT string string_replaceall(const string &s, const char *s1,
				  const char *s2);
  
  //////////////////////////////////////////////////////////////////////////////
  // system functions
  
  //! Tar directory dir into a .tgz archive, using directory's rightmost name,
  //! into target directory tgtdir.
  //! Return false upon failure, true otherwise.
  EXPORT bool tar(const string &dir, const string &tgtdir);
  //! Tar all files found recursively in directory dir into a compressed tar 
  //! archive "tgtdir/tgtfilename", using matching pattern 'pattern'.
  //! Return false upon failure, true otherwise.
  EXPORT bool tar_pattern(const string &dir, const string &tgtdir,
			  const string &tgtfilename, const char *pattern);

  //! Resize and fill vector v with values in l.
  template <typename T>
    void list_to_vector(list<T> &l, vector<T> &v);

  //! Return the name of a file stripped from its root.
  //! This is just looking for the first
  //! directory separator from the right, this is not doing any system calls
  //! to find the directory.
  EXPORT string filename(const char *s);
    
  //! Execute command 'cmd' and return the resulting string.
  EXPORT string system_to_string(const string &cmd);

} // end namespace ebl

#include "tools_utils.hpp"

#endif /* TOOLS_UTILS_ */
