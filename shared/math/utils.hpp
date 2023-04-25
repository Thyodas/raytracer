/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** utils.hpp
*/

#pragma once

#include <cmath>
#include <iostream>

namespace math {
    float clamp(const float &low, const float &high, const float &value);
    float deg2Rad(const float &deg);
}