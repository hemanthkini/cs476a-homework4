//
//  delayCircle.h
//  cs476a-homework4
//
//  Created by Hemanth Kini on 10/31/16.
//
//

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
    
public:
    delayCircle (int x, int y, int index) {
        this->x = x;
        this->y = y;
        this->index = index;
        radius = 10;
        red = ((int)ofRandom(40.0)) % 40;
        green = ((int)ofRandom(40.0)) % 40;
        blue = ((int)ofRandom(40.0)) % 40;
        // TODO make everything not blue
        color = ((red & 0xFF) << 4)  | ((green & 0xFF) << 2) | ((blue & 0xFF));
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
    }
    
    // Draw the circle
    void draw() {
        ofFill();
        ofSetHexColor(this->getColor());
        ofDrawCircle((float)this->getX(), (float)this->getY(), (float)this->getRadius());
    }
    
};
#endif /* delayCircle_h */
