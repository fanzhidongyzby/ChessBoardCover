#include "StdAfx.h"
#include "Board.h"
#include "OpenGLView.h"
#include <cmath>
#include <ctime>

void Board::copy(float x[],float y[],int l)
{
	len=l;
	for(int i=0;i<l;i++)
	{
		vertX[i]=x[i];
		vertY[i]=y[i];
	}
}
Board::Board(void)
{
	srand((unsigned int)time(0));
	N=(int)pow(2.0f,rand()%4+1);//�������
	row=rand()%N;
	col=rand()%N;
	//N=4;row=1;col=1;
	//N=8;row=3;col=7;
	/*CString s;
	s.Format("N=%d:(%d,%d)",N,row,col);
	::AfxMessageBox(s);*/
	initVertexes();//��ʼ����������
	int n=(N*N-1)/3;
	blockes=new Block*[n];//��ָ������
	for(int i=0;i<n;i++)
		blockes[i]=NULL;
	index=0;
	::AfxBeginThread(cover,NULL,0,0,0);
}

void Board::initVertexes()
{
	float te=(float)N*BLOCKWIDTH/2+BLOCKTHICK;//���������
	float ti=te-BLOCKTHICK;//�ڲ�������
	if(row!=0&&row!=N-1&&col!=0&&col!=N-1)//�ڲ�
	{
		len=8;
		float x[8]={-te,-te,te,te,-ti,-ti,ti,ti};
		float y[8]={te,-te,-te,te,ti,-ti,-ti,ti};
		copy(x,y,8);
	}
	else if((row==0||row==N-1)&&(col==0||col==N-1))//����
	{
		len=12;
		int flag=row*2+col;
		if(flag==0)//����
		{
			float x[12]={-te,-te,te,te,-te+BLOCKWIDTH,-te+BLOCKWIDTH,
				-ti,-ti,ti,ti,-ti+BLOCKWIDTH,-ti+BLOCKWIDTH};
			float y[12]={te-BLOCKWIDTH,-te,-te,te,te,te-BLOCKWIDTH,
				ti-BLOCKWIDTH,-ti,-ti,ti,ti,ti-BLOCKWIDTH};
			copy(x,y,12);
		}
		else if(flag==2*(N-1))//����
		{
			float x[12]={-te,-te,-te+BLOCKWIDTH,-te+BLOCKWIDTH,te,te,
				-ti,-ti,-ti+BLOCKWIDTH,-ti+BLOCKWIDTH,ti,ti};
			float y[12]={te,-te+BLOCKWIDTH,-te+BLOCKWIDTH,-te,-te,te,
				ti,-ti+BLOCKWIDTH,-ti+BLOCKWIDTH,-ti,-ti,ti};
			copy(x,y,12);
		}
		else if(flag==N-1)//����
		{
			float x[12]={-te,-te,te,te,te-BLOCKWIDTH,te-BLOCKWIDTH,
				-ti,-ti,ti,ti,ti-BLOCKWIDTH,ti-BLOCKWIDTH};
			float y[12]={te,-te,-te,te-BLOCKWIDTH,te-BLOCKWIDTH,te,
				ti,-ti,-ti,ti-BLOCKWIDTH,ti-BLOCKWIDTH,ti};
			copy(x,y,12);
		}
		else//����
		{
			float x[12]={-te,-te,te-BLOCKWIDTH,te-BLOCKWIDTH,te,te,
				-ti,-ti,ti-BLOCKWIDTH,ti-BLOCKWIDTH,ti,ti};
			float y[12]={te,-te,-te,-te+BLOCKWIDTH,-te+BLOCKWIDTH,te,
				ti,-ti,-ti,-ti+BLOCKWIDTH,-ti+BLOCKWIDTH,ti};
			copy(x,y,12);
		}
	}
	else//�߽�
	{
		if(row==0)//�ϱ�
		{
			float x[16]={
				-te,-te,te,te,-te+(col+1)*BLOCKWIDTH,-te+(col+1)*BLOCKWIDTH,-ti+col*BLOCKWIDTH+BLOCKTHICK,-ti+col*BLOCKWIDTH+BLOCKTHICK,
				-ti,-ti,ti,ti,-ti+(col+1)*BLOCKWIDTH,-ti+(col+1)*BLOCKWIDTH,-ti+col*BLOCKWIDTH,-ti+col*BLOCKWIDTH
			};
			float y[16]={
				te,-te,-te,te,te,te-BLOCKWIDTH,te-BLOCKWIDTH,te,
				ti,-ti,-ti,ti,ti,ti-BLOCKWIDTH,ti-BLOCKWIDTH,ti
			};
			copy(x,y,16);
		}
		else if(row==N-1)//�±�
		{
			float x[16]={
				-te,-te,-ti+col*BLOCKWIDTH+BLOCKTHICK,-ti+col*BLOCKWIDTH+BLOCKTHICK,-te+(col+1)*BLOCKWIDTH,-te+(col+1)*BLOCKWIDTH,te,te,
				-ti,-ti,-ti+col*BLOCKWIDTH,-ti+col*BLOCKWIDTH,-ti+(col+1)*BLOCKWIDTH,-ti+(col+1)*BLOCKWIDTH,ti,ti
			};
			float y[16]={
				te,-te,-te,-te+BLOCKWIDTH,-te+BLOCKWIDTH,-te,-te,te,
				ti,-ti,-ti,-ti+BLOCKWIDTH,-ti+BLOCKWIDTH,-ti,-ti,ti
			};
			copy(x,y,16);
		}
		else if(col==0)//���
		{
			float x[16]={
				-te,-te,-te+BLOCKWIDTH,-te+BLOCKWIDTH,-te,-te,te,te,
				-ti,-ti,-ti+BLOCKWIDTH,-ti+BLOCKWIDTH,-ti,-ti,ti,ti
			};
			float y[16]={
				te,ti-row*BLOCKWIDTH-BLOCKTHICK,ti-row*BLOCKWIDTH-BLOCKTHICK,te-(row+1)*BLOCKWIDTH,te-(row+1)*BLOCKWIDTH,-te,-te,te,
				ti,ti-row*BLOCKWIDTH,ti-row*BLOCKWIDTH,ti-(row+1)*BLOCKWIDTH,ti-(row+1)*BLOCKWIDTH,-ti,-ti,ti
			};
			copy(x,y,16);
		}
		else//�ұ�
		{
			float x[16]={
				-te,-te,te,te,te-BLOCKWIDTH,te-BLOCKWIDTH,te,te,
				-ti,-ti,ti,ti,ti-BLOCKWIDTH,ti-BLOCKWIDTH,ti,ti
			};
			float y[16]={
				te,-te,-te,te-(row+1)*BLOCKWIDTH,te-(row+1)*BLOCKWIDTH,ti-row*BLOCKWIDTH-BLOCKTHICK,ti-row*BLOCKWIDTH-BLOCKTHICK,te,
				ti,-ti,-ti,ti-(row+1)*BLOCKWIDTH,ti-(row+1)*BLOCKWIDTH,ti-row*BLOCKWIDTH,ti-row*BLOCKWIDTH,ti
			};
			copy(x,y,16);
		}
	}
}
void Board::clearBlockes()
{
	//�������
	//int n=(N*N-1)/3;
	for(int i=0;i<index;i++)
	{
		//if(blockes[i]!=NULL)
		delete blockes[i];
		blockes[i]=NULL;
	}
	index=0;
}
Board::~Board(void)
{
	clearBlockes();
	delete [] blockes;
}

