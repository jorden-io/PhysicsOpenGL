#pragma once
#include "vector3.hpp"
namespace Physics
{
    class MovingObject
    {
    public:
        enum class Vectors
        {
            Velocity,
            Acceleration,
            Location,
        };
        f32 mass{10};
        f32 top_speed{1};
        f32 angle{0};
        f32 angular_velocity{0};
        f32 angular_acceleration{0.0};
        Physics::Point3Dimensional location_vector{0};
        Physics::Vector3Dimensional velocity_vector{0};
        Physics::Vector3Dimensional acceleration_vector{0};
        Physics::Vector3Dimensional temp_use_vector{0};
        MovingObject(Point3Dimensional _lvector, Vector3Dimensional _vvector, Vector3Dimensional _avector) : location_vector{_lvector}, velocity_vector{_vvector}, acceleration_vector{_avector} {};
        void accumulate_force(Vector3Dimensional force_vector)
        {
            force_vector.div(this->mass);
            this->acceleration_vector.add(force_vector);
        };
        void copy_vector_data(Vector3Dimensional vector)
        {
            temp_use_vector.x = vector.x;
            temp_use_vector.y = vector.y;
            temp_use_vector.z = vector.z;
        };
        Vector3Dimensional get_vector_copy(Vectors vector_enum)
        {
            switch (vector_enum)
            {
            case Vectors::Velocity:
            {
                Vector3Dimensional copy_velocity_vector{velocity_vector};
                return copy_velocity_vector;
                break;
            }
            case Vectors::Location:
            {
                Vector3Dimensional copy_location_vector{location_vector};
                return copy_location_vector;
                break;
            }
            }
        };
        void update()
        {
            DirectionVector3Dimensional direction_vector = Vector3Dimensional::sub(temp_use_vector, location_vector);
            direction_vector.normalize();
            direction_vector.scale(0.02);
            acceleration_vector = direction_vector;
            velocity_vector.add(acceleration_vector);
            location_vector.add(velocity_vector);

            angular_velocity += angular_acceleration;
            angle += angular_velocity;

            acceleration_vector.scale(0);
        };
        void checkEdges()
        {

            if (location_vector.x > 40)
            {
                location_vector.x = 40;
                velocity_vector.x *= -1;
            }
            else if (location_vector.x < 0)
            {
                velocity_vector.x *= -1;
                location_vector.x = 0;
            }
            if (location_vector.y > 40)
            {
                velocity_vector.y *= -1;
                location_vector.y = 40;
            }

            if (location_vector.z > 40)
            {
                location_vector.z = 40;
                velocity_vector.z *= -1;
            }
            else if (location_vector.z < 0)
            {
                velocity_vector.z *= -1;
                location_vector.z = 0;
            }
            // if ((location_vector.x > 40) || (location_vector.x < 0))
            // {
            //     velocity_vector.x = velocity_vector.x * -1;
            // }
            // if ((location_vector.y > 40) || (location_vector.y < 0))
            // {
            //     velocity_vector.y = velocity_vector.y * -1;
            // }
            // if ((location_vector.z > 40) || (location_vector.z < 0))
            // {
            //     velocity_vector.z = velocity_vector.z * -1;
            // };
        };
    };
};