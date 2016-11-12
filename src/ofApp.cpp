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
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    bufferSize		= 256;
    sampleRate 			= 44100;
    nInputChans         = 2;
    volume				= 0.5f;
    
     
    soundStream.printDeviceList();
    
    // INSERT THE DEVICE YOU WANT TO PLAY ON HERE!
    soundStream.setDeviceID(4);
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    
    
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
    
    
    ofSetFrameRate(60);
    stk::Stk::setSampleRate((float)sampleRate);
    
    // Initialize empty audio buffer to draw from.
    
    for (int i = 0; i < bufferSize + sampleRate; i++) {
        audio.push_back(0.0);
    }
    
    // Alpha blend!
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetBackgroundAuto(false);
    
    angle = MY_PIE;
    
    
    ofBackground(0);  // Clear the screen with a black color
}

//--------------------------------------------------------------
void ofApp::update(){
    
    bufferedAudio = audio;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    angle = MY_PIE;
    
    int wh = ofGetWindowHeight();
    int ww = ofGetWindowWidth();
    
    // Fake motion blur by applying a black rectangle with
    // low alpha over every frame, as the background.
    float waveform_height   = 0.5 * wh;
    float waveform_width = 0.5 * ww;
    ofSetColor(0,0,0,75);
    ofDrawRectangle(0,0,ww,wh);
    
    

    // Draw main audio visualization
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(waveform_width, waveform_height, 0);
    
    ofSetLineWidth(5);
    ofNoFill();
    int radius = (wh < ww ? wh / 4 : ww / 4);
    int extension = radius * 2;
    ofSetColor(47, 245, 60, 164);
    ofBeginShape();
    int circleLen = sampleRate / ((unsigned int)ofGetTargetFrameRate());
    float incr = MY_PIE * 2.0 / ((float)(circleLen + 1));

    for (unsigned int i = 0; i < circleLen; i++){
        float amp = bufferedAudio[bufferSize + sampleRate - (i+1)];
        // Can replace amp with summed, normalized amp (over buffersize) for cool effect
        float ampRadius = extension * amp +radius;
        ofVertex(ampRadius * cos(this->angle),
                 ampRadius * sin(this->angle),
                 0);
        this->angle = this->angle + incr;
        
    }
    ofEndShape(true);
    
    
    ofPopMatrix();
    ofPopStyle();
    
    
    // Draw each delay's circle
    ofPushStyle();
    ofPushMatrix();
    // Cap circle length at buffersize, since, it might be far back
    // and we don't want to overflow our audio array when wrapping
    // each circle in its waveform visualization
    circleLen = (circleLen < bufferSize ? circleLen : bufferSize);
    incr = MY_PIE * 2.0 / ((float)(circleLen + 1));
    
    // Fill in the circle when paused, otherwise don't.
    if (!fileLoaded)
        ofFill();
    else {
        ofNoFill();
        ofSetLineWidth(5);
    }
    
    int i = 0;
    for (i = 0; i < num_circles; i++) {
        delayCircle* cir = circleVector[i];
        cir->draw(); // No longer draws circle, but sets color
        ofBeginShape();
        
        for (unsigned int i = 0; i < circleLen; i++){
            float amp = 0.0;
            // Need to get amp based on how far back the delay is, so it's
            // accurately showing the audio at a given time
            if (fileLoaded) {
                amp = bufferedAudio[bufferSize + sampleRate - ((i+1) + (cir->getEchoDelay() * sampleRate))];
                
            }
            
            float ampRadius = (cir->getRadius() * amp * 3) + cir->getRadius();
            ofVertex(ampRadius * cos(this->angle) + cir->getX(),
                     ampRadius * sin(this->angle) + cir->getY(),
                     0);
            this->angle = this->angle + incr;
            
        }
        ofEndShape(true);
        
    }
    ofPopMatrix();
    ofPopStyle();
 
    // Draw text.
    
    ofSetColor(255, 255, 255, 255);
    if (fileInput.isOpen()) {
        if (fileLoaded) {
            ofDrawBitmapString("p to pause, c to close file", 10, wh - 10);
        }
        else {
            ofDrawBitmapString("p to play, c to close file", 10, wh - 10);
        }
    }
    else {
        ofDrawBitmapString("drag and drop a mono/stereo WAV/AIFF file", 10, wh - 10);
    }
    ofDrawBitmapString("click to create/drag ECHOES. d deletes an ECHO", 10, wh - 20);
    
    ofDrawBitmapString("hemanth's ECHO visualizer", ww - 210, wh - 10);
    
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
    // Move the selected circle around
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
            selected_index = num_circles - 1;
            
            return;
        }
    }

    // if we've not maxed out the number of delayCircles, draw one
    if (num_circles < MAX_CIRCLES) {
        circleVector.push_back(new delayCircle(x, y, delay_id_queue.front(), WINDOW_WIDTH, WINDOW_HEIGHT, sampleRate));
        delay_id_queue.pop();
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
    filePath = dragInfo.files[0];
    ofLog(OF_LOG_NOTICE, "String filepath =  %s", filePath.c_str());
    // TODO force that it's .wav
    //if (filePath.endswith)
    
    // Query for number of channels in file (1 or 2)
    stk::FileRead fr;
    fr.open(filePath);
    fileNumChannels = fr.channels();
    fr.close();
    
    // Load file and start playback
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
                output[i+1] = fileInput.lastOut(1) * volume; // writing to the right channel
            
            // Compute each delay
            for (int j = 0; j < MAX_CIRCLES; j++) {
                output[i] = (output[i] * (1.0 - delay_levels[j])) + (delays[j * 2].tick(output[i]) * delay_levels[j]);
                // Use second delay if we have a stereo file, otherwise just copy left to write channel
                if (fileNumChannels == 2) {
                    output[i+1] = (output[i+1] * (1.0 - delay_levels[j])) + (delays[j * 2 +1].tick(output[i+1]) * delay_levels[j]);
                }
                else {
                    output[i+1] = output[i];
                }
            }
            audio.push_back(output[i]);
            audio.erase(audio.begin());
            
        }

    }
    else {
        // Clear the buffer.
        for (int i = 0; i < bufferSize; i++) {
            audio.push_back(0.0);
            audio.erase(audio.begin());
        }
    }
}
