#include "draw.h"
#define PI 3.141592653


#define SKEL_RES "res/skel/2/A186C74D.skel"

#define SKIN_RES_MAX 2
const char *SKIN_RES[SKIN_RES_MAX] ={
	"res/avatargirl/wf01.skin",
	//"res/avatargirl/nvzhujue_shenti_up.skin",
	//"res/avatargirl/nvzhujue_shenti_down.skin",
	//"res/avatargirl/mhair/ff01_00.skin",
	//"res/avatargirl/yifu/shangyi_1.skin",
	//"res/avatargirl/yifu/kuzi_1.skin",
	//"res/avatargirl/yifu/xie_1.skin",
	"res/shizhuang/sz_22.skin"
};




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

	for(int i = 0; i < skinList.size(); i ++)
		delete skinList[i];
	skinList.clear();
}


void Draw::init()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	frameId = 0;
	frameDt = 0;

	skel = new Skel(SKEL_RES);
	skel->showHeadInfo();
	skel->initWorldSpace(0);

	for(int i = 0; i < SKIN_RES_MAX; i++)
	{
		Skin* skin = new Skin(SKIN_RES[i]);
		skin->showHeadInfo();
		for(int j = 0; j < skin->m_mtlList.size(); j++)
		{
			char path[256] = "res/";
			strcat(path, skin->m_mtlList[j].map[0] + 7);
			for(int t = 0; t < strlen(path); t++)
			{
				if(path[t] == '\\')
					path[t] = '/';
				else
					path[t] = tolower(path[t]);

			}
			cout << path << endl;

			Bitmap bmp = Bitmap::bitmapFromFile(path);
			//bmp.flipVertically();
			Texture *tex = new Texture(bmp);
			skin->m_texList.push_back(tex);
		}

		skinList.push_back(skin);
	}
}

void Draw::update(double dt)
{
	//cout << dec << frameId << " "<< frameDt << endl;
	frameDt += dt;
	if(frameDt > skel->m_frames[frameId].time)
	{
		frameDt = 0;
		frameId ++;
		if(frameId >= skel->m_frames.size())
			frameId = 0;
		
		skel->initWorldSpace(frameId);
	}

	for(int s = 0; s < skinList.size(); s ++)
	{
		Skin *skin = skinList[s];
	    for(int i = 0; i < skin->m_vertList.size(); i ++)
	    {
			Vec3f vv(0, 0, 0);
	    	for(int j = 0; j < skin->m_vertList[i].bones.size(); j ++)
	    	{
	    		SkinBone bone = skin->m_vertList[i].bones[j];
	    		Matrix4f trans = skel->getWorldSpace(bone.boneId);
				Vec3f offset(bone.offset.x, bone.offset.y, bone.offset.z);
				vv = vv + (trans * offset) * bone.weight;
	    	}
			skin->m_vertList[i].pos.x = vv.x;
	    	skin->m_vertList[i].pos.y = vv.y;
	    	skin->m_vertList[i].pos.z = vv.z;

	    }
	}

}

void Draw::render()
{
	//drawSkel();
	for(int i = 0; i < skinList.size(); i++)
		drawSkin(skinList[i]);
}

void Draw::drawSkel()
{
	glColor3f(1.0f, 1.0f, 1.0f);	
	drawAxes();

	for(int i = 0; i < skel->m_info.boneNames.size(); i ++)
	{
		Matrix4f trans = skel->getWorldSpace(i);
		Vec3f pos(0, 0, 0);
		pos = trans * pos;

		
		glColor3f(1.0f, 1.0f, 1.0f);	
		drawCube(pos.x, pos.y, pos.z, 0.01);

		BoneData data = skel->m_frames[frameId].boneDatas[i];
		for(int j = 0; j < data.children.size(); j ++)
		{
			Matrix4f ctrans = skel->getWorldSpace(data.children[j]);
			Vec3f cpos(0, 0, 0);
			cpos = ctrans * cpos;
		
			glColor3f(0.0f, 1.0f, 0.0f);	
			drawLine(pos.x, pos.y, pos.z, cpos.x, cpos.y, cpos.z);

		}

	}
}

void Draw::drawSkin(Skin *skin)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(SkinVert), &skin->m_vertList[0].pos);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(SkinVert), &skin->m_vertList[0].uv);


	for(int i = skin->m_meshList.size() - 1; i >= 0; i--)
	{
		SkinMesh mesh = skin->m_meshList[i];
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, skin->m_texList[mesh.mtlId]->object());
		glDrawElements(GL_TRIANGLES, mesh.baseFaces.size() * 3, GL_UNSIGNED_SHORT, &mesh.baseFaces[0]);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void Draw::drawAxes()
{
	glBegin(GL_LINES);
	glVertex3f(-1000, 0, 0);
	glVertex3f(1000, 0, 0);//x
	glVertex3f(0, -1000, 0);
	glVertex3f(0, 1000, 0);//y
	glVertex3f(0, 0, -1000);
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














