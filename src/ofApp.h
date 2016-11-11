#pragma once

#include "ofMain.h"
#include "delayCircle.h"
#include "ofxStk.h"

#define MAX_CIRCLES 15

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

class ofApp : public ofBaseApp{
    private:
        int num_circles;
        vector<delayCircle *> circleVector;
        int rolling_ptr;
        queue<int> delay_id_queue;
        int selected_index;
        int selected_x;
        int selected_y;
    
        // audio stuff
        int	sampleRate, nInputChans;
        int bufferSize;
        float volume;
        int fileNumChannels;
        bool fileLoaded;

        stk::Echo delays[MAX_CIRCLES * 2];
        float delay_levels[MAX_CIRCLES];
        float delay_lengths[MAX_CIRCLES];
    
        ofSoundStream soundStream;
    
        stk::FileWvIn fileInput;
    
    // TODO remove if we use STK
    /*
        vector <float> lAudio;
        vector <float> rAudio;
    
        float **audioBuffer; // The 2d audio buffer that Faust wants to work with
     */
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void audioOut(float * input, int bufferSize, int nChannels);
		
};
