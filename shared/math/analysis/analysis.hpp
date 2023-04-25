/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** analysis.hpp
*/

#pragma once

#include <iostream>
#include <cmath>

namespace math {
    /**
     * @brief Solves a quadratic equation, returns true if at least one solution exists
     * Stores the roots of the equation in x0 and x1
     * @param a
     * @param b
     * @param c
     * @param x0
     * @param x1
     * @return true
     * @return false
     */
    bool solveQuadratic(const float &a,
                        const float &b,
                        const float &c,
                        float &x0,
                        float &x1);
}