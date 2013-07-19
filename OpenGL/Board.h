#pragma once
#include "Block.h"
class Board
{
	int N;//2^n,n:1~5
	int row;//��λ�кţ�0~N-1
	int col;//��λ�кţ�0~N-1
	/*
		1:��λ���ڲ�����Ҫ4*2����
		2:��λ�ڽ��䣬��Ҫ6*2����
		3:��λ�ڱ߽磬��Ҫ8*2����
	*/
	//��¼�߿�Ĺؼ������飬����������
	float vertX[16];//x����
	float vertY[16];//y����
	int len;//��¼��Ч��ֵ����
	void initVertexes();//����������ɵĲ�����ʼ����ܶ�������
	void copy(float x[],float y[],int l);//��x,y������vertX��vertY��
	void drawframe(float x[],float y[],int l);
	Block**blockes;//ָ���ָ�������ָ��
	int index;//����������
	void clearBlockes();//��տ�
	void addBlock(int type,float x,float y);//���һ����
public:
	Board(void);
	~Board(void);
	void draw(GLenum mode);
	void cover(int n,int i,int j,float x,float y);//���Ǻ���
	void cover();
	static UINT cover (LPVOID pParam) ;
	void processHits(GLint hits, GLuint buffer[]);//�������м�¼
	void pickSquares(int x, int y);//ѡ������
	void allJump();//��Ⱦѡ��Ч��
};

