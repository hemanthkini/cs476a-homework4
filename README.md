#CS476a-homework4

An interactive drag-and-drop music player with echo effects. Drag and drop any mono or stereo WAV file and enjoy.
hkini [at] stanford

## Installation
Make sure you have OpenFrameworks installed.

1. Clone the repository.

2. Copy the rawwaves folder into bin/data of the project (see the ofxStk readme, on GitHub).

3. Compile.

4. Run the program once - you may not hear any sound. If so, in the setup() method in ofApp.cpp, make sure you’re selecting the right audio output device. (Running the app once will print your output devices to the console.)

5. Enjoy! 

# Acknowledgements

I used and modified the openFrameworks starter code provided by Tim and Romain to drawing and artwork, and used ofxStk. The alpha-motion-blurring technique came from a tutorial online as well.


