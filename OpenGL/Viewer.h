#pragma once
class Viewer
{
	float eye[3];//�ӵ㣬����Ĭ��ԭ�㡢
public:
	Viewer(void);
	void look();
	void near_far(bool flag);//Զ��
	void up_down(bool flag);//����
	~Viewer(void);
};

