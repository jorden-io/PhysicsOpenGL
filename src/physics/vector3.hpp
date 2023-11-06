#pragma once
#include "../defines.hpp"
#include <cmath>
// #include <ostream>
namespace Physics
{
    class Vector3Dimensional
    {
    public:
        f32 x{0.0f};
        f32 y{0.0f};
        f32 z{0.0f};

        f32 operator[](i32 n)
        {
            if (n > 3)
            {
                std::cout << "beyond";
                return 0;
            };
            switch (n)
            {
            case 0:
            {
                return this->x;
                break;
            };
            case 1:
            {
                return this->y;
                break;
            };
            case 2:
            {
                return this->z;
                break;
            };
            default:
            {
                return 0;
            }
            };
        };
        friend std::ostream &operator<<(std::ostream &out, const Vector3Dimensional &vector)
        {
            out << vector.x << vector.y << vector.z;
            return out;
        };
        Vector3Dimensional(f32 n) : x{n}, y{n}, z{n} {};
        Vector3Dimensional(f32 _x, f32 _y, f32 _z) : x{_x}, y{_y}, z{_z} {};
        void add(Vector3Dimensional vec)
        {
            this->x = this->x + vec.x;
            this->y = this->y + vec.y;
            this->z = this->z + vec.z;
        };
        static Vector3Dimensional &sub(Vector3Dimensional vec1, Vector3Dimensional vec2)
        {
            Vector3Dimensional temp_vector{0};
            temp_vector.x = vec1.x - vec2.x;
            temp_vector.y = vec1.y - vec2.y;
            temp_vector.z = vec1.z - vec2.z;
            return temp_vector;
        };
        static f32 dot_product(Physics::Vector3Dimensional v1, Physics::Vector3Dimensional v2)
        {
            return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
        };
        void sub(Vector3Dimensional vec)
        {
            this->x = this->x - vec.x;
            this->y = this->y - vec.y;
            this->z = this->z - vec.z;
        };
        void scale(f32 scalar)
        {
            this->x = this->x * scalar;
            this->y = this->y * scalar;
            this->z = this->z * scalar;
        };
        f32 magnitude()
        {
            return sqrt(x * x + y * y + z * z);
        }
        void div(f32 n)
        {
            this->x = this->x / n;
            this->y = this->y / n;
            this->z = this->z / n;
        }
        void normalize()
        {
            f32 m = this->magnitude();
            if (m != 0)
            {
                div(m);
            }
        };
    };
    typedef Vector3Dimensional RGBVector;
    using Point3Dimensional = Vector3Dimensional;
    using DirectionVector3Dimensional = Vector3Dimensional;
};