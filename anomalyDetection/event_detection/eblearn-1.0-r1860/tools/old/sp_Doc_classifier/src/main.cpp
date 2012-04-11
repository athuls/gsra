/***************************************************************************
 *   Copyright (C) 2008 by Cyril Poulet   *
 *   cyril.poulet@centraliens.net   *
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

#include "preprocess.h"
#include "spex.h"

//! this global variable is the number of data samples that will be loaded in
//! the training database.
intg trainsize = 20000;
intg testsize = 10000;

//! the first argument is the path to the data ( ../eblearn/demos/sp_Doc_classifier/data
//! if you follow the instructions in the Readme), the second argument is optional :
//! write "true" if you need to preprocess the data (the first time you use
//! the application)
int main(int argc, const char **argv){
	string path;
	bool preproc = false;
	for (int i=1; i<argc; i++)
	{
		const char *arg = argv[i];
		if(i == 1) path = arg;
		if(i == 2){
			if(arg == "true") preproc = true;
		}
	}
	if(preproc) preprocess(path);

	string trainfile = path + "/test.dat.gz";
	string testfile = path + "/train.dat.gz";
	sptrainer mytrainer(trainfile, testfile);
	mytrainer.train(1);

	return 0;
}
