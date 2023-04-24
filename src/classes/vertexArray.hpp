#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "../defines.hpp"
namespace gl
{
    template <size_t vao_amount = 1>
    class vertex_array
    {
    private:
    public:
        ui32 id{};
        i32 va_location{};
        vertex_array();
        ~vertex_array();
        void bind();
        void unbind();
        void vertex_attrib_pointer(ui32 index, i32 size, GLenum type, bool nomrmalize, i32 stride, const void *pointer);
        void enable_vertex_attrib_array(ui32 i);
    };

    template <size_t vao_amount>
    vertex_array<vao_amount>::vertex_array()
    {
        glGenVertexArrays(vao_amount, &id);
    };

    template <>
    void vertex_array<>::vertex_attrib_pointer(ui32 index, i32 size, GLenum type, bool normalize, i32 stride, const void *pointer)
    {
        glVertexAttribPointer(index, size, type, normalize, stride, pointer);
    };
    
    template<>
    void vertex_array<>::enable_vertex_attrib_array(ui32 i){
        glEnableVertexAttribArray(i);
    };

    template <>
    void vertex_array<>::bind()
    {
        glBindVertexArray(id);
    };

    template <>
    void vertex_array<>::unbind(){
        glBindVertexArray(0);
    };

    template <>
    vertex_array<>::~vertex_array()
    {
    }
};