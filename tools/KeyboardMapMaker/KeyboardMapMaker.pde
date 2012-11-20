// KeyboardMapMaker by Patricio Gonzalez Vivo
// more info: www.patriciogonzalezvivo.com
//
// This program let you make the maping for the keyboard

Key[] keys;

int currentKey = 0;

boolean border;
float   borderWidth;
float   borderHeight;
PVector kCenter = new PVector(0,0);
PVector startPoint  = new PVector(0,0);
PVector endPoint = new PVector(0,0);

PImage keyboardImage;

PFont font;

void setup(){
  keyboardImage = loadImage("keyboard.jpg");  // Load the keyboard photo
  font = createFont("Arial",12);              // Load the the font
  
  String[] data = loadStrings("map.kbd");     // Load the keyboard map file
  
  if ( data.length > 1 ){                    // Populate the information from the keyboard map file to the keys variable
    keys = new Key[data.length-1];            // Set the lenght of total key parameters
    
                                                // The first line of the file is for the border the center point and width & height
    String[] borderValues = split(data[0],","); // of the keyboard. This is usefull only to this program.
                                        
      kCenter.x = float(borderValues[0]);         // The kCenter variable it´s crucial information in order
      kCenter.y = float(borderValues[1]);         // to set the polar coordinates of each key!!
      borderWidth = float(borderValues[2]);       
      borderHeight = float(borderValues[3]);
      border = true;
    
    for(int i = 0; i < keys.length; i++){     // Here start reading and extracting the rest of the keys parameters
      String[] values = split(data[i+1],","); // remember that the fist line only have the info about the keyboard center and width&height
      float rad = float(values[1]);
      float ang   = float(values[2]);
      int w  = int(values[3]);
      int h   = int(values[4]);
      keys[i] = new Key(kCenter, values[0], rad, ang, w, h); // Finaly enter the information in to the "keys" array
    }

  } else {
    keys = new Key[0];
    border = false;                       // If the file is empty it means it have no border. So it will
                                          // alert the user to draw it first 
  }
 
  size(keyboardImage.width, keyboardImage.height);
  smooth();
}

void draw(){
  background(255);
  image(keyboardImage,0,0);
  
  fill(0);
  textFont(font);
  textAlign(LEFT);
  
  if (border) {                              // If there is a border (and center) of the keyboard tell to start populate the keys
    text("Press a key on your fisical KeyBoard to make it apear. Then drag it with the mouse to the place you like",20,40);
    text("The key-map configuration will be saved at: map.kbd" ,20,60);
    rBox((int) kCenter.x, (int) kCenter.y, (int)borderWidth, (int)borderHeight, color(255,100), color(0,200), false);
    for(int i = 0; i < keys.length; i++) keys[i].render();  // Draw the keys already made 
  } else {                                   // Otherwise it´s crucial to draw the boarder of it. In order to get the center of the keyboard
    text("Start by drawing the border of the keyboard from top left corner to right down corner",20,40);
    text("And the press enter to save it." ,20,60);
    if ((startPoint.x != endPoint.x) && ( startPoint.y != endPoint.y)){
      fill(255,70);
      stroke(255);
      rectMode(CORNER);
      rect(startPoint.x,startPoint.y, (endPoint.x-startPoint.x) , (endPoint.y-startPoint.y) );
    }
  }
  

}

// --------------------------------------------------- Mouse Events
void mousePressed(){
  if (border) {
    for(int i = 0; i< keys.length; i++)        // If the border and the center of the is setup it looks if the mouse is over a key.
      if (keys[i].isOver(new PVector(mouseX,mouseY))) currentKey = i;    //if it´s over a key it will remember his position on the array and move it where it´s dragged
  } else {
      startPoint.x = mouseX;
      startPoint.y = mouseY;
  } 
}

void mouseDragged(){
  if (border) {
    keys[currentKey].moveTo(mouseX,mouseY);
  } else {
    endPoint.x = mouseX;
    endPoint.y = mouseY;
  }
}

void mouseReleased(){
  if (border){
    saveData();
  } else {
    endPoint.x = mouseX;
    endPoint.y = mouseY;
  }
}

// --------------------------------------------------- Keyboard Event
void keyPressed(){
  if (border) {  
    Key t = new Key(kCenter,""+key+"");      //For each key is pressed create a key with the same letter
    t.moveTo(mouseX,mouseY);
    keys = (Key[]) append(keys,t); 
    saveData();
  } else if ((startPoint.x < endPoint.x) && (startPoint.y < endPoint.y) && ( keyCode == ENTER )) {
    borderWidth = endPoint.x - startPoint.x;
    borderHeight= endPoint.y - startPoint.y;
    kCenter = new PVector(borderWidth/2 + startPoint.x,borderHeight/2 + startPoint.y);
    border = true;
  }
}

void saveData() {
  if (border) {                                  // Only store the data if there is a border of the keyboard (that´s have to be the very first step )
    String[] data = new String[keys.length+1];   // The first line will store the center and the borders of the keyboard
    data[0] = kCenter.x + "," + kCenter.y + "," + borderWidth + "," + borderHeight;
  
    for (int i = 0; i < keys.length; i++) {      // Then writes the information of each key
      data[i+1] = keys[i].s + "," + keys[i].rad + "," + keys[i].ang + "," + keys[i].w + "," + keys[i].h;
    }
    
    saveStrings("data/map.kbd",data);            // Finaly write all the info to the file
  }
}

void close(){
  saveData();    // This could prevent data lost
}
