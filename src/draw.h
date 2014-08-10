#ifndef __DRAW_H__
#define __DRAW_H__

#include <GLFW/glfw3.h>

#include "define.h"
#include "skel.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/quat.h"
#include "math/transform.h"

class Draw
{
public:
	Draw();
	~Draw();

	void init();

	void update(double dt);
	void render();

	void drawAxes();
	void drawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat ll);
    void drawSphere(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat radius, GLuint M, GLuint N);

public:
	Skel *skel;
};



#endif //__SKEL_H__

