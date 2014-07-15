#pragma once

#include "ofxOpenNI.h"
#include "ofxOpenNITypes.h"
#include "ofMain.h"

class ofxPoseRecognizerEvent: public ofEventArgs
{
public:
	string message;
	bool	bTurnLeft;
	bool	bTurnRight;
	bool	bWalking;
	bool	bStop;

	ofxPoseRecognizerEvent(){
		message = "";
		bTurnLeft = false;
		bTurnRight = false;
		bWalking = false;
		bStop = false;
	}

	static ofEvent <ofxPoseRecognizerEvent> events;
};

class ofxPoseRecognizer
{
public:
	ofxPoseRecognizer();

	~ofxPoseRecognizer(){}

	void listenTurnLeft(){bListenTurnLeft = true;}
	void listenTurnRight(){bListenTurnRight = true;}
	void listenWalking(){bListenWalking = true;}

	void update(ofxOpenNIUser & user);

	bool isTurnLeft(ofxOpenNIUser & user);
	bool isTurnRight(ofxOpenNIUser & user);
	bool isWalking(ofxOpenNIUser & user);



protected:

	bool isLeftArmRaiseUp(ofxOpenNIUser & user);
	bool isRightArmRaiseUp(ofxOpenNIUser & user);

	bool isLeftFootUp(ofxOpenNIUser & user);
	bool isRightFootUp(ofxOpenNIUser & user);  
	
private:
	
	bool   ping_pang;
	float  timeFootUp;
	float  timeThreshold;

	//以下三个变量用于限制指令的发出频率，即每次动作只触发一次指令
	bool bWalking;		
	bool bTurnLeft;
	bool bTurnRight;

	bool bListenTurnLeft;
	bool bListenTurnRight;
	bool bListenWalking;
};


