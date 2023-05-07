/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Matrix44.hpp
*/

#pragma once

#include "../Vectors/Vec3.hpp"

#include <iomanip>

namespace math {
    template<typename T>
    class Matrix44 {
        public:
        static const Matrix44 identity;

        Matrix44() {};

        Matrix44 (T a, T b, T c, T d, T e, T f, T g, T h,
                  T i, T j, T k, T l, T m, T n, T o, T p)
        {
            x[0][0] = a;
            x[0][1] = b;
            x[0][2] = c;
            x[0][3] = d;
            x[1][0] = e;
            x[1][1] = f;
            x[1][2] = g;
            x[1][3] = h;
            x[2][0] = i;
            x[2][1] = j;
            x[2][2] = k;
            x[2][3] = l;
            x[3][0] = m;
            x[3][1] = n;
            x[3][2] = o;
            x[3][3] = p;
        }

        const T* operator [] (uint8_t i) const { return x[i]; }
        T* operator [] (uint8_t i) { return x[i]; }

        static void multiply(const Matrix44<T> &a, const Matrix44<T> &b, Matrix44<T> &c)
        {
            const T *ap = &a.x[0][0];
            const T *bp = &b.x[0][0];
            T *cp = &c.x[0][0];

            T a0, a1, a2, a3;

            a0 = ap[0];
            a1 = ap[1];
            a2 = ap[2];
            a3 = ap[3];

            cp[0]  = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];
            cp[1]  = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];
            cp[2]  = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];
            cp[3]  = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];

            a0 = ap[4];
            a1 = ap[5];
            a2 = ap[6];
            a3 = ap[7];

            cp[4]  = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];
            cp[5]  = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];
            cp[6]  = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];
            cp[7]  = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];

            a0 = ap[8];
            a1 = ap[9];
            a2 = ap[10];
            a3 = ap[11];

            cp[8]  = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];
            cp[9]  = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];
            cp[10] = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];
            cp[11] = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];

            a0 = ap[12];
            a1 = ap[13];
            a2 = ap[14];
            a3 = ap[15];

            cp[12] = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];
            cp[13] = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];
            cp[14] = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];
            cp[15] = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];
        }

        Matrix44<T> operator*(const Matrix44<T>& v) const
        {
            Matrix44<T> tmp;
            multiply(*this, v, tmp);

            return tmp;
        }

        Matrix44 transposed() const
        {
            return Matrix44 (x[0][0],
                             x[1][0],
                             x[2][0],
                             x[3][0],
                             x[0][1],
                             x[1][1],
                             x[2][1],
                             x[3][1],
                             x[0][2],
                             x[1][2],
                             x[2][2],
                             x[3][2],
                             x[0][3],
                             x[1][3],
                             x[2][3],
                             x[3][3]);
        }

        Matrix44& transpose()
        {
            Matrix44 tmp (x[0][0],
                          x[1][0],
                          x[2][0],
                          x[3][0],
                          x[0][1],
                          x[1][1],
                          x[2][1],
                          x[3][1],
                          x[0][2],
                          x[1][2],
                          x[2][2],
                          x[3][2],
                          x[0][3],
                          x[1][3],
                          x[2][3],
                          x[3][3]);
            *this = tmp;
            return *this;
        }

        template<typename S>
        void multVecMatrix(const Vec3<S> &src, Vec3<S> &dest) const
        {
            S a, b, c, w;

            a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0] + x[3][0];
            b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1] + x[3][1];
            c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2] + x[3][2];
            w = src.x * x[0][3] + src.y * x[1][3] + src.z * x[2][3] + x[3][3];
    
            dest.x = a / w;
            dest.y = b / w;
            dest.z = c / w;
        }

        template<typename S>
        void multDirMatrix(const Vec3<S> &src, Vec3<S> &dest) const
        {
            S a, b, c;

            a = src.x * x[0][0] + src.y * x[1][0] + src.z * x[2][0];
            b = src.x * x[0][1] + src.y * x[1][1] + src.z * x[2][1];
            c = src.x * x[0][2] + src.y * x[1][2] + src.z * x[2][2];

            dest.x = a;
            dest.y = b;
            dest.z = c;
        }

        Matrix44 inverse() const
        {
            int i = 0;
            int j = 0;
            int k = 0;
            Matrix44 s;
            Matrix44 t (*this);

            // Forward elimination
            for (i = 0; i < 3 ; i++) {
                int pivot = i;
                T pivotsize = t[i][i];
                if (pivotsize < 0)
                    pivotsize = -pivotsize;
                for (j = i + 1; j < 4; j++) {
                    T tmp = t[j][i];
                    if (tmp < 0)
                        tmp = -tmp;
                    if (tmp > pivotsize) {
                        pivot = j;
                        pivotsize = tmp;
                    }
                }
                if (pivotsize == 0)
                    return Matrix44();
                if (pivot != i) {
                    for (j = 0; j < 4; j++) {
                        T tmp;

                        tmp = t[i][j];
                        t[i][j] = t[pivot][j];
                        t[pivot][j] = tmp;

                        tmp = s[i][j];
                        s[i][j] = s[pivot][j];
                        s[pivot][j] = tmp;
                    }
                }

                for (j = i + 1; j < 4; j++) {
                    T f = t[j][i] / t[i][i];

                    for (k = 0; k < 4; k++) {
                        t[j][k] -= f * t[i][k];
                        s[j][k] -= f * s[i][k];
                    }
                }
            }

            // Backward substitution
            for (i = 3; i >= 0; --i) {
                T f;

                if ((f = t[i][i]) == 0)
                    return Matrix44();

                for (j = 0; j < 4; j++) {
                    t[i][j] /= f;
                    s[i][j] /= f;
                }

                for (j = 0; j < i; j++) {
                    f = t[j][i];

                    for (k = 0; k < 4; k++) {
                        t[j][k] -= f * t[i][k];
                        s[j][k] -= f * s[i][k];
                    }
                }
            }

            return s;
        }

        const Matrix44<T>& invert()
        {
            *this = inverse();
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &s, const Matrix44 &m)
        {
            std::ios_base::fmtflags oldFlags = s.flags();
            int width = 12;
            s.precision(5);
            s.setf(std::ios_base::fixed);

            s << "[" << std::setw(width) << m[0][0] <<
                " " << std::setw(width) << m[0][1] <<
                " " << std::setw(width) << m[0][2] <<
                " " << std::setw(width) << m[0][3] << "\n" <<

                " " << std::setw(width) << m[1][0] <<
                " " << std::setw(width) << m[1][1] <<
                " " << std::setw(width) << m[1][2] <<
                " " << std::setw(width) << m[1][3] << "\n" <<

                " " << std::setw(width) << m[2][0] <<
                " " << std::setw(width) << m[2][1] <<
                " " << std::setw(width) << m[2][2] <<
                " " << std::setw(width) << m[2][3] << "\n" <<

                " " << std::setw(width) << m[3][0] <<
                " " << std::setw(width) << m[3][1] <<
                " " << std::setw(width) << m[3][2] <<
                " " << std::setw(width) << m[3][3] << "]";

            s.flags(oldFlags);
            return s;
        }

        public:
            T x[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    };
}

typedef math::Matrix44<float> Matrix44f;