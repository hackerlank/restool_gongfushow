#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;



typedef signed char         int8;
typedef short               int16;
typedef int                 int32;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;


typedef struct _vec2
{
	float x;
	float y;
} vec2;

typedef struct _vec3
{
	float x;
	float y;
	float z;
} vec3;

typedef struct _vec4
{
	float x;
	float y;
	float z;
	float w;
} vec4;



typedef struct _quat
{
	float x;
	float y;
	float z;
	float w;
} quat;

typedef struct _mat4
{
	float m[4][4];
	
} mat4;


#endif

