#pragma once

#include "ofMain.h"
#include "delayCircle.h"
#include "ofxStk.h"

#define MAX_CIRCLES 5

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
        float volume;
    
        ofSoundStream soundStream;

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
