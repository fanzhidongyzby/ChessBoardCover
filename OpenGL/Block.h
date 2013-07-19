#pragma once
class Block
{
	int type;//块类型：0-3
	float x,y,z;//块位置
	static float blockColors[4][4];
	static float selectedColors[4][4];
	bool selected;
	bool direct;//记录跳跃的方向
public:
	Block(int type,float x,float y);
	Block(int type,float x,float y,float z);
	~Block(void);
	void draw();//绘制特定类型的块
	bool drop();//降落
	void select(bool flag);//选择
	bool jump();//选择动作，跳跃
};

