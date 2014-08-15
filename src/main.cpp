#include <math.h>
#include <iostream>
#include "draw.h"
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

using namespace std;

#define MAX_DELTA_T 0.01


GLfloat alpha = 180.f, beta = 0.f, zoom = 200.f, tall = 100.f;
GLboolean paused = GL_FALSE;
GLboolean lockedL = GL_FALSE;
GLboolean lockedR = GL_FALSE;

int cursorX;
int cursorY;

double dt;
Draw draw;


static void error_callback(int error, const char* description)
{
    cout << "Error: " << description << endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_SPACE:
			paused = !paused;
            break;
        case GLFW_KEY_LEFT:
            alpha += 5;
            break;
        case GLFW_KEY_RIGHT:
            alpha -= 5;
            break;
        case GLFW_KEY_UP:
            beta -= 5;
            break;
        case GLFW_KEY_DOWN:
            beta += 5;
            break;
        case GLFW_KEY_PAGE_UP:
            zoom -= 25.0f;
            if (zoom < 0.f)
                zoom = 0.f;
            break;
        case GLFW_KEY_PAGE_DOWN:
            zoom += 25.0f;
            break;
        default:
            break;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
	{

    	if (action == GLFW_PRESS)
    	{
    	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    	    lockedL = GL_TRUE;
    	}
    	else
    	{
    	    lockedL = GL_FALSE;
    	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    	}
	}else
	{
		if (action == GLFW_PRESS)
    	{
    	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    	    lockedR = GL_TRUE;
    	}
    	else
    	{
    	    lockedR = GL_FALSE;
    	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    	}

	}
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if (lockedL)
    {
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;
    }

	if(lockedR)
	{
        tall += (GLfloat) (y - cursorY) / 2.f;
	}

    cursorX = (int) x;
    cursorY = (int) y;
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y *25.0f;
    if (zoom < 0)
        zoom = 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;

    if (height > 0)
        ratio = (float) width / (float) height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, ratio, 1.0, 1024.0);
}


int main(int argc, char* argv[])
{
	glewInit();

    GLFWwindow* window;
    double t, dt_total, t_old;
    int width, height;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
		return 1;

    window = glfwCreateWindow(800, 600, "Wave Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
		return 1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    // Initialize OpenGL
    glDisable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);

	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glClearColor(0.3, 0.3, 0.3, 0.3);

    draw.init();

    // Initialize timer
    t_old = glfwGetTime() - 0.01;

    while (!glfwWindowShouldClose(window))
    {
        t = glfwGetTime();
        dt_total = t - t_old;
        t_old = t;

        // Safety - iterate if dt_total is too large
        while (dt_total > 0.f)
        {
            // Select iteration time step
            dt = dt_total > MAX_DELTA_T ? MAX_DELTA_T : dt_total;
            dt_total -= dt;

			if(!paused)
				draw.update(dt);
        }

        // Draw wave grid to OpenGL display
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        glTranslatef(0.0, -tall, -zoom);
        glRotatef(beta, 1.0, 0.0, 0.0);
        glRotatef(alpha, 0.0, 1.0, 0.0);


        draw.render();
    
		glfwSwapBuffers(window);
        glfwPollEvents();
    }

	return 0;
}

