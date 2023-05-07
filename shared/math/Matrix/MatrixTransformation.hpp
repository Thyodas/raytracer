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
    void rotateAroundXAxis(Matrix44f &transformationMatrix, float angle);
    void rotateAroundYAxis(Matrix44f &transformationMatrix, float angle);
    void rotateAroundZAxis(Matrix44f &transformationMatrix, float angle);
}