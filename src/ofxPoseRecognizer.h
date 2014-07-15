/*
 *  ofxPoseRecognizer.h
 *
 *  Created by Leslie Yang on 07/15/14.
 *  Copyright 2014 https://github.com/LeslieFire All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************ */ 
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


