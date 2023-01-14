#pragma once

const float kRadToDeg = 0.0174532925f;

class Point4
{
public:
    Point4(float x, float y, float z, float w);

    float operator[](int idx) const { return mData[idx]; }
    const float* Data() const { return mData; }

private:
    float mData[4];
};

class TransformMatrix
{
public:
    TransformMatrix();

    TransformMatrix(float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44);

    void SetTranslation(float x, float y, float z);
    void Invert();

    TransformMatrix operator*(const TransformMatrix& rhs);
    Point4          operator*(const Point4& v);

    static TransformMatrix RotateX(float rads);
    static TransformMatrix RotateY(float rads);
    static TransformMatrix RotateZ(float rads);

private:
    float mData[4][4];
};
