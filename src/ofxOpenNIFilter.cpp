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

