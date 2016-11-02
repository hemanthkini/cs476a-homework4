#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // initialize circle stuff
    num_circles = 0;
    ofRandom(1337);
    rolling_ptr = 0;
    for (int i = 0; i < MAX_CIRCLES; i++) {
        delay_id_queue.push(i);
    }

    // initialize audio
    
    // 2 output channels,
    // 0 input channels
    // 44100 samples per second
    // 512 samples per buffer
    // 4 num buffers (latency)
    
    int bufferSize		= 512;
    sampleRate 			= 44100;
    nInputChans         = 2;
    volume				= 0.5f;
    
    // TODO remove if we use STK delays
    /*
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    audioBuffer = new float*[2];
    audioBuffer[0] = &lAudio[0];
    audioBuffer[1] = &rAudio[0];
    */
     
    soundStream.printDeviceList();
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
    // TODO implement sound playing 
    
    ofSetFrameRate(60);
    stk::Stk::setSampleRate(44100.0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);  // Clear the screen with a black color


    // draw each circle
    int i = 0;
    for (i = 0; i < num_circles; i++) {
        circleVector[i]->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'd') {
        if (num_circles == 0) {
            return;
        }
        
        for (int i = num_circles - 1; i >= 0; i--) {
            if (circleVector[i]->within(ofGetMouseX(),ofGetMouseY())) {
                delay_id_queue.push(circleVector[i]->getIndex());
                circleVector.erase(circleVector.begin() + i);
                num_circles = num_circles - 1;
                return;
            }
        }
        
        delay_id_queue.push(circleVector[0]->getIndex());
        circleVector.erase(circleVector.begin());
        num_circles = num_circles - 1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    circleVector[selected_index]->setXYandUpdate(x + selected_x,y + selected_y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    int i = 0;
    
    // Check if we touched a circle that exists, and update it if so
    for (i = num_circles - 1; i >= 0; i--) {
        if (circleVector[i]->within(x,y)) {
            // push the selected circle so it's drawn on top
            circleVector.push_back(circleVector[i]);
            selected_x = circleVector[i]->getX() - x;
            selected_y = circleVector[i]->getY() - y;
            circleVector.erase(circleVector.begin() + i);
            selected_index = num_circles;
            
            return;
        }
    }

    // if we've not maxed out the number of delayCircles, draw one
    if (num_circles < MAX_CIRCLES) {
        circleVector.push_back(new delayCircle(x, y, delay_id_queue.front()));
        selected_index = num_circles;
        num_circles = num_circles+1;
        selected_x = 0;
        selected_y = 0;
    }
    
    // else, pop the head off and update it.
    else 
    {
        delay_id_queue.push(circleVector[0]->getIndex());
        circleVector.erase(circleVector.begin());
        circleVector.push_back(new delayCircle(x,y, delay_id_queue.front()));
        delay_id_queue.pop();
        selected_index = num_circles - 1;
        selected_x = 0;
        selected_y = 0;
    }
    
    
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
    // TODO load and start playing soundfile here.
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // Move most recent circle to the back of queue - it gets drawn on top
    circleVector.push_back(circleVector[selected_index]);
    circleVector.erase(circleVector.begin() + selected_index);
    
    // TODO Implement deletion here.
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {
    // TODO get audio playing
    for(int i = 0; i < bufferSize * nChannels; i += 2) {
        output[i] = 0.0 * volume; // writing to the left channel
        output[i+1] = 0.0 * volume; // writing to the right channel
    }
}
