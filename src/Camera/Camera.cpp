/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Camera.cpp
*/

#include "Camera.hpp"

namespace raytracer {
    Camera::Camera(Matrix44f &c2w, float s_width, float s_heigth, float c_fov) : cameraToWorld(c2w), width(s_width), height(s_heigth), fov(c_fov)
    {
        c2w.multVecMatrix(Vec3f(0), orig);
        scaleRatio = tan(math::deg2Rad(fov * 0.5));
        imageAspectRatio = width / (float)height;
    }

    void Camera::getDir(float i, float j, Vec3f &dir) const
    {
        float x = (2 * (i + 0.5) / (float)width - 1) * imageAspectRatio * scaleRatio;
        float y = (1 - 2 * (j + 0.5) / (float)height) * scaleRatio;
        cameraToWorld.multDirMatrix(Vec3f(x, y, -1), dir);
        dir = math::normalize(dir);
    }

    void Camera::translate(Vec3f translation)
    {
        math::translate(cameraToWorld, translation);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundX(float angle)
    {
        math::rotateX(cameraToWorld, angle);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundY(float angle)
    {
        math::rotateY(cameraToWorld, angle);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundZ(float angle)
    {
        math::rotateZ(cameraToWorld, angle);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundOriginX(float angle)
    {
        math::rotateAroundOriginX(cameraToWorld, angle);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundOriginY(float angle)
    {
        math::rotateAroundOriginY(cameraToWorld, angle);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundOriginZ(float angle)
    {
        math::rotateAroundOriginZ(cameraToWorld, angle);
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::scale(float x, float y, float z)
    {
        math::scale(cameraToWorld, x, y , z);
        cameraToWorld.multVecMatrix(orig, orig);
    }
}