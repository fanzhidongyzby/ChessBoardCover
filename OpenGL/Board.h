#pragma once
#include "Block.h"
class Board
{
	int N;//2^n,n:1~5
	int row;//空位行号：0~N-1
	int col;//空位列号：0~N-1
	/*
		1:空位在内部，需要4*2个点
		2:空位在角落，需要6*2个点
		3:空位在边界，需要8*2个点
	*/
	//记录边框的关键点数组，分内外两层
	float vertX[16];//x坐标
	float vertY[16];//y坐标
	int len;//记录有效数值长度
	void initVertexes();//根据随机生成的参数初始化框架顶点数组
	void copy(float x[],float y[],int l);//将x,y拷贝到vertX和vertY中
	void drawframe(float x[],float y[],int l);
	Block**blockes;//指向块指针数组的指针
	int index;//块数组索引
	void clearBlockes();//清空块
	void addBlock(int type,float x,float y);//添加一个块
public:
	Board(void);
	~Board(void);
	void draw(GLenum mode);
	void cover(int n,int i,int j,float x,float y);//覆盖函数
	void cover();
	static UINT cover (LPVOID pParam) ;
	void processHits(GLint hits, GLuint buffer[]);//处理命中记录
	void pickSquares(int x, int y);//选择命中
	void allJump();//渲染选择效果
};

