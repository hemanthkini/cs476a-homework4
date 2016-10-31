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
    int x, int y;
    int index;
    int Enabled;
    
public:
    delayCircle (int x, int y, int index) {
        this.x = x;
        this.y = y;
        this.index = index;
    }
    
    
    void getX () {
        return this.x;
    }
    
    void getY () {
        return this.y;
    }
    
    void setX (int x) {
        this.x = x;
    }
    
    void setY (int y) {
        this.y = y;
    }
    
    
};
#endif /* delayCircle_h */
