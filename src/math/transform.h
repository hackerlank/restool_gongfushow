#ifndef _MATH_TRANSFORM_H
#define _MATH_TRANSFORM_H

#include "vec3.h"
#include "quat.h"


struct Transform
{
    Vec3 pos;
    Quat rot;
    float scale;

    Transform() : pos(Vec3(0.0f)), rot(Quat(0.0f)), scale(0.0f) { };
    Transform(const Transform &t) : pos(t.pos), rot(t.rot), scale(t.scale) { };
    Transform(const float pos[3], const float rot[4], float scale) : pos(pos), rot(rot), scale(scale) {}

    static Transform Identity() { return Transform(Vec3(0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), 1.0f); }

    Transform operator *(const Transform &t)    { return Transform(*this * t.pos, rot*t.rot, scale*t.scale); } 
    Vec3 operator *(const Vec3 &v) const        { return pos + rot.Transform(v * scale); }    

    static Transform LookAtRH(const Vec3 eye, const Vec3 at, const Vec3 up)
    {
        Vec3 n = (eye - at).Normalized();
        Vec3 u = up.Cross(n).Normalized();
        Vec3 v = n.Cross(u);

        float w = sqrtf(1.0f + u.x + v.y + n.z) * 0.5f;
        float w4 = 1.0f / (4.0f * w);
        
        return Transform(   Vec3(-u.Dot(eye), -v.Dot(eye), -n.Dot(eye)), // Position
                            Quat((v.z-n.y) * w4, (n.x-u.z) * w4, (u.y-v.x) * w4, -w).Normalized(), // Rotation
                            1); // Scale

    }

    void ToMatrix(float m[16]) 
    {
        // http://www.flipcode.com/documents/matrfaq.html#Q54

        float xx      = rot.x * rot.x;
        float xy      = rot.x * rot.y;
        float xz      = rot.x * rot.z;
        float xw      = rot.x * rot.w;

        float yy      = rot.y * rot.y;
        float yz      = rot.y * rot.z;
        float yw      = rot.y * rot.w;

        float zz      = rot.z * rot.z;
        float zw      = rot.z * rot.w;

        m[0]  = (1 - 2 * ( yy + zz )) * scale;
        m[1]  =     2 * ( xy - zw );
        m[2]  =     2 * ( xz + yw );
        m[3]  = pos.x;

        m[4]  =     2 * ( xy + zw );
        m[5]  = (1 - 2 * ( xx + zz )) * scale;
        m[6]  =     2 * ( yz - xw );
        m[7]  = pos.y;

        m[8]  =     2 * ( xz - yw );
        m[9]  =     2 * ( yz + xw );
        m[10] = (1 - 2 * ( xx + yy )) * scale;
        m[11] = pos.z;

        m[12] = m[13] = m[14] = 0;
        m[15] = 1;
    }

    void ToViewProjectionMatrix(float m[16], float fov, float aspect, float znear, float zfar)
    {

        float w = 1.0f/tan(fov/2.0f);
	    float h = w * aspect;
	    float z1 = zfar / (znear-zfar);
	    float z2 = z1 * znear;
        
        ToMatrix(m);
        *((Vec4*)m + 3) = -*((Vec4*)m + 2);
        *((Vec4*)m) *= w;
        *((Vec4*)m + 1) *= h;
        *((Vec4*)m + 2) *= z1;
        m[11] += z2;
    }

};


#endif
