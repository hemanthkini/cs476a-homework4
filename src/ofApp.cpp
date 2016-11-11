#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // initialize circle stuff
    num_circles = 0;
    ofRandom(1337);
    rolling_ptr = 0;
    for (int i = 0; i < MAX_CIRCLES; i++) {
        delay_id_queue.push(i);
        delays[i].clear();
        delays[i+1].clear();
        delays[i].setMaximumDelay(44100);
        delays[i+1].setMaximumDelay(44100);
        delay_levels[i] = 0.0;
        delay_lengths[i] = 0.5;
    }

    ofLog(OF_LOG_NOTICE, "INITIALIZING...");
    
    // initialize audio
    
    // 2 output channels,
    // 0 input channels
    // 44100 samples per second
    // 512 samples per buffer
    // 4 num buffers (latency)
    
    bufferSize		= 256;
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
    stk::Stk::setSampleRate((float)sampleRate);
    
    
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
    
    // Delete a circle
    if (key == 'd') {
        if (num_circles == 0) {
            return;
        }
        
        // Attempt to delete the circle that our mouse is over
        for (int i = num_circles - 1; i >= 0; i--) {
            if (circleVector[i]->within(ofGetMouseX(),ofGetMouseY())) {
                int delayIndex = circleVector[i]->getIndex();
                delay_levels[delayIndex] = 0.0;
                delay_id_queue.push(delayIndex);
                circleVector.erase(circleVector.begin() + i);
                num_circles = num_circles - 1;
                return;
            }
        }
        
        // Otherwise, delete the least-recent circle we modified
        int delayIndex = circleVector[0]->getIndex();
        delay_levels[delayIndex] = 0.0;
        delay_id_queue.push(circleVector[0]->getIndex());
        circleVector.erase(circleVector.begin());
        num_circles = num_circles - 1;
    }
    
    // Close the currently playing song.
    if (key == 'c') {
        fileInput.closeFile();
        fileLoaded = false;
    }
    
    // Pause the song.
    if (key == 'p') {
        if (fileInput.isOpen())
            fileLoaded = !fileLoaded;
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
    int delayIndex = circleVector[selected_index]->getIndex();
    delay_levels[delayIndex] = circleVector[selected_index]->getEchoMix();
    delay_lengths[delayIndex] = circleVector[selected_index]->getEchoDelay();
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
        circleVector.push_back(new delayCircle(x, y, delay_id_queue.front(), WINDOW_WIDTH, WINDOW_HEIGHT, sampleRate));
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
        circleVector.push_back(new delayCircle(x,y, delay_id_queue.front(), WINDOW_WIDTH, WINDOW_HEIGHT, sampleRate));
        delay_id_queue.pop();
        selected_index = num_circles - 1;
        selected_x = 0;
        selected_y = 0;
    }
    
    int delayIndex = circleVector[selected_index]->getIndex();
    delay_levels[delayIndex] = circleVector[selected_index]->getEchoMix();
    delay_lengths[delayIndex] = circleVector[selected_index]->getEchoDelay();
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
    // TODO load and start playing soundfile here.
    string filePath = dragInfo.files[0];
    ofLog(OF_LOG_NOTICE, "String filepath =  %s", filePath.c_str());
    // TODO force that it's .wav
    //if (filePath.endswith)
    
    // Query for number of channels in file (1 or 2)
    stk::FileRead fr;
    fr.open(filePath);
    fileNumChannels = fr.channels();
    fr.close();
    
    // Load file and signal playback
    fileInput.openFile(filePath);
    fileLoaded = true;
    float nFrames = fileInput.getSize();
    ofLog(OF_LOG_NOTICE, "File size in frames: %f\tnumChannels: %d", nFrames, fileNumChannels);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // Move most recent circle to the back of queue - it gets drawn on top
    circleVector.push_back(circleVector[selected_index]);
    circleVector.erase(circleVector.begin() + selected_index);
    
    // Update all delays.
    for (int i = 0; i < MAX_CIRCLES; i++) {
        delays[i*2].setDelay((int)(delay_lengths[i] * sampleRate));
        delays[(2*i)+1].setDelay((int)(delay_lengths[i] * sampleRate));
    }
    ofLog(OF_LOG_NOTICE, "%d", (int)(delay_lengths[0] * sampleRate));
    
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
    if (fileLoaded) {
        //ofLog(OF_LOG_NOTICE, "PLAYING");
        for(int i = 0; i < bufferSize * nChannels; i += 2) {
            output[i] = fileInput.tick(0) * volume; // writing to the left channel
            if (fileNumChannels == 2)
                output[i+1] = fileInput.tick(1) * volume; // writing to the right channel

            for (int j = 0; j < MAX_CIRCLES; j++) {
                output[i] = (output[i] * (1.0 - delay_levels[j])) + (delays[j * 2].tick(output[i]) * delay_levels[j]);
                if (fileNumChannels == 2) {
                    output[i+1] = (output[i+1] * (1.0 - delay_levels[j])) + (delays[j * 2 +1].tick(output[i+1]) * delay_levels[j]);
                }
                else {
                    output[i+1] = output[i];
                }
            }
            
        }
    }
}
