/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MatrixTransformation.hpp
*/

#pragma once

#include "Matrix44.hpp"
#include "../Vectors/Vec3.hpp"

namespace math {
    void translate(Matrix44f &transformationMatrix, Vec3f translation);
    void rotateAroundOriginX(Matrix44f &transformationMatrix, float angle);
    void rotateAroundOriginY(Matrix44f &transformationMatrix, float angle);
    void rotateAroundOriginZ(Matrix44f &transformationMatrix, float angle);
    void rotateX(Matrix44f &transformationMatrix, float angle);
    void rotateY(Matrix44f &transformationMatrix, float angle);
    void rotateZ(Matrix44f &transformationMatrix, float angle);
    void scale(Matrix44f &transformationMatrix, float x = 1, float y = 1, float z = 1);
}