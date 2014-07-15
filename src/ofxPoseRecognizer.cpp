/*
 *  ofxPoseRecognizer.cpp
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

#include "ofxPoseRecognizer.h"

ofEvent <ofxPoseRecognizerEvent> ofxPoseRecognizerEvent::events;

ofxPoseRecognizer::ofxPoseRecognizer(){
	ping_pang = false;
	timeFootUp = 0.0f;
	timeThreshold = 3.0f;
	
	bWalking = false;	
	bTurnLeft = false;
	bTurnRight = false;

	bListenTurnLeft = false;
	bListenTurnRight = false;
	bListenWalking = false;
}

void ofxPoseRecognizer::update(ofxOpenNIUser & user){
	if ( user.isTracking() ){

		if ( bListenTurnLeft && isTurnLeft( user )){
			// notify event
			static ofxPoseRecognizerEvent eventTurnLeft;
			eventTurnLeft.bTurnLeft = true;
			eventTurnLeft.message = "left";

			ofNotifyEvent(ofxPoseRecognizerEvent::events, eventTurnLeft);
		}
		if ( bListenTurnRight && isTurnRight( user )){
			// notify event
			static ofxPoseRecognizerEvent eventTurnRight;
			eventTurnRight.bTurnRight = true;
			eventTurnRight.message = "right";

			ofNotifyEvent(ofxPoseRecognizerEvent::events, eventTurnRight);
		}
		if ( bListenWalking ){
			if (isWalking( user )){
				//没有在走，所以触发指令；如果已经在行走了，就没必要发出多余的指令
				if (!bWalking){
					bWalking = true;
					// notify event
					static ofxPoseRecognizerEvent eventWalking;
					eventWalking.bWalking = true;
					eventWalking.message = "run";

					ofNotifyEvent(ofxPoseRecognizerEvent::events, eventWalking);
				}
				
			}else{
				if (bWalking){
					bWalking = false;
					// notify event
					static ofxPoseRecognizerEvent eventWalking;
					eventWalking.bStop = true;
					eventWalking.message = "stop";

					ofNotifyEvent(ofxPoseRecognizerEvent::events, eventWalking);
				}
			}
		}
	}
}

bool ofxPoseRecognizer::isTurnLeft(ofxOpenNIUser & user){
	if ( user.isSkeleton() ){
		return isLeftArmRaiseUp(user);
	}
	return false;
}

bool ofxPoseRecognizer::isTurnRight(ofxOpenNIUser & user){
	if ( user.isSkeleton()){
		return isRightArmRaiseUp(user);
	}
	return false;
}

bool ofxPoseRecognizer::isWalking(ofxOpenNIUser & user){
	if ( user.isSkeleton() ){
		if (!ping_pang){
			if (isLeftFootUp(user)){
				ping_pang = true;
				timeFootUp = ofGetElapsedTimef();
			}else{
				if (ofGetElapsedTimef() - timeFootUp >= timeThreshold){
					return false;
				}
			}
		}else{
			if (isRightFootUp(user)){
				ping_pang = false;
				if (ofGetElapsedTimef() - timeFootUp < timeThreshold){
					timeFootUp = ofGetElapsedTimef();
					return true;
				}
			} else{
				if (ofGetElapsedTimef() - timeFootUp >= timeThreshold){
					return false;
				}
			}
		}
	}
	return bWalking;
}

bool ofxPoseRecognizer::isLeftArmRaiseUp(ofxOpenNIUser & user){
	ofPoint &torso = user.getJoint(JOINT_TORSO).getProjectivePosition();
	ofPoint &leftHand = user.getJoint(JOINT_LEFT_HAND).getProjectivePosition();
	ofPoint &leftElbow = user.getJoint(JOINT_LEFT_ELBOW).getProjectivePosition();

	// left hand is on the top of torso  and on the left of left elbow
	return ((leftHand.y < torso.y) && (leftHand.x < leftElbow.x))/* ? true : false*/;
}

bool ofxPoseRecognizer::isRightArmRaiseUp(ofxOpenNIUser & user){
	ofPoint &torso = user.getJoint(JOINT_TORSO).getProjectivePosition();
	ofPoint &righttHand = user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition();
	ofPoint &rightElbow = user.getJoint(JOINT_RIGHT_ELBOW).getProjectivePosition();

	// right hand is on the top of torso  and on the right of right elbow
	return ((righttHand.y < torso.y) && (righttHand.x > rightElbow.x))/* ? true : false*/;		
}



bool ofxPoseRecognizer::isLeftFootUp(ofxOpenNIUser & user){
	ofPoint &leftFoot = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
	ofPoint &rightFoot = user.getJoint(JOINT_RIGHT_FOOT).getProjectivePosition();
	
	return leftFoot.y < rightFoot.y;
}
bool ofxPoseRecognizer::isRightFootUp(ofxOpenNIUser & user){
	ofPoint &leftFoot = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
	ofPoint &rightFoot = user.getJoint(JOINT_RIGHT_FOOT).getProjectivePosition();

	return leftFoot.y > rightFoot.y;
}    