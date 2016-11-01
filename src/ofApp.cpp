#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    num_circles = 0;
    ofRandom(1337);
    rolling_ptr = 0;
    for (int i = 0; i < MAX_CIRCLES; i++) {
        delay_id_queue.push(i);
    }
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
        // TODO this is dirty. fix it?
        circleVector[i]->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    circleVector[selected_index]->setXYandUpdate(x,y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofLog(OF_LOG_NOTICE, "mouse was pressed at %d, %d (x, y)\n", x, y);

    int i = 0;
    for (i = 0; i < num_circles; i++) {
        if (circleVector[i]->within(x,y)) {
            circleVector[i]->setXYandUpdate(x, y);
            selected_index = i;
            return;
        }
    }

    // if we've not maxed out the number of delayCircles, draw one
    if (num_circles < MAX_CIRCLES) {
        circleVector.push_back(new delayCircle(x, y, delay_id_queue.front()));
        selected_index = num_circles;
        num_circles = num_circles+1;
        
    }
    // else, pop the head off and update it.
    else 
    {
        delay_id_queue.push(circleVector[0]->getIndex());
        circleVector.erase(circleVector.begin());
        circleVector.push_back(new delayCircle(x,y, delay_id_queue.front()));
        delay_id_queue.pop();
        selected_index = num_circles;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // Implement deletion here.
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
