#ofxKeyboard

This is a very old multiTouch keyboard project that starts with a Processing sketch:

[http://openprocessing.org/visuals/?visualID=10188](http://openprocessing.org/visuals/?visualID=10188)
[http://openprocessing.org/visuals/?visualID=10189](http://openprocessing.org/visuals/?visualID=10189)

The idea is to make a keyboard map maker in order to make my own distribution of keys for a super configurable tools. So, you can make your owns keyboards, control remotes, mouse or drag-able control panels for any multiTouch/TUIO projects.

After the migration to OF It still use the same type of config file (one that ends with .kbd) for storing the key distribution. For it, uses polar geometry in order to let it be rotate, scale and drag and continue to be 100% functional.
Note about the .kbd: The first line have the information of the keyboard. Center x and y, the width and height. And the other ones are the keys with their position (on radio and angle), width and height.

On openFrameworks uses ofEvents to handles the pressed keys and need the next addons:

- ofxOsc
- [ofxTuio](https://github.com/patriciogonzalezvivo/ofxTuio)
