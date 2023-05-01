/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MatrixUtils.hpp
*/

#pragma once

#include "Matrix44.hpp"

namespace math {
    template<typename T>
    Matrix44<T> getRotationMatrixX(float angle)
    {
        Matrix44<T> result;
        result.x[1][1] = cos(angle);
        result.x[1][2] = -sin(angle);
        result.x[2][1] = sin(angle);
        result.x[2][2] = cos(angle);
        return result;
    }

    template<typename T>
    Matrix44<T> getRotationMatrixY(float angle)
    {
        Matrix44<T> result;
        result[0][0] = cos(angle);
        result[0][2] = sin(angle);
        result[2][0] = -sin(angle);
        result[2][2] = cos(angle);
        return result;
    }

    template<typename T>
    Matrix44<T> getRotationMatrixZ(float angle)
    {
        Matrix44<T> result;
        result[0][0] = cos(angle);
        result[0][1] = -sin(angle);
        result[1][0] = sin(angle);
        result[1][1] = cos(angle);
        return result;
    }

    template<typename T>
    Matrix44<T> getScaleMatrix(float scaleX, float scaleY, float scaleZ)
    {
        Matrix44<T> result;
        result[0][0] = scaleX;
        result[1][1] = scaleY;
        result[2][2] = scaleZ;
        return result;
    }

    template<typename T>
    Matrix44<T> getShearMatrix(float tilt)
    {
        Matrix44<T> result;
        result[0][1] = 1 / tan(tilt);
        return result;
    }

    template<typename T>
    Matrix44<T> getTranslationMatrix(Vec3f shift)
    {
        Matrix44<T> result;
        result[3][0] = shift.x;
        result[3][1] = shift.y;
        result[3][2] = shift.z;
        return result;
    }
}