/*
 *  ofxOpenNIFilter.cpp
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

#include "ofxOpenNIFilter.h"

int ofxOpenNIFilter::minDistance = 500;
int ofxOpenNIFilter::maxDistance = 3000;

ofxOpenNIFilter::ofxOpenNIFilter(void)
{	
}


ofxOpenNIFilter::~ofxOpenNIFilter(void)
{
}

int ofxOpenNIFilter::getProperUser(ofxOpenNI & device, PLAYERLOCATION mode){
	//得到最适宜人物的骨骼数据
	int trackedUserID = -1;

	if (device.getNumTrackedUsers() > 0){
		int numUsers = device.getNumTrackedUsers();
		//	cout << "getProperUser :: Skeleton number: " + ofToString(numUsers);

		for(int nID = 0; nID < numUsers; nID++)
		{
			if(device.getTrackedUser(nID).isSkeleton())
			{
//				ofLogNotice() << "getProperUser is Skeleton: " + ofToString(device.getTrackedUser(nID).isSkeleton());
				if(trackedUserID == -1)
				{
					trackedUserID = nID;
					//				cout << "Tmp right hand x = " <<trackedUser->right_lower_arm.position[1].X
					//                    << " y = " <<trackedUser->right_lower_arm.position[1].Y
					//                    << " z = " <<trackedUser->right_lower_arm.position[1].Z<<endl;
					//如果站的太偏，太近或太远则初始为NULL
					if(fabs(device.getTrackedUser(trackedUserID).getJoint(JOINT_NECK).getProjectivePosition().x -320.0f) > 100.0f ||
						device.getTrackedUser(nID).getJoint(JOINT_NECK).getWorldPosition().z < minDistance ||
						device.getTrackedUser(nID).getJoint(JOINT_NECK).getWorldPosition().z > maxDistance){
							trackedUserID = -1;
					}
				}	//首先判断是否符合要求
				else if(fabs(device.getTrackedUser(trackedUserID).getJoint(JOINT_NECK).getProjectivePosition().x-320.0f) <= 100.0f &&
					device.getTrackedUser(nID).getJoint(JOINT_NECK).getWorldPosition().z >= minDistance &&
					device.getTrackedUser(nID).getJoint(JOINT_NECK).getWorldPosition().z <= maxDistance)
				{
					//选取更靠近的那个
					if(device.getTrackedUser(nID).getJoint(JOINT_NECK).getWorldPosition().z <
						device.getTrackedUser(trackedUserID).getJoint(JOINT_NECK).getWorldPosition().z)
					{
						trackedUserID = nID;
					}
				}
			}
		}
	}
	return trackedUserID;
}

