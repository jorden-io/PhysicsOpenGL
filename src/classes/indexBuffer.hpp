#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "../defines.hpp"
namespace gl
{
    template <size_t vao_amount = 1>
    class index_buffer
    {
    private:
    public:
        ui32 id{};
        i32 va_location{};
        index_buffer(void *_data, ui32 _size);
        ~index_buffer();
        void bind();
        void unbind();
    };

    template <size_t vao_amount>
    index_buffer<vao_amount>::index_buffer(void *_data, ui32 _size)
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
    };

    template <>
    void index_buffer<>::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    };

    template <>
    void index_buffer<>::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };

    template <>
    index_buffer<>::~index_buffer()
    {
    }
};