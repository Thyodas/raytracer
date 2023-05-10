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
        transformationMatrix = transformationMatrix * tmp;
    }

    void rotateAroundXAxis(Matrix44f &transformationMatrix, float angle)
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

    void rotateAroundYAxis(Matrix44f &transformationMatrix, float angle)
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
        transformationMatrix = transformationMatrix * tmp1 * tmp2 * tmp3;
    }

    void rotateAroundZAxis(Matrix44f &transformationMatrix, float angle)
    {
        Matrix44f tmp1;
        translate(tmp1, Vec3f(0, 0, -1));

        Matrix44f tmp2;
        tmp2[0][0] = cos(angle);
        tmp2[0][1] = -sin(angle);
        tmp2[1][0] = sin(angle);
        tmp2[1][1] = cos(angle);

        Matrix44f tmp3;
        translate(tmp3, Vec3f(0, 0, 1));
        transformationMatrix = transformationMatrix * tmp1 * tmp2 * tmp3;
    }
}