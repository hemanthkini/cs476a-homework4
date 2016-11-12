//
//  delayCircle.h
//  cs476a-homework4
//
//  Created by Hemanth Kini on 10/31/16.
//
//

#include "ofxStk.h"
#include "ofApp.h"

#ifndef delayCircle_h
#define delayCircle_h

class delayCircle : public ofBaseApp{
private:
    float x;
    float y;
    float radius;
    int index;
    int Enabled;
    int red;
    int green;
    int blue;
    int alpha;
    ofColor* color;
    int brightness;
    float echoMix;
    float echoDelay;
    int Window_Width;
    int Window_Height;
    int Sample_Rate;
    
    
public:
    delayCircle (int x, int y, int index, int Window_Width, int Window_Height, int Sample_Rate) {
        this->x = (float)x;
        this->y = (float)y;
        this->index = index;
        this->Window_Width = Window_Width;
        this->Window_Height = Window_Height;
        radius = 20;
        red = ((int)ofRandom(256.0)) % 256;
        green = (green + (int)ofRandom(74.0) + 17) % 256;
        blue = (red + ((int)ofRandom(174.0)) + 41) % 256;
        this->setRadius();
        setXYandUpdate(x, y);
        alpha = 128;
        brightness = ((int)ofRandom(128.0) + 127);
        color = new ofColor(red, green, blue, alpha);
        color->setBrightness(brightness);
        
    }
    
    void setEchoMix() {
        echoMix = 0.4 * (((float)Window_Height - (float)y) / (float)Window_Height);
    }
    
    void setEchoDelay() {
        echoDelay = 1.0 * (1.0 - (((float)Window_Width - (float)x)/ (float)Window_Width));
    }
    
    float getEchoMix() {
        return echoMix;
    }
    
    float getEchoDelay() {
        return echoDelay;
    }

    int getBrightness() {
        return this->brightness;
    }
    
    void setRadius() {
        this->radius = (15.0 + 30.0 * (1.0 - ((float)y) / ofGetHeight()));
    }
    
    int getX () {
        return this->x;
    }
    
    int getY () {
        return this->y;
    }
    
    int getRadius() {
        return this->radius;
    }
    
    int getRed() {
        return this->red;
    }
    
    int getBlue() {
        return this->blue;
    }
    
    int getGreen() {
        return this->green;
    }
    
    int getAlpha() {
        return this->alpha;
    }
    
    
    void setX (int x) {
        this->x = x;
    }
    
    void setY (int y) {
        this->y = y;
    }
    
    int getIndex() {
        return this->index;
    }
    
    
    // tells us if the provided location is within the circle
    bool within (int x, int y) {
        // TODO make this better
        if (abs(x - this->x) < radius && abs(y - this->y) < radius)
            return true;
        return false;
    }
    
    void setXYandUpdate(int x, int y) {
        this->setX(x);
        this->setY(y);
        this->setRadius();
        setEchoMix();
        setEchoDelay();
    }
    
    // Draw the circle
    void draw() {
        ofSetColor(*color);
        //ofDrawCircle((float)this->getX(), (float)this->getY(), (float)this->getRadius());
    }
    
};
#endif /* delayCircle_h */
