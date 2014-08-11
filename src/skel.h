#ifndef __SKEL_H__
#define __SKEL_H__

#include "define.h"
#include "utMath.h"


#pragma pack(4)

typedef struct _BoneData
{
	int32 id;
	int32 parent;
	vector<int32> children;

	vec3 trans;
	quat rotat;
	bool mirror;

} BoneData;

typedef struct _BoneNode
{
	uint32 tag;
	vec3 trans;
	quat rotat;
	bool mirror;
} BoneNode;


typedef struct _SkelHead
{
    uint32 Lable;        // 类型标志
    uint8  Version;      // 版本

    uint32 infoOffset;   // 骨骼信息起始地址

    uint32 frameNum;     // 动画帧数量
    uint32 frameOffset;  // 动画帧起始地址
} SkelHead;

#pragma pack()

typedef struct _SkelInfo
{
	vector<string> boneNames;
	vector<uint32> boneRoots;
} SkelInfo;  


typedef struct _SkelFrame
{
	char name[5];
	float time;

	vector<BoneData> boneDatas;
	vector<BoneNode> boneNodes;
} SkelFrame;



class Skel
{
public:
	Skel(const char* filename);
	~Skel();

	void showHeadInfo();

	void initWorldSpace(int frameId);
	void initWorldSpace(int frameId, int boneId);
	Matrix4f getWorldSpace(int boneId);
public:
	char 	 m_path[255];
	ifstream m_file;

	SkelHead m_head;
	SkelInfo m_info;

	vector<SkelFrame> m_frames;

	vector<Matrix4f*> m_worlds;
};



#endif //__SKEL_H__