void Board::drawframe(float x[],float y[],int l)
{
	l/=2;
	//���
	glColor4f(1,1,0,0.8f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLE_STRIP);//�ϱ�
	for(int i=0;i<l;i++)
	{
		glVertex3f(x[i],y[i],0);
		glVertex3f(x[i+l],y[i+l],0);
	}
	glVertex3f(x[0],y[0],0);
	glVertex3f(x[l],y[l],0);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);//�±�
	for(int i=0;i<l;i++)
	{
		glVertex3f(x[i],y[i],BLOCKTHICK);
		glVertex3f(x[i+l],y[i+l],BLOCKTHICK);
	}
	glVertex3f(x[0],y[0],BLOCKTHICK);
	glVertex3f(x[l],y[l],BLOCKTHICK);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);//���
	for(int i=0;i<l;i++)
	{
		glVertex3f(x[i],y[i],0);
		glVertex3f(x[i],y[i],BLOCKTHICK);
	}
	glVertex3f(x[0],y[0],0);
	glVertex3f(x[0],y[0],BLOCKTHICK);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);//���
	for(int i=0;i<l;i++)
	{
		glVertex3f(x[i+l],y[i+l],0);
		glVertex3f(x[i+l],y[i+l],BLOCKTHICK);
	}
	glVertex3f(x[l],y[l],0);
	glVertex3f(x[l],y[l],BLOCKTHICK);
	glEnd();
	//�߽�
	int next;
	glColor4f(0,0,0,0.5f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_LINES);	
	for(int i=0;i<l;i++)
	{
		next=(i+1)%l;
		//��
		glVertex3f(x[i],y[i],0);
		glVertex3f(x[next],y[next],0);
		glVertex3f(x[i],y[i],BLOCKTHICK);
		glVertex3f(x[next],y[next],BLOCKTHICK);
		glVertex3f(x[i],y[i],0);
		glVertex3f(x[i],y[i],BLOCKTHICK);

		//��
		glVertex3f(x[i+l],y[i+l],0);
		glVertex3f(x[next+l],y[next+l],0);
		glVertex3f(x[i+l],y[i+l],BLOCKTHICK);
		glVertex3f(x[next+l],y[next+l],BLOCKTHICK);
		glVertex3f(x[i+l],y[i+l],0);
		glVertex3f(x[i+l],y[i+l],BLOCKTHICK);
	}
	glEnd();
}

