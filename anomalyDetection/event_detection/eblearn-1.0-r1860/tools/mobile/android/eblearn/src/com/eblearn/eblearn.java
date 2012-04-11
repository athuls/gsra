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

package com.eblearn;

import java.util.Iterator;
import android.app.Activity;
import android.widget.TextView;
import android.widget.ImageView;
import android.os.Bundle;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;

// for assets extraction
import java.io.FileDescriptor;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import java.lang.Exception;

import android.graphics.Paint;
import android.graphics.Canvas;
import android.view.View;
import android.content.Context;
import android.graphics.Color;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.text.Editable;

import java.util.ArrayList;

import android.graphics.Rect;

public class eblearn extends Activity {
    static final String tag = "eblearn";
    String url = "http://cs.nyu.edu/~sermanet/face/0.png";
    // String url = "http://www.fbi.gov/headlines/fames_faces500.jpg";
    Bitmap bm;
    bboxview bbview;
    bboxes bb;
    int ret = 0;
    float threshold = (float) -.1;
    AlertDialog.Builder alert, erralert;
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
	initLearning(); // load learning data
	// download default image
	bm = downloadBitmap(url);
	// init view
	bbview = new bboxview(this);
	bbview.setData(bm, bb, url);
	setContentView(bbview);

	// get user input
	alert = new AlertDialog.Builder(this);
	alert.setTitle("Face detection input");
	alert.setMessage("Enter the URL of an image:\n"
			 + "(available: 0.png, 1.png, ...)");
	final EditText input = new EditText(this);
	input.setText(url);
	alert.setView(input);

	erralert = new AlertDialog.Builder(this);
	erralert.setTitle("Face detection input");
	erralert.setMessage("Error while processing image.");

	// Positive
	alert.setPositiveButton("Process",new DialogInterface.OnClickListener(){
		public void onClick(DialogInterface dialog, int whichButton) {
		    url = input.getText().toString();
		    bm = downloadBitmap(url);
		    bb.clear();
		    bbview.setData(bm, bb, url);
		    ret = process(bm);
		    if (ret < 0)
			erralert.show();
		    else
			setContentView(bbview);
		}});
	// Positive
	erralert.setPositiveButton("Ok",new DialogInterface.OnClickListener(){
		public void onClick(DialogInterface dialog, int whichButton) {
		}});
	alert.show();
    }

    ///////////////////////////////////////////////////////////////////////////
    // native functions

    public native int detect(Bitmap intidx, FileDescriptor fd1, long off1,
			     FileDescriptor fd2, long off2,
			     FileDescriptor fd3, long off3, bboxes bb,
			     float threshold);

    ///////////////////////////////////////////////////////////////////////////
    // network
    
    private AssetFileDescriptor raw_net, raw_netclass, raw_conf;
    private FileDescriptor fd_net, fd_netclass, fd_conf;
    private long off_net, off_netclass, off_conf;
    
    // load learning data
    private void initLearning() {
	bb = new bboxes();
	// loading learning libraries
        System.loadLibrary("idx");
	System.loadLibrary("eblearn");
	// loading learned configuration
	AssetManager man = getAssets();
	try {
	    String net = "face.mat.mp3", netclass = "face_classes.mat.mp3",
		netconf = "face.conf.mp3";
	    raw_net = man.openFd(net);
	    raw_netclass = man.openFd(netclass);
	    raw_conf = man.openFd(netconf);
	    fd_net = raw_net.getFileDescriptor();
	    fd_netclass = raw_netclass.getFileDescriptor();
	    fd_conf = raw_conf.getFileDescriptor();
	    off_net = raw_net.getStartOffset();
	    off_netclass = raw_netclass.getStartOffset();
	    off_conf = raw_conf.getStartOffset(); 
	    Log.i(tag, "Loaded assets " + net + ", " + netclass
		  + " and " + netconf);
	} catch(Exception e) {
	    Log.e(tag, "failed to load weights from assets: " + e);
	}
    }

    // process an image
    public int process(Bitmap b) {
	int z = detect(b, fd_net, off_net, fd_netclass, off_netclass,
		       fd_conf, off_conf, bb, threshold);
	Log.i(tag, "Detected " + z + " faces");
	return z;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // get an image off the web
    
    Bitmap downloadBitmap(String fileUrl) {
	URL myFileUrl = null;
	Bitmap b = null;
	try {
	    myFileUrl= new URL(fileUrl);
	} catch (MalformedURLException e) {
	    e.printStackTrace();
	}
	try {
	    Log.i(tag, "Downloading " + fileUrl);
	    HttpURLConnection conn =
		(HttpURLConnection) myFileUrl.openConnection();
	    conn.setDoInput(true);
	    conn.connect();
	    int length = conn.getContentLength();
	    InputStream is = conn.getInputStream();
            
	    Bitmap tmp = BitmapFactory.decodeStream(is);
	    Log.i(tag, "Got image: " + tmp.getHeight() + "x" + tmp.getWidth()
		  + " density: " + tmp.getDensity()
		  + " config: " + tmp.getConfig());
	    b = tmp.copy(Bitmap.Config.ARGB_8888, true);
	    if (b == null)
		Log.e(tag, "could not convert to config");
	    Log.i(tag, "Got image: " + b.getHeight() + "x" + b.getWidth()
		  + " density: " + b.getDensity()
		  + " config: " + b.getConfig());
          } catch (IOException e) {
	    e.printStackTrace();
	}
	return b;
    }

    ///////////////////////////////////////////////////////////////////////////
    // painting
    
    private class bboxview extends View {
	private bboxes bb;
	private Bitmap bm;
	private String s;
	
	public bboxview(Context c) {
	    super(c);
	}

	public void setData(Bitmap bmap, bboxes b, String msg) {
	    bm = bmap;
	    bb = b;
	    s = msg;
	}

	@Override protected void onDraw(Canvas canvas) {
	    super.onDraw(canvas);	
	    Paint p = new Paint();
	    p.setStyle(Paint.Style.STROKE); // don't fill rects
	    p.setColor(Color.BLUE);

	    canvas.drawBitmap(bm, 0, 0, p);
	    
	    Iterator i = bb.iterator();
	    while (i.hasNext()) {
		bbox b = (bbox)i.next();
		canvas.drawRect(b.r, p); 
	    }
	    p.setColor(Color.WHITE);
	    canvas.drawText(s, 0, 12, p);
	}
    }

    ///////////////////////////////////////////////////////////////////////////
    // bboxes

    public class bbox {
	public bbox(float conf, int h0, int w0, int height, int width) {
	    r = new Rect(w0, h0, w0 + width, h0 + height);
	    confidence = conf;
	}
    
	public Rect r;
	public float confidence;
    }
    
    public class bboxes extends ArrayList<bbox> {
	public bboxes() {
	}

	public void add(float conf, int h0, int w0, int height, int width) {
	    add(new bbox(conf, h0, w0, height, width));
	}
    }
}