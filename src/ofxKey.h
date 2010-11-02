/*
 *  ofxKey.h
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

#ifndef _ofxKEY
#define _ofxKEY

#include "MSACore.h"			//  Using MSACore addon made by Memo Akten, www.memo.tv


using namespace MSA;

class ofxKey{
private:
	void rBox();					// Draw the round box corner
	float scaleW;
	float scaleH;
	
public:
	string letter;
	
	float radio, angle;				// Polar position (radio, angle)
	Vec2f position;					// Cartesian Position (x,y)
	
	float width,height;				// Width and Height of the key
	
	// Contructors
	ofxKey();
	ofxKey(string _s, float _rad, float _ang, float _w, float _h);
	
	// Initial Setup. Must be set before draw!
	ofTrueTypeFont * font;		
	void setFont(ofTrueTypeFont * _font){font = _font;};
	
	Vec2f * keyboardCenter;
	void setKeyboardCenter(Vec2f * _keyboardCenter){keyboardCenter = _keyboardCenter;};
	
	float * keyboardNorth;
	void setKeyboardNorth(float * _keyboardNorth){keyboardNorth = _keyboardNorth;};
	
	const int * foregroundColor;
	const int * backgroundColor;
	void setColors(const int * _foregroundColor, const int * _backgroundColor ){ foregroundColor = _foregroundColor; backgroundColor = _backgroundColor;};
	
	//	This information is important for making new keyboard map.
	//	Don´t mess with them unless you want to make your own boards.
	void setRadio(Vec2f _position){	radio = _position.distance(*keyboardCenter);};
	void setAngle(Vec2f _position){ _position -= *keyboardCenter; angle = ( (-1*atan2(_position.x,_position.y)+(PI/2) - *keyboardNorth)); };
	
	void moveTo(int _x, int _y){moveTo(Vec2f(_x,_y));};
	void moveTo(Vec2f _location){ setRadio(_location); setAngle(_location); };
	
	void update();					// Recalculate the position and angle of the key from the center and rotation of the keyboard
	
	void draw();					// First update. Then draw.
	
	// Checkers. someday this will become events
	bool pressed;
	bool isOver(int _x, int _y){isOver(Vec2f(_x,_y));};
	bool isOver(Vec2f _location);
};

#endif

