#include "StdAfx.h"
#include "Block.h"
#include "OpenGLView.h"
float Block::blockColors[4][4]={{1,0,0,0.5f},{0,1,0,0.5f},{0,0,1,0.5f},{0.5f,0.5f,0.5f,0.5f}};//初始化块的颜色
float Block::selectedColors[4][4]={{1,0,1,0.5f},{0.5f,0.5f,1,0.5f},{0,1,1,0.5f},{0.9f,0.9f,0.9f,0.5f}};//选择块的颜色
Block::Block(int type,float x,float y)
{
	this->type=type;
	this->x=x;
	this->y=y;
	z=0;
	selected=false;
	direct=false;
}
Block::Block(int type,float x,float y,float z)
{
	this->type=type;
	this->x=x;
	this->y=y;
	this->z=z;
	selected=false;
	direct=false;
	COpenGLView::tmpBlock=this;
}
void Block::select(bool flag)
{
	selected=flag;
}
bool Block::drop()
{
	z--;
	if(z<0)z=0;
	return z!=0;
}

bool Block::jump()
{
	if(selected)
	{
		if(direct==false)//向上
		{
			z++;
			if(z>=2*BLOCKWIDTH)//最高点
			{
				direct=true;
				z=2*BLOCKWIDTH;
				z-=2;
			}
		}
		else//向下
		{
			z-=2;
			if(z<=0)//最低点
			{
				z=0;
				direct=false;
				selected=false;//停止选择模式
				return false;
			}
		}
		return true;
	}
	return false;
}

Block::~Block(void)
{
}

void Block::draw()
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef(90.0f*this->type,0,0,1);
	
	//填充
	if(!selected)
		glColor4f(blockColors[type][0],blockColors[type][1],blockColors[type][2],blockColors[type][3]);
	else
		glColor4f(selectedColors[type][0],selectedColors[type][1],selectedColors[type][2],selectedColors[type][3]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLE_FAN);//上面
	glVertex3i(0,0,0);
	glVertex3i(0,-BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(BLOCKWIDTH,0,0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);//下面
	glVertex3i(0,0,BLOCKTHICK);
	glVertex3i(0,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,0,BLOCKTHICK);
	glVertex3i(0,0,BLOCKTHICK);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);//周围
	glVertex3i(0,0,0);
	glVertex3i(0,0,BLOCKTHICK);
	glVertex3i(0,-BLOCKWIDTH,0);
	glVertex3i(0,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,0,0);
	glVertex3i(BLOCKWIDTH,0,BLOCKTHICK);
	glVertex3i(0,0,0);
	glVertex3i(0,0,BLOCKTHICK);
	glEnd();

	//绘制边界
	glColor4f(0,0,0,0.8f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_LINE_LOOP);//上面
	glVertex3i(0,0,0);
	glVertex3i(0,-BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(BLOCKWIDTH,0,0);
	glVertex3i(0,0,0);
	glEnd();
	glBegin(GL_LINE_LOOP);//下面
	glVertex3i(0,0,BLOCKTHICK);
	glVertex3i(0,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,0,BLOCKTHICK);
	glVertex3i(0,0,BLOCKTHICK);
	glEnd();
	glBegin(GL_LINES);//周围
	glVertex3i(0,0,0);
	glVertex3i(0,0,BLOCKTHICK);
	glVertex3i(0,-BLOCKWIDTH,0);
	glVertex3i(0,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,-BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(-BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,0);
	glVertex3i(BLOCKWIDTH,BLOCKWIDTH,BLOCKTHICK);
	glVertex3i(BLOCKWIDTH,0,0);
	glVertex3i(BLOCKWIDTH,0,BLOCKTHICK);	
	glEnd();
	glPopMatrix();
}