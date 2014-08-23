#ifndef __SMM_H__
#define __SMM_H__

#include "define.h"
#include "utMath.h"
#include "vertex.h"
#include "texture.h"
#include "material.h"


#pragma pack(4)
typedef struct _SmmHead
{
    uint32      Lable;  // 类型标志
    uint32      Version;// 版本

	uint32		vertType;

	uint32 		vertNum;
    uint32 		vertOffset;  

	uint32 		meshNum;
    uint32 		meshOffset;  
	
	uint32 		mtlNum;
    uint32 		mtlOffset;  
	
	uint32		nodeNum;
	uint32		nodeOffset;

	vec3		min;
	vec3		max;

	uint32		reserve[64];
	
} SmmHead;



typedef struct _SmmTri
{
	uint16 index[3];
} SmmTri;

typedef struct _SmmMesh
{
	uint16 baseIndex;
	int mtlId;
	uint16 vertNum;

	vec3 max;
	vec3 min;

	vector<SmmTri> baseFaces;

	int lodNum;
	vector<SmmTri> lodFaces[4];
} SmmMesh;  

typedef struct _SmmVert
{
	vec3	pos;
	vec3	normal;
	vec2	uv;
} SmmVert;

typedef struct _SmmNode
{
	uint32 tag;
	vec3 trans;
	quat rotat;
	bool mirror;
} SmmNode;



#pragma pack()


class Smm
{
public:
	Smm(const char* filename);
	~Smm();

	void showHeadInfo();
	void showMaterialInfo();

	SmmVert readSmmVert(int i);
	SmmMesh readSmmMesh(int i);
	Material readMaterial(int i);
	SmmNode readSmmNode(int i);

	void initWorldSpace();
	Matrix4f getWorldSpace(int i);
public:
	char 	 m_path[255];
	ifstream m_file;

	SmmHead m_head;

	vector<SmmVert> m_vertList;
	vector<SmmMesh> m_meshList;
	vector<Material> m_mtlList;
	vector<SmmNode> m_nodeList;

	vector<vec3>	  m_vertForRender;
	vector<Matrix4f*> m_worlds;
	vector<Texture *> m_texList;
};



#endif //__SKIN_H__