void Board::draw(GLenum mode)
{
	if(len==8)//�ڲ�
	{
		float te=BLOCKWIDTH/2;
		float ti=BLOCKWIDTH/2-BLOCKTHICK;
		float pos0=(N/2-0.5f)*BLOCKWIDTH;
		float x[8]={-te,-te,te,te,-ti,-ti,ti,ti};
		float y[8]={te,-te,-te,te,ti,-ti,-ti,ti};
		glPushMatrix();
		glTranslatef(-pos0+col*BLOCKWIDTH,pos0-row*BLOCKWIDTH,0);
		drawframe(x,y,8);
		glPopMatrix();
	}
	this->drawframe(vertX,vertY,len);//���
	//�ڲ���
	for(int i=0;i<index;i++)
	{
		if (mode == GL_SELECT)
			glLoadName(i+1);
		blockes[i]->draw();
	}
}
int map[4]={2,1,3,0};
void Board::cover(int n,int i,int j,float x,float y)
{
	if(n==2)addBlock(map[i*2+j],x,y);//����ֱ�Ӹ���
	else
	{
		int pos=2*(i>=n/2)+(j>=n/2);
		addBlock(map[pos],x,y);
		if(pos==0)//��λ������
			cover(n/2,i,j,x-n/4*BLOCKWIDTH,y+n/4*BLOCKWIDTH);
		else
			cover(n/2,n/2-1,n/2-1,x-n/4*BLOCKWIDTH,y+n/4*BLOCKWIDTH);
		if(pos==1)//��λ������
			cover(n/2,i,j-n/2,x+n/4*BLOCKWIDTH,y+n/4*BLOCKWIDTH);
		else
			cover(n/2,n/2-1,0,x+n/4*BLOCKWIDTH,y+n/4*BLOCKWIDTH);
		if(pos==2)//��λ������
			cover(n/2,i-n/2,j,x-n/4*BLOCKWIDTH,y-n/4*BLOCKWIDTH);
		else
			cover(n/2,0,n/2-1,x-n/4*BLOCKWIDTH,y-n/4*BLOCKWIDTH);
		if(pos==3)//��λ������
			cover(n/2,i-n/2,j-n/2,x+n/4*BLOCKWIDTH,y-n/4*BLOCKWIDTH);
		else
			cover(n/2,0,0,x+n/4*BLOCKWIDTH,y-n/4*BLOCKWIDTH);
	}
}
void Board::addBlock(int type,float x,float y)
{
	blockes[index++]=new Block(type,x,y,2*BLOCKWIDTH);
	while(COpenGLView::tmpBlock!=NULL)
	{
		Sleep(100);
	}
}

void Board::cover()
{
	clearBlockes();
	cover(N,row,col,0,0);
	COpenGLView::runOver=true;
}

UINT Board::cover(LPVOID param)
{
	COpenGLView::board->cover();
	return 0;
}

void Board::processHits(GLint hits, GLuint buffer[])
{
	if(hits<=0)
		return ;
	GLuint names,disp, *ptr;
	ptr = (GLuint *) buffer;
	for (GLint i = 0; i < hits; i++)
	{
		names = *ptr;
		ptr++;ptr++;ptr++;
		for (GLuint j = 0; j < names; j++)
		{
			disp=*ptr;
			if(disp>0)
			{
				if(blockes[disp-1]!=COpenGLView::tmpBlock)
				{				
					blockes[disp-1]->select(true);
				}
			}
			ptr++;
		}
	}	
}
#define BUFSIZE 512
void Board::pickSquares(int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(BUFSIZE, selectBuf);
	(void) glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	/* create 5x5 pixel picking region near cursor location */
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y),
		0.01, 0.01, viewport);
	
	gluPerspective							// ����͸��ͼ
				( 60,							// ͸�ӽ�����Ϊ 60 ��
				(GLfloat)COpenGLView::width/(GLfloat)COpenGLView::height,	// ���ڵĿ���߱�
				1.0f,								// ��Ұ͸�����:����1.0f
				2000							// ��Ұ͸�����:ʼ��0.1fԶ��1000.0f
				);
	COpenGLView::viewer->look();

	glPushMatrix();
	glRotatef(COpenGLView::angle,0,0,1);
	draw(GL_SELECT);
	glPopMatrix();

	glPopMatrix();
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void Board::allJump()
{
	for(int i=0;i<index;i++)
		blockes[i]->jump();
}