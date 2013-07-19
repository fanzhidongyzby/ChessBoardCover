#include "StdAfx.h"
#include "Viewer.h"
#include<cmath>

Viewer::Viewer(void)
{
	/*eye[0]=30;
	eye[1]=-200;
	eye[2]=150;*/
	eye[0]=100;
	eye[1]=100;
	eye[2]=100;
}


Viewer::~Viewer(void)
{
}

void Viewer::look()
{
	gluLookAt(eye[0],eye[1],eye[2],0,0,0,0,0,1);
}
void Viewer::near_far(bool flag)
{
	float scale;
	if(flag)//放大
		scale=1.1f;
	else //缩小
		scale=0.9f;//缩放比例
	for(int i=0;i<3;i++)
		eye[i]*=scale;
}
void Viewer::up_down(bool flag)
{
	float pi=3.1415926f;
	float dis=sqrt(eye[0]*eye[0]+eye[1]*eye[1]+eye[2]*eye[2]);
	float disXY=sqrt(eye[0]*eye[0]+eye[1]*eye[1]);
	float angle=acos(disXY/dis)*180/pi;
	if(flag)angle++;
	else angle--;
	if(angle>=89)angle=89;
	if(angle<=1)angle=1;
	float angleXY=acos(eye[0]/disXY);
	eye[0]=dis*cos(angle*pi/180)*cos(angleXY);
	eye[1]=dis*cos(angle*pi/180)*sin(angleXY);
	eye[2]=dis*sin(angle*pi/180);
}