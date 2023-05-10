/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MatrixTransformation.cpp
*/

#include "MatrixTransformation.hpp"
#include <cmath>

namespace math {
    void translate(Matrix44f &transformationMatrix, Vec3f translation)
    {
        Matrix44f tmp;
        tmp[3][0] = translation.x;
        tmp[3][1] = translation.y;
        tmp[3][2] = translation.z;
        transformationMatrix =  tmp;
    }

    void rotateAroundOriginX(Matrix44f &transformationMatrix, float angle)
    {
        Matrix44f tmp1;
        translate(tmp1, Vec3f(0, 0, -1));

        Matrix44f tmp2;
        tmp2[1][1] = cos(angle);
        tmp2[1][2] = -sin(angle);
        tmp2[2][1] = sin(angle);
        tmp2[2][2] = cos(angle);

        Matrix44f tmp3;
        translate(tmp3, Vec3f(0, 0, 1));
        transformationMatrix = transformationMatrix * tmp1 * tmp2 * tmp3;
    }

    void rotateAroundOriginY(Matrix44f &transformationMatrix, float angle)
    {
        Matrix44f tmp1;
        translate(tmp1, Vec3f(0, 0, -1));

        Matrix44f tmp2;
        tmp2[0][0] = cos(angle);
        tmp2[0][2] = sin(angle);
        tmp2[2][0] = -sin(angle);
        tmp2[2][2] = cos(angle);

        Matrix44f tmp3;
        translate(tmp3, Vec3f(0, 0, 1));
        transformationMatrix =  tmp1 * tmp2 * tmp3;
    }

    void rotateAroundOriginZ(Matrix44f &transformationMatrix, float angle)
    {
        Vec3f pos = Vec3f(transformationMatrix[3][0],transformationMatrix[3][1], transformationMatrix[3][2]);
        Matrix44f tmp1;
        translate(tmp1, Vec3f(0, 0, -1));

        Matrix44f tmp2;
        tmp2[0][0] = cos(angle);
        tmp2[0][1] = -sin(angle);
        tmp2[1][0] = sin(angle);
        tmp2[1][1] = cos(angle);

        Matrix44f tmp3;
        translate(tmp3, pos - Vec3f(0, 0, -1));
        transformationMatrix = transformationMatrix * tmp1 * tmp2 * tmp3;
    }

        void rotateX(Matrix44f &transformationMatrix, float angle)
    {
        Matrix44f tmp;
        tmp[1][1] = cos(angle);
        tmp[1][2] = -sin(angle);
        tmp[2][1] = sin(angle);
        tmp[2][2] = cos(angle);

        transformationMatrix = transformationMatrix * tmp;
    }

    void rotateY(Matrix44f &transformationMatrix, float angle)
    {
        Matrix44f tmp;
        tmp[0][0] = cos(angle);
        tmp[0][2] = sin(angle);
        tmp[2][0] = -sin(angle);
        tmp[2][2] = cos(angle);

        transformationMatrix = transformationMatrix * tmp;
    }

    void rotateZ(Matrix44f &transformationMatrix, float angle)
    {
        Matrix44f tmp;
        tmp[0][0] = cos(angle);
        tmp[0][1] = -sin(angle);
        tmp[1][0] = sin(angle);
        tmp[1][1] = cos(angle);

        Matrix44f tmp3;
        translate(tmp3, Vec3f(0, 0, 1));
        transformationMatrix = transformationMatrix * tmp;
    }

    void scale(Matrix44f &transformationMatrix, float x, float y, float z)
    {
        Matrix44f tmp;
        tmp[0][0] = x;
        tmp[1][1] = y;
        tmp[2][2] = z;

        transformationMatrix = transformationMatrix * tmp;
    }
}