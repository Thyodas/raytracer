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
        Matrix44f tmp1;
        math::translate(tmp1, Vec3f(0, 0, -1) - orig);

        Matrix44f tmp2;
        tmp2[1][1] = cos(angle);
        tmp2[1][2] = -sin(angle);
        tmp2[2][1] = sin(angle);
        tmp2[2][2] = cos(angle);

        Matrix44f tmp3;
        math::translate(tmp3, -(Vec3f(0, 0, -1) - orig));
        cameraToWorld =  tmp3 * tmp2 * tmp1 ;
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundOriginY(float angle)
    {

        Matrix44f tmp1;
        math::translate(tmp1, Vec3f(0, 0, -1) - orig);

        Matrix44f tmp2;
        tmp2[0][0] = cos(angle);
        tmp2[0][2] = sin(angle);
        tmp2[2][0] = -sin(angle);
        tmp2[2][2] = cos(angle);

        Matrix44f tmp3;
        math::translate(tmp3, -(Vec3f(0, 0, -1) - orig));
        cameraToWorld =  tmp3 * tmp2 * tmp1 ;
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::rotateAroundOriginZ(float angle)
    {
            Matrix44f tmp1;
        math::translate(tmp1, Vec3f(0, 0, -1) - orig);

        Matrix44f tmp2;
        tmp2[0][0] = cos(angle);
        tmp2[0][1] = -sin(angle);
        tmp2[1][0] = sin(angle);
        tmp2[1][1] = cos(angle);

        Matrix44f tmp3;
        math::translate(tmp3, -(Vec3f(0, 0, -1) - orig));
        cameraToWorld =  tmp3 * tmp2 * tmp1 ;
        cameraToWorld.multVecMatrix(orig, orig);
    }

    void Camera::scale(float x, float y, float z)
    {
        math::scale(cameraToWorld, x, y , z);
        cameraToWorld.multVecMatrix(orig, orig);
    }
}