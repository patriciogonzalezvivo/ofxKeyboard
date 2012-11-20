#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    
    tuioClient.start(3333);
    
    keyboard.loadmap( "mac.kbd" );
    keyboard.loadfont( "helvetica.ttf");
    keyboard.setTuioClient( &tuioClient );
    ofAddListener(keyboard.keyPressed,this,&testApp::virtualKeyPressed);
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    keyboard.draw();
    tuioClient.drawCursors();
}

void testApp::virtualKeyPressed(string & letter){
	cout << letter << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}