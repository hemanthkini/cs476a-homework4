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
    int x;
    int y;
    int radius;
    int index;
    int Enabled;
    int red;
    int green;
    int blue;
    int color;
    float echoMix;
    float echoDelay;
    int Window_Width;
    int Window_Height;
    int Sample_Rate;
    
public:
    delayCircle (int x, int y, int index, int Window_Width, int Window_Height, int Sample_Rate) {
        this->x = x;
        this->y = y;
        this->index = index;
        this->Window_Width = Window_Width;
        this->Window_Height = Window_Height;
        radius = 20;
        red = ((int)ofRandom(256.0)) % 256;
        green = ((int)ofRandom(256.0)) % 256;
        blue = ((int)ofRandom(256.0)) % 256;
        color = ((red & 0xFF) << 16)  | ((green & 0xFF) << 8) | ((blue & 0xFF));
        this->setRadius();
        setXYandUpdate(x, y);
        
    }
    
    void setEchoMix() {
        echoMix = 0.7 * (((float)Window_Height - (float)y) / (float)Window_Height);
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

    void setRadius() {
        this->radius = (int)(20.0 + 40.0 * (1.0 - ((float)y) / ofGetHeight()));
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
    
    int getColor() {
        return this->color;
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
        ofFill();
        ofSetHexColor(this->getColor());
        ofDrawCircle((float)this->getX(), (float)this->getY(), (float)this->getRadius());
    }
    
};
#endif /* delayCircle_h */
