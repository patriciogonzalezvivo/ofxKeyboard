/*
 *  ofxKeyboard.cpp
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

#include "ofxKeyboard.h"

ofxKeyboard::ofxKeyboard(){
	foregroundColor = 0xffffff;
	backgroundColor = 0x000000;
	
	angle = 0;
	
	position.x = ofGetWidth()/2;
	position.y = ofGetHeight()/2;
	
	width = 0;
	height = 0;
}

void ofxKeyboard::loadmap(const string& path){
	ifstream	fs( ofToDataPath(path).c_str());
	
	cKey = NULL;
	nKeys = 0;
	
	string line;
	int lineCounter = 0;
	
	while(!(fs >> line).fail()){
		vector <string> values = ofSplitString(line, ",");
		
		if ( (lineCounter == 0) && (values.size()) == 4) {
			width = ofToFloat(values[2]);								// The first line of the file have information about the 
			height = ofToFloat(values[3]);								// the width and the height of the keyboard. (The first two values are a relative position of the keyboard center when was draw)
		} else if ((values.size() == 5)) {
			string _letter = values[0];
			float _radio	= ofToFloat(values[1].c_str());             // the first value is the Letter or Key, the second one is radio from the center
			float _angle = ofToFloat(values[2].c_str());				// the 3th is the angle on radians from the center (this last to parameters are the ones that let the keabord be redraw in any angle and continue working)
			int	_width	= ofToInt(values[3].c_str());               // the 4th and 5th are the with and height of each key. 
			int	_height	= ofToInt(values[4].c_str());
			
			ofxKey k = ofxKey(_letter, _radio, _angle, _width, _height);
			k.setFont(&font);
			k.setKeyboardNorth(&angle);
			k.setKeyboardCenter(&position);
			k.setColors(&foregroundColor, &backgroundColor);
			k.update();
			keys[nKeys++] = k;
		}
		lineCounter++;
	}

	fs.seekg(0,ios::beg);
	fs.clear();
	fs.close();
}

void ofxKeyboard::resize(float _resize){
	width *= _resize;
	height *= _resize;
	
	for (int i = 0; i < nKeys; i++){
		keys[i].scale(_resize);
		keys[i].update();
	}	
}

void ofxKeyboard::draw(){
	// Draw the keyboard frame
	ofPushMatrix();
		ofTranslate(position.x,position.y);
		ofRotateZ(ofRadToDeg(angle));
	
		ofFill();
		ofSetColor(backgroundColor);
    
    ofRectRounded(-width*0.5, -height*0.5, width, height, 20);
	
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(foregroundColor);
    ofRectRounded(-width*0.5, -height*0.5, width, height, 20);
	ofPopMatrix();
	
	for(int i = 0; i < nKeys; i++)
		keys[i].draw();
}
void ofxKeyboard::addKey(string _letter, int _x, int _y, int _width, int _height){
	ofxKey k = ofxKey(_letter, 0, 0, _width, _height);
	k.setFont(&font);
	k.setKeyboardNorth(&angle);
	k.setKeyboardCenter(&position);
	k.moveTo(_x, _y);
	k.setColors(&foregroundColor, &backgroundColor);
	k.update();
	
	cKey = nKeys;
	keys[nKeys++] = k;
	
}

void ofxKeyboard::savemap(const string& path){
	ofstream	fs( ofToDataPath(path).c_str());
	
	fs << position.x << "," << position.y << "," << width << "," << height << endl; 
	for (int i = 0; i < nKeys; i++){
		fs << keys[i].letter << "," << keys[i].radio << "," << keys[i].angle << "," << keys[i].width << "," << keys[i].height << endl;
	}
	
	fs.close();
}

bool ofxKeyboard::isOver(ofPoint _loc){
	ofPoint dirToCenter = position - _loc;
	float theta = atan2(dirToCenter.x,dirToCenter.y)-(PI/2);
	float r = dirToCenter.length();
	float x = r * cos(theta + angle);
	float y = r * sin(theta + angle);
	
	if ( (x <= width/2) && (x >= -width/2) && (y <= height/2) && (y >= -height/2)) return true;
	else return false;
}

bool ofxKeyboard::isOnBorder(ofPoint _loc){
	float dist = position.distance(_loc);
	
	if ( (dist <= width*0.5) && (dist >= width*0.4)) return true;
    else return false;
}

bool ofxKeyboard::checkKeys(ofPoint _loc){
	bool pressed = false;
	
	if (isOver(_loc))
		for(int i = 0; i< nKeys; i++)
			if (keys[i].isOver(_loc)){
				ofNotifyEvent(keyPressed, keys[i].letter,this);
				pressed = true;
				cKey = i;
			}
	
	return pressed;
}

#ifdef USE_TUIO
void ofxKeyboard::setTuioClient (ofxTuioClient * _tuioClient){
	tuioClient = _tuioClient;
	
	// HELP with this in order to be independent from the main.
	ofAddListener(tuioClient->cursorAdded,this,&ofxKeyboard::tuioAdded);
	ofAddListener(tuioClient->cursorRemoved,this,&ofxKeyboard::tuioRemoved);
	ofAddListener(tuioClient->cursorUpdated,this,&ofxKeyboard::tuioUpdated);
}

void ofxKeyboard::tuioAdded(ofxTuioCursor &tuioCursor){
	ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
	
	if (isOver(loc))					// IF the cursor is over the keyboard
		if (!checkKeys(loc)){			// ... and is not pressing a key
			//if (isOnBorder(loc)){		// ... and wish location is near the borders 
				tCursor c;
				c.idN = tuioCursor.getSessionId();
				c.loc = loc;
				
				if (cursorsOnBorder.size() == 0){	// If it´s the first finger over the border it will save remember it
					cursorsOnBorder.push_back(c);
				} else if (cursorsOnBorder.size() == 1){	// If it´s the second one it will check if it´s at the right distance
						cursorsOnBorder.push_back(c);
						
						oldLoc[0] = cursorsOnBorder[0].loc;
						oldLoc[1] = cursorsOnBorder[1].loc;
				}
			//}
		}
}
				
void ofxKeyboard::tuioUpdated(ofxTuioCursor &tuioCursor){
	// First it will update the information of the fingers that are over the border
	for ( int i = 0; i < cursorsOnBorder.size(); i++)
		if (cursorsOnBorder[i].idN == tuioCursor.getSessionId())
			cursorsOnBorder[i].loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());

	// Then check if there is two fingers over it
	if (cursorsOnBorder.size() == 2){
		int sta = -1;	// a NULL parameter
		int din = -1;	// a NULL parameter
		
		// First it will watch witch one is the static finger and witch one is the dinamic
		for ( int i = 0; i < cursorsOnBorder.size(); i++)
			if (cursorsOnBorder[i].loc == oldLoc[i]) sta = i;
			else din = i;
		
		// If one it´s the dinamic and the other one the static it´s all OK and it´s time to calculate new parameters
		if ((sta != -1) && (din != -1) && (din != sta)){
			ofPoint Di = oldLoc[sta] - oldLoc[din];		// Get Vector between OLD finger position
			ofPoint Dii = oldLoc[sta] - cursorsOnBorder[din].loc; // Get Vector between NEW finger position
		
			float di = Di.length();						// Get OLD finger distance
			float dii = Dii.length();					// Get NEW finger distance
			
			float scaleF = dii / di;					// Set the scale diference before and after
			
			float ri = -1*atan2(Di.x,Di.y)+(PI/2);		// Get OLD fingers inclination
			float rii = -1*atan2(Dii.x,Dii.y)+(PI/2);	// Get NEW fingers inclination
			
			float rotateF = rii - ri;					// Set the angle diference before and after
			
			ofPoint oldStaCursorToCenter = position - oldLoc[sta] ;	// Get the OLD vector from the static finger to the center of the keyboard
			float oldAngleToCenter = -1*atan2(oldStaCursorToCenter.x,oldStaCursorToCenter.y)+(PI/2);	// Get OLD Angle to the center 
			float oldRadioToCenter = oldStaCursorToCenter.length();		// Get the OLD distance from the static figer to the center
			
			float newRadioToCenter = oldRadioToCenter * scaleF;		// Set the NEW distance to the center
			float newAngleToCenter = oldAngleToCenter + rotateF;	// Set the NEW angle to the center
			ofPoint newStaCursorToCenter = ofPoint(newRadioToCenter*cos(newAngleToCenter),newRadioToCenter*sin(newAngleToCenter)); // Set the NEW vector from the static finger to the center of the keyboard
			
			resize(scaleF);											// RESIZE the scale diference proportion
			rotate(rotateF);										// ROTATE the diference in angle
			position = oldLoc[sta] + newStaCursorToCenter;			// MOVE	the draged distance
		}
			
		oldLoc[0] = cursorsOnBorder[0].loc;
		oldLoc[1] = cursorsOnBorder[1].loc;
	}
}

void ofxKeyboard::tuioRemoved(ofxTuioCursor &tuioCursor){
	for (int i = 0; i < cursorsOnBorder.size(); i++ )
		if (cursorsOnBorder[i].idN == tuioCursor.getSessionId())
			cursorsOnBorder.erase(cursorsOnBorder.begin()+i);
}
#endif