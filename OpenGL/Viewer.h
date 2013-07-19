#pragma once
class Viewer
{
	float eye[3];//视点，焦点默认原点、
public:
	Viewer(void);
	void look();
	void near_far(bool flag);//远近
	void up_down(bool flag);//上下
	~Viewer(void);
};

