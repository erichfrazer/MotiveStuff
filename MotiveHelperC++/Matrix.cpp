#include "pch.h"
#include "Matrix.h"
#include <cmath>

//
// Point4
//

Point4::Point4(float x, float y, float z, float w)
{
    mData[0] = x;
    mData[1] = y;
    mData[2] = z;
    mData[3] = w;
}

//
// TransformMatrix
//

TransformMatrix::TransformMatrix()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i == j)
            {
                mData[i][j] = 1.0f;
            }
            else
            {
                mData[i][j] = 0.0f;
            }
        }
    }
}

TransformMatrix::TransformMatrix(float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44)
{
    mData[0][0] = m11;
    mData[0][1] = m12;
    mData[0][2] = m13;
    mData[0][3] = m14;
    mData[1][0] = m21;
    mData[1][1] = m22;
    mData[1][2] = m23;
    mData[1][3] = m24;
    mData[2][0] = m31;
    mData[2][1] = m32;
    mData[2][2] = m33;
    mData[2][3] = m34;
    mData[3][0] = m41;
    mData[3][1] = m42;
    mData[3][2] = m43;
    mData[3][3] = m44;
}

void TransformMatrix::SetTranslation(float x, float y, float z)
{
    mData[0][3] = x;
    mData[1][3] = y;
    mData[2][3] = z;
}

void TransformMatrix::Invert()
{
    // Exploit the fact that we are dealing with a rotation matrix + translation component.
    // http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform

    float   tmp;
    float   vals[3];

    // Transpose left-upper 3x3 (rotation) sub-matrix
    tmp = mData[0][1]; mData[0][1] = mData[1][0]; mData[1][0] = tmp;
    tmp = mData[0][2]; mData[0][2] = mData[2][0]; mData[2][0] = tmp;
    tmp = mData[1][2]; mData[1][2] = mData[2][1]; mData[2][1] = tmp;

    // Multiply translation component (last column) by negative inverse of upper-left 3x3.
    for (int i = 0; i < 3; ++i)
    {
        vals[i] = 0.0f;
        for (int j = 0; j < 3; ++j)
        {
            vals[i] += -mData[i][j] * mData[j][3];
        }
    }
    for (int i = 0; i < 3; ++i)
    {
        mData[i][3] = vals[i];
    }
}

TransformMatrix TransformMatrix::RotateX(float rads)
{
    return TransformMatrix(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, (float)cos(rads), (float)-sin(rads), 0.0f,
        0.0f, (float)sin(rads), (float)cos(rads), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

TransformMatrix TransformMatrix::RotateY(float rads)
{
    return TransformMatrix((float)cos(rads), 0.0f, (float)sin(rads), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        (float)-sin(rads), 0.0f, (float)cos(rads), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

TransformMatrix TransformMatrix::RotateZ(float rads)
{
    return TransformMatrix((float)cos(rads), (float)-sin(rads), 0.0f, 0.0f,
        (float)sin(rads), (float)cos(rads), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

TransformMatrix TransformMatrix::operator*(const TransformMatrix& rhs)
{
    TransformMatrix result;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            float rowCol = 0.0;
            for (int k = 0; k < 4; ++k)
            {
                rowCol += mData[i][k] * rhs.mData[k][j];
            }
            result.mData[i][j] = rowCol;
        }
    }
    return result;
}

Point4 TransformMatrix::operator*(const Point4& v)
{
    const float* pnt = v.Data();
    float   result[4];

    for (int i = 0; i < 4; ++i)
    {
        float rowCol = 0.0;
        for (int k = 0; k < 4; ++k)
        {
            rowCol += mData[i][k] * pnt[k];
        }
        result[i] = rowCol;
    }
    return Point4(result[0], result[1], result[2], result[3]);
}
