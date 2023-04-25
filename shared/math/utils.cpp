/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** utils.cpp
*/

#include "utils.hpp"

namespace math {
    float clamp(const float &low, const float &high, const float &value)
    {
        return std::max(low, std::min(high, value));
    }
    float deg2Rad(const float &deg)
    {
        return deg * M_PI / 180;
    }
}