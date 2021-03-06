#ifndef __DRAW_H__
#define __DRAW_H__


#include "define.h"
#include "skel.h"
#include "skin.h"
#include "smm.h"

#include <GLFW/glfw3.h>

class Draw
{
public:
	Draw();
	~Draw();

	void init();

	void update(double dt);
	void render();
	void drawSkel();
	void drawSkin(Skin *skin);
	void drawSmm();

	void drawAxes();
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
	void drawCube(float x, float y, float z, float ll);

public:
	Skel *skel;
	vector<Skin *> skinList;
	Smm *smm;
	int frameId;
	double frameDt;
};



#endif //__SKEL_H__

