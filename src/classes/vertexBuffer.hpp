#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "../defines.hpp"
namespace gl
{
    template <size_t vbo_amount = 1>
    class vertex_buffer
    {
    private:
    public:
        ui32 id{};
        vertex_buffer(void *_data, ui32 _size);
        void bind();
        void unbind();
        ~vertex_buffer();
    };

    template <size_t vbo_amount>
    vertex_buffer<vbo_amount>::vertex_buffer(void *_data, ui32 _size)
    {
        glGenBuffers(vbo_amount, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
    };

    template <>
    void vertex_buffer<>::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    };

    template <>
    void vertex_buffer<>::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    };

    template <>
    vertex_buffer<>::~vertex_buffer()
    {
    }
};