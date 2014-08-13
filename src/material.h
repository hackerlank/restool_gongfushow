#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "define.h"

enum EMapChannel
{
    EMap_Diffuse=0,
    EMap_Lighting,
    EMap_Specular,
    EMap_Bump,
    EMap_Max
};

#pragma pack(4)
typedef struct _Material
{
    int  id;
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float power;

    bool bTwoSide;
    bool bOpcityMap;
    uint8 opcityVal;

    char map[EMap_Max][256];

    bool bMultiPass;

    //~ zrw added UV 动画.
    float fUSpeed;        
    float fVSpeed;
    float fRot;
    uint32 dwFluidColor;
    bool bFluidFlash;
    bool bAddMode;

    // 场景中自发光有的不希望闪烁.
    bool bGlowStatic;
    
    uint8 reserve[13];        //~ zrw字节对齐会消耗空间.
} Material;
#pragma pack()

#endif
