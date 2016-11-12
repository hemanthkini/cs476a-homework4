#pragma once

#include "ofMain.h"
#include "delayCircle.h"
#include "ofxStk.h"

#define MAX_CIRCLES 10

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define MY_PIE           3.14159265358979 // for convenience

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
        string filePath;

        stk::Echo delays[MAX_CIRCLES * 2];
        float delay_levels[MAX_CIRCLES];
        float delay_lengths[MAX_CIRCLES];
    
        // Angle for the starting draw point of the circle. 
        float angle;
    
        ofSoundStream soundStream;
    
        stk::FileWvIn fileInput;
    
        // Vectors for our waveform
        vector<float> audio;
        vector<float> bufferedAudio;
    
        // Double vectors to save previous waveforms, for plotting
        vector< vector<float> > audioHistory;
    
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
