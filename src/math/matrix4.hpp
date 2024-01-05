#pragma once
#include "../defines.hpp"
#include "../physics/vector3.hpp"
#include <cmath>
namespace Math
{
    template <typename Type>
    class Matrix4Dimensional;
    template <typename T>
    class MatrixI32Pair
    {
    public:
        Matrix4Dimensional<T> *matrix{0};
        i32 integer{};
        i32 integer2{};
        MatrixI32Pair(Matrix4Dimensional<T> *_matrix) : matrix{_matrix} {};
        T &operator[](i32 n)
        {
            this->integer2 = n;
            return matrix->asign(integer, integer2);
        }
    };
    template <typename Type>
    class Matrix4Dimensional
    {
    private:
    public:
        Type raw_data_array[16]{0};
        Type elements[4][4]{};
        void func(){};
        Type &asign(i32 i1, i32 i2)
        {
            return this->elements[i1][i2];
        };
        std::ostream &operator<<(std::ostream &out)
        {
            for (i32 i = 0; i < 4; i++)
            {
                out << "\n";
                for (i32 j = 0; j < 4; j++)
                {
                    out << (*this)[i][j];
                };
            };
            return out;
        };
        MatrixI32Pair<Type> operator[](i32 n)
        {
            MatrixI32Pair<Type> mi{this};
            mi.integer = n;
            return mi;
        };
        static void rotateY(Math::Matrix4Dimensional<Type> *matrix, Type theta, Physics::Vector3Dimensional vector);
        Type determinant();
        Matrix4Dimensional(Type data)
        {
            for (i32 i = 0; i < 16; i++)
            {
                raw_data_array[i] = data;
            };
            for (i32 i = 0; i < 4; i++)
                for (i32 j = 0; j < 4; j++)
                {
                    elements[i][j] = data;
                };
        };
    };
};
template <typename Type>
void Math::Matrix4Dimensional<Type>::rotateY(Math::Matrix4Dimensional<Type> *matrix, Type theta, Physics::Vector3Dimensional vector)
{
    (*matrix).elements[0][0] = cos(theta);
    (*matrix).elements[0][1] = 0;
    (*matrix).elements[0][2] = sin(theta);
    (*matrix).elements[1][0] = 0;
    (*matrix).elements[1][1] = 1;
    (*matrix).elements[1][2] = 0;
    (*matrix).elements[2][0] = -sin(theta);
    (*matrix).elements[2][1] = 0;
    (*matrix).elements[2][2] = cos(theta);
    return;
};
template <typename Type>
Type Math::Matrix4Dimensional<Type>::determinant()
{
    return Type{};
};