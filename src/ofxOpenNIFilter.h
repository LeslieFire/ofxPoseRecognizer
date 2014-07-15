#pragma once

#include "ofxOpenNI.h"

enum PLAYERLOCATION{
	//player location
	CENTER_FRONT=0
};

class ofxOpenNIFilter
{
public:
	ofxOpenNIFilter(void);
	virtual ~ofxOpenNIFilter(void);

	static int getProperUser(ofxOpenNI & device, PLAYERLOCATION mode = PLAYERLOCATION::CENTER_FRONT);

private:
	static int minDistance;
	static int maxDistance;
};

