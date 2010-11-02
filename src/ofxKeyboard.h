/*
 *  ofxKeyboard.h
 *
 *  Copyright 2010 Patricio Gonzalez Vivo http://www.patriciogonzalezvivo.com
 *	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#ifndef _ofxKEYBOARD
#define _ofxKEYBOARD

#define USE_TUIO

#ifdef USE_TUIO
#define tuioCursorSpeedMult				0.5	// the iphone screen is so small, easy to rack up huge velocities! need to scale down 
#define tuioStationaryForce				0.001f	// force exerted when cursor is stationary
#include "ofxTuio.h"
#endif

#include "MSACore.h"			//  Using MSACore addon made by Memo Akten, www.memo.tv

#include "ofxKey.h"

#include <iostream>
#include <fstream>

using namespace MSA;

struct tCursor {
	int	idN;
	Vec2f loc;
};

class ofxKeyboard{
private:	
	void	rBox();								// Draws a rounded box (Mac style)
	
public:
	int		nKeys;								// Number of loaded keys 
	ofxKey	keys[200];							// Array of keys
	int		cKey;								// Position on the array of the selected key
	
	Vec2f	position;							// Cartesian coordinates of the center of the keyboard
	float	angle;								// The keyboard could be rotated. Because is constructed using polar geometry it won´t lose funtionality
	
	float	width,height;						// Width and Height of the keyboard
	int		foregroundColor,backgroundColor;	// Hexcode of the fore and background
	
	ofTrueTypeFont font;
	
	ofEvent<string> keyPressed;					// Key Event

	ofEvent<float>  scaling;
	ofEvent<float>	rotation;
	ofEvent<Vec2f>	moving;
	
	ofxKeyboard();
	
	// Initial setup. It must be set before updating or drawing;
	void loadmap(const string& filePath);
	void loadfont(const string& fontPath){font.loadFont(fontPath,15,true,true,true);};
	
	// Acctions
	void rotate(float _angle){ angle += _angle;};
	void resize(float _resize);
	void draw();
	
	// Edit keyboard actions
	void addKey(string _letter, int _x, int _y, int _width, int _height);				// Pass the cartesian x and y cordinates of the screen
	void moveKeyTo(int _indexKey, int _x, int _y ){ keys[_indexKey].moveTo(_x,_y); };	// Pass the cartesian x and y cordinates of the screen
	void moveCurrentKeyTo(int _x, int _y ){ keys[cKey].moveTo(_x,_y); };
	void savemap(const string& filePath);
	
	// Checks things
	bool isOver(Vec2f _loc);
	bool isOver(int _x, int _y){isOver(Vec2f(_x,_y));};
	
	bool isOnBorder(Vec2f _loc);
	bool isOnBorder(int _x, int _y){isOnBorder(Vec2f(_x,_y));};
	
	bool checkKeys(Vec2f _loc);
	bool checkKeys(int _x, int _y){checkKeys(Vec2f(_x,_y));};
	
#ifdef USE_TUIO
	myTuioClient * tuioClient;
	void	setTuioClient (myTuioClient * _tuioClient);
	
	vector<tCursor>	cursorsOnBorder;
	Vec2f	oldLoc[3];
	
	// TUIO Events Handlers
	void	tuioAdded(ofxTuioCursor & tuioCursor);
	void	tuioRemoved(ofxTuioCursor & tuioCursor);
	void	tuioUpdated(ofxTuioCursor & tuioCursor);
#endif
};

#endif
