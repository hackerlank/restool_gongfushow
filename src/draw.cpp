#include "draw.h"
#define PI 3.141592653

Draw::Draw()
{

}


Draw::~Draw()
{
	if(skel)
	{
		delete skel;
		skel = NULL;
	}
}


void Draw::init()
{
	skel = new Skel("res/6BE9F32A.skel");
	skel->showHeadInfo();
	skel->initWorldSpace(0);
}

void Draw::update(double dt)
{

}

void Draw::render()
{
	glColor3f(1.0f, 1.0f, 1.0f);	
	drawAxes();

	for(int i = 0; i < skel->m_info.boneNames.size(); i ++)
	{
		Matrix4f trans = skel->getWorldSpace(i);
		Vec3f pos(0, 0, 0);
		pos = trans * pos/100;

		
		glColor3f(1.0f, 0.0f, 0.0f);	
		drawCube(pos.x, pos.y, pos.z, 0.01);

		BoneData data = skel->m_frames[0].boneDatas[i];
		for(int j = 0; j < data.children.size(); j ++)
		{
			Matrix4f ctrans = skel->getWorldSpace(data.children[j]);
			Vec3f cpos(0, 0, 0);
			cpos = ctrans * cpos/100;
		
			glColor3f(0.0f, 1.0f, 0.0f);	
			drawLine(pos.x, pos.y, pos.z, cpos.x, cpos.y, cpos.z);

		}

	}
}

void Draw::drawAxes()
{
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);//x
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);//y
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);//z
	glEnd();	
}

void Draw::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();	

}

void Draw::drawCube(float x, float y, float z, float ll)
{

	//glBegin(GL_QUADS);					    // 开始绘制立方体
	glBegin(GL_LINE_LOOP);					    // 开始绘制立方体
	glVertex3f(x + ll, y + ll, z - ll);			// 四边形的右上顶点 (顶面)
	glVertex3f(x - ll, y + ll, z - ll);			// 四边形的左上顶点 (顶面)
	glVertex3f(x - ll, y + ll, z + ll);			// 四边形的左下顶点 (顶面)
	glVertex3f(x + ll, y + ll, z + ll);			// 四边形的右下顶点 (顶面)
	glEnd();	
	
	glBegin(GL_LINE_LOOP);					    // 开始绘制立方体
	glVertex3f(x + ll, y - ll, z + ll);			// 四边形的右上顶点(底面)
	glVertex3f(x - ll, y - ll, z + ll);			// 四边形的左上顶点(底面)
	glVertex3f(x - ll, y - ll, z - ll);			// 四边形的左下顶点(底面)
	glVertex3f(x + ll, y - ll, z - ll);			// 四边形的右下顶点(底面)
	glEnd();	

	glBegin(GL_LINE_LOOP);					    // 开始绘制立方体
	glVertex3f(x + ll, y + ll, z + ll);			// 四边形的右上顶点(前面)
	glVertex3f(x - ll, y + ll, z + ll);			// 四边形的左上顶点(前面)
	glVertex3f(x - ll, y - ll, z + ll);			// 四边形的左下顶点(前面)
	glVertex3f(x + ll, y - ll, z + ll);			// 四边形的右下顶点(前面)
	glEnd();	

	glBegin(GL_LINE_LOOP);					    // 开始绘制立方体
	glVertex3f(x + ll, y - ll, z - ll);			// 四边形的右上顶点(后面)
	glVertex3f(x - ll, y - ll, z - ll);			// 四边形的左上顶点(后面)
	glVertex3f(x - ll, y + ll, z - ll);			// 四边形的左下顶点(后面)
	glVertex3f(x + ll, y + ll, z - ll);			// 四边形的右下顶点(后面)
	glEnd();	
	
	glBegin(GL_LINE_LOOP);					    // 开始绘制立方体
	glVertex3f(x - ll, y + ll, z + ll);			// 四边形的右上顶点(左面)
	glVertex3f(x - ll, y + ll, z - ll);			// 四边形的左上顶点(左面)
	glVertex3f(x - ll, y - ll, z - ll);			// 四边形的左下顶点(左面)
	glVertex3f(x - ll, y - ll, z + ll);			// 四边形的右下顶点(左面)
	glEnd();	

	glBegin(GL_LINE_LOOP);					    // 开始绘制立方体
	glVertex3f(x + ll, y + ll, z - ll);			// 四边形的右上顶点(右面)
	glVertex3f(x + ll, y + ll, z + ll);			// 四边形的左上顶点(右面)
	glVertex3f(x + ll, y - ll, z + ll);			// 四边形的左下顶点(右面)
	glVertex3f(x + ll, y - ll, z - ll);			// 四边形的右下顶点(右面)
	glEnd();	
}

//球心坐标为（x，y，z），球的半径为radius，M，N分别表示球体的横纵向被分成多少份
void Draw::drawSphere(float xx, float yy, float zz, float radius, int M, int N)
{
    float step_z = PI/M;
    float step_xy = 2*PI/N;
    float x[4],y[4],z[4];
    
    float angle_z = 0.0;
    float angle_xy = 0.0;

    for(int i=0; i<M; i++)
    {
    	angle_z = i * step_z;
    	
    	for(int j=0; j<N; j++)
    	{
    		angle_xy = j * step_xy;
    		
    		x[0] = radius * sin(angle_z) * cos(angle_xy);
    		y[0] = radius * sin(angle_z) * sin(angle_xy);
    		z[0] = radius * cos(angle_z);
    		
    		x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
    		y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
    		z[1] = radius * cos(angle_z + step_z);
    		
    		x[2] = radius * sin(angle_z + step_z) * cos(angle_xy + step_xy);
    		y[2] = radius * sin(angle_z + step_z) * sin(angle_xy + step_xy);
    		z[2] = radius * cos(angle_z + step_z);
    		
    		x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
    		y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
    		z[3] = radius * cos(angle_z);
    		
			//glBegin(GL_QUADS);
			glBegin(GL_LINE_LOOP);
    		for(int k=0; k<4; k++)
    		    glVertex3f(xx+x[k], zz+z[k], yy+y[k]);
    		glEnd();
    	}
    }
}

