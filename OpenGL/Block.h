#pragma once
class Block
{
	int type;//�����ͣ�0-3
	float x,y,z;//��λ��
	static float blockColors[4][4];
	static float selectedColors[4][4];
	bool selected;
	bool direct;//��¼��Ծ�ķ���
public:
	Block(int type,float x,float y);
	Block(int type,float x,float y,float z);
	~Block(void);
	void draw();//�����ض����͵Ŀ�
	bool drop();//����
	void select(bool flag);//ѡ��
	bool jump();//ѡ��������Ծ
};

