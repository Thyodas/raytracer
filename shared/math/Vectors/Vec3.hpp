/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Vec3f.hpp
*/

#pragma once

#include <iostream>
#include <cmath>

namespace math {
    template <typename T>
    class Vec3 {
        public:
            ~Vec3() = default;
            Vec3() : x(0), y(0) {};
            Vec3(T xx) : x(xx), y(xx), z(xx) {};
            Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {};

        Vec3 operator*(const float &r) const {return Vec3(x * r, y * r, z * r);}
        Vec3 operator*(const Vec3 &v) const {return Vec3(x * v.x, y * v.y, z * v.z);}
        Vec3 operator-(const Vec3 &v) const {return Vec3(x - v.x, y - v.y, z - v.z);}
        Vec3 operator+(const Vec3 &v) const {return Vec3(x + v.x, y + v.y, z + v.z);}
        Vec3 operator-() const { return Vec3(-x, -y, -z);}
        Vec3& operator+=(const Vec3 &v) { x += v.x, y += v.y, z += v.z; return *this;}
        friend Vec3 operator*(const float &r, const Vec3 &v)
        {return Vec3(v.x * r, v.y * r, v.z * r);}
        friend std::ostream & operator << (std::ostream &os, const Vec3 &v)
        {return os << v.x << ", " << v.y << ", " << v.z;}

            T x, y, z;
    };
}

typedef math::Vec3<float> Vec3f;
typedef math::Vec3<int> Vec3i;
typedef math::Vec3<double> Vec3d;