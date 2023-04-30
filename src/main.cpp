#include <iostream>
#include <cmath>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "filesystem.h"
#include "shader.h"
#include "tes_shader.hpp"
#include "camera.h"
#include "PerlinNoise.hpp"
#include "defines.hpp"
#include "classes/vertexArray.hpp"
#include "classes/vertexBuffer.hpp"
#include "classes/indexBuffer.hpp"
#define SCR_WIDTH 1600
#define SCR_HEIGHT 1600
#define NUM_PATCH_PTS 4

f32 light_x{10.0f};
f32 light_y{180.0f};
f32 light_z{10.0f};

Camera cam(glm::vec3(0.0, 100.0, 3.0f));
i32 magnitude = 4;
i64 cube_m = 5;
f64 per = 0.5;
f32 map[1000][1000];
i32 test = 5;

f32 wave_speed{0.0005f};
bool waves{false};

gl::shader c_shader{};

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height);
void mouse_callback(GLFWwindow *window, f64 xpos, f64 ypos);
void scroll_callback(GLFWwindow *window, f64 xoffset, f64 yoffset);
void processInput(GLFWwindow *window);

glm::vec3 cameraPos{glm::vec3(-1.0f, 5.0f, 10.0f)};
glm::vec3 cameraFront{glm::vec3(0.0f, 0.0f, -1.0f)};
glm::vec3 cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)};

bool firstMouse{true};
f32 yaw = -90.0f;
f32 pitch = 0.0f;
f32 lastX = 800.0f / 2.0;
f32 lastY = 600.0 / 2.0;
f32 fov = 100.0f;
f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

bool to_rotate = false;
bool one = false;
glm::vec3 cubePositions[50000]{};
glm::vec3 terrainPositions[20000]{};
namespace enums
{
    enum class shader_t
    {
        vetex,
        fragment
    };
};
void swap_poly_mode(i32 n)
{
    if (n == 0)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    };
    if (n == 1)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    };
    return;
};
void swap_fshader(enums::shader_t shader_e, const char *fshader_path, const char *vshader_path = "shader.vs")
{
    switch (shader_e)
    {
    case enums::shader_t::fragment:
        c_shader.set_paths(vshader_path, (const char *)fshader_path);
        c_shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        return;
    case enums::shader_t::vetex:
        c_shader.set_paths(vshader_path, (const char *)fshader_path);
        c_shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        return;
    }
    return;
};
f32 r = 50;

template <size_t n = 50000>
void gen_sphere(glm::vec3 cube_pos[(i32)n])
{
    i32 iter = 0;
    for (i32 i = 0; i < cube_m; i++)
        for (i32 j = 0; j < cube_m; j++)
        {
            f32 lon = i;
            f32 lat = j;
            f32 x{r * sin(lon) * cos(lat)};
            f32 y{r * sin(lon) * sin(lat)};
            f32 z{r * cos(lon)};
            iter++;
            cubePositions[iter] = glm::vec3{x, y, z};
        };
    return;
};

f32 flying{0.0f};
f32 noise_map[1000][1000]{};
void change_magnitude(f32 map[1000][1000], i32 magnitude)
{
    // i32 iter{0};
    waves ? flying += wave_speed : flying += 0.0;
    // const siv::PerlinNoise::seed_type seed = 123456u * magnitude + magnitude;
    // const siv::PerlinNoise perlin{seed};
    // for (i32 i = 0; i < cube_m; i++)
    // {
    //     for (i32 j = 0; j < cube_m; j++)
    //     {
    //         // for (i32 k = 0; k < cube_m; k++)
    //         // {
    //         const f64 noise = perlin.octave2D((f64)((i * 0.0035f)), (f64)((j * 0.0035f)), (i32)magnitude, per);
    //         // const f64 noise = perlin.octave3D((f64)((i * 0.0035f)), (f64)((j * 0.0035f)), (f64)((k * 0.0035f)), (i32)magnitude, per);
    //         noise_map[i][j] = (f32)(noise + test);
    //         //};
    //     }
    // }
    // for (i32 i = 0; i < cube_m; i++)
    // {
    //     for (i32 j = 0; j < cube_m; j++)
    //     {
    //         // for (i32 k = 0; k < cube_m; k++)
    //         // {
    //         // const f64 noise = perlin.octave3D((f64)((i * 0.0035f + noise_map[i][j][k] + flying)), (f64)((j * 0.0035f + noise_map[i][j][k] + flying)), (f64)((k * 0.0035f + noise_map[i][j][k] + flying)), (i32)magnitude, per);
    //         // const f64 noise = perlin.octave2D((f64)((i * 0.0035f + noise_map[i][j][k] + flying)), (f64)((j * 0.0035f + noise_map[i][j][k] + flying)), (i32)magnitude, per);
    //         const f64 noise = perlin.octave2D((f64)((i * 0.0035f - noise_map[i][j] + flying)), (f64)((j * 0.0035f + noise_map[i][j] - flying)), (i32)magnitude, per);
    //         // const f64 noise = perlin.octave3D((f64)((i * 0.0035f + flying)), (f64)((j * 0.0035f + flying)), (f64)((k * 0.0035f + flying)), (i32)magnitude, per);
    //         map[i][j] = (f32)(noise * test);
    //         // cubePositions[iter] = glm::vec3{(f32)map[i][j][k] + i, (f32)map[i][j][k] + j, (f32)map[i][j][k] + k};
    //         // cubePositions[iter] = glm::vec3{(f32)i, (f32)map[i][j][k] + j, (f32)k};
    //         cubePositions[iter] = glm::vec3{(f32)i, (f32)map[i][j] + 15, (f32)(j)};
    //         iter++;
    //         //}
    //     }
    // }
#define h_magnitude 25
    return;
};
bool show_land{true};
bool show_water{false};
i32 height = 250;
i32 width = 250;
static i8 last_magnitude{(i8)magnitude};
inline void createSphere(float radius, int numSlices, int numStacks, const siv::PerlinNoise::seed_type seed, i32 mag, f32 per)
{
    const siv::PerlinNoise perlin{seed};
    std::vector<GLfloat> bertices;
    std::vector<GLuint> bindices;
    const i32 numStrips = (numStacks - 1) / 1;
    const i32 numTrisPerStrip = (numSlices / 1) * 2 - 2;
    for (int i = 0; i < numStacks - 1; i++)
    {
        float v = i / (float)numStacks;
        float phi = v * glm::pi<float>();
        for (int j = 0; j < numSlices - 1; j++)
        {
            const f64 noise = perlin.octave2D((f64)(i * 0.00035), (f64)(j * 0.00035), mag, per);
            float u = j / (float)numSlices;
            float theta = u * glm::two_pi<float>();
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);
            bertices.push_back(radius * x * noise * 10);
            bertices.push_back(radius * y * noise * 10);
            bertices.push_back(radius * z * noise * 10);
            bertices.push_back(x);
            bertices.push_back(y);
            bertices.push_back(z);
            bertices.push_back(u);
            bertices.push_back(v);
        }
    };
    for (int i = 0; i <= numStacks; i++)
    {
        for (int j = 0; j <= numSlices; j++)
        {
            int a = i * (numSlices + 1) + j;
            int b = a + numSlices + 1;
            bindices.push_back(a);
            bindices.push_back(b);
            bindices.push_back(a + 1);
            bindices.push_back(b);
            bindices.push_back(b + 1);
            bindices.push_back(a + 1);
        }
    }
    GLuint vbo, vao, ibo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bertices.size() * sizeof(GLfloat), &bertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bindices.size() * sizeof(GLuint), &bindices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32 * sizeof(GLfloat), (void *)0);
    glBindVertexArray(vao);
    for (ui32 strip = 0; strip < numStrips; strip++)
    {
        glDrawElements(
            GL_TRIANGLE_STRIP,
            numTrisPerStrip + 2,
            GL_UNSIGNED_INT,
            (void *)(sizeof(ui32) * (numTrisPerStrip + 2) * strip));
    };
    glBindVertexArray(0);
};
i32 main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "null", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    GLint maxTessLevel;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);
    // c_shader.set_paths("../shaders/shader.vs", "../shaders/shader.fs");

    std::vector<f32> terrain_vertices;
    std::vector<ui32> terrain_indices;

    const i8 rez{1};

    const siv::PerlinNoise::seed_type tseed{1234567u};
    const siv::PerlinNoise tperlin{tseed};

    f32 tmap[height][width];
    for (i32 i = 0; i < height; i++)
    {
        for (i32 j = 0; j < width; j++)
        {
            const f64 noise = tperlin.octave2D((f64)((i * 0.00035f)), (f64)((j * 0.00035f)), (i32)(20), 0.70);
            tmap[i][j] = (f32)(noise * 300);
        };
    };
    for (i32 i = 0; i < height; i++)
    {
        for (i32 j = 0; j < width; j++)
        {
            terrain_vertices.push_back(-height / 2.0f + height * i / (f32)height); // vx
            terrain_vertices.push_back(tmap[i][j] - 350);                       // vy
            terrain_vertices.push_back(-width / 2.0f + width * j / (f32)width); // vz
        }
    }
    for (ui32 i = 0; i < height - 1; i += rez)
    {
        for (ui32 j = 0; j < width; j += rez)
        {
            for (ui32 k = 0; k < 2; k++)
            {
                terrain_indices.push_back(j + width * (i + k * rez));
            };
        };
    };
    f32 colors[]{
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};
#define SHOWV 0
    f32 vertices[]{
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
#ifdef SHOWV
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
#endif
    };
    gl::vertex_buffer<1> cubeVBO{vertices, sizeof(vertices)};
    gl::vertex_array<1> cubeVAO{};
    cubeVAO.bind();

    cubeVAO.vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)0);
    cubeVAO.enable_vertex_attrib_array(0);

    cubeVAO.vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)(3 * sizeof(f32)));
    cubeVAO.enable_vertex_attrib_array(1);

    cubeVAO.vertex_attrib_pointer(2, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(f32), (void *)(3 * sizeof(f32)));
    cubeVAO.enable_vertex_attrib_array(2);

    cubeVAO.unbind();
    cubeVBO.unbind();

    gl::vertex_buffer<1> lightVBO{colors, sizeof(colors)};

    gl::vertex_array<1> lightVAO{};
    lightVAO.bind();
    cubeVBO.bind();
    lightVAO.vertex_attrib_pointer(0, 3, GL_FLOAT, false, 6 * sizeof(f32), (void *)0);
    lightVAO.enable_vertex_attrib_array(0);

    gl::vertex_array<1> voxel_terrainVAO{};
    voxel_terrainVAO.bind();
    cubeVBO.bind();
    voxel_terrainVAO.vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)0);
    voxel_terrainVAO.enable_vertex_attrib_array(0);

    voxel_terrainVAO.vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)(3 * sizeof(f32)));
    voxel_terrainVAO.enable_vertex_attrib_array(1);

    gl::vertex_array smooth_terrainVAO{};
    smooth_terrainVAO.bind();

    gl::vertex_buffer smooth_terrainVBO{&terrain_vertices[0], (ui32)(terrain_vertices.size() * sizeof(f32))};

    smooth_terrainVAO.vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
    smooth_terrainVAO.enable_vertex_attrib_array(0);

    gl::index_buffer<1> smooth_terrainIBO{&terrain_indices[0], (ui32)(terrain_indices.size() * sizeof(i32))};
    smooth_terrainVAO.unbind();

    const gl::shader light_shader{"../shaders/light_shader.vs", "../shaders/light_shader.fs"};
    const gl::shader tshader{"../shaders/tshader.vs", "../shaders/tshader.fs"};
    const gl::shader water_shader{"../shaders/water_shader.vs", "../shaders/water_shader.fs"};
    const gl::shader terrain_shader{"../shaders/terrain_shader.vs", "../shaders/terrain_shader.fs"};

    Shader tessHeightMapShader("../shaders/tes.vs", "../shaders/tes.fs", nullptr,
                               "../shaders/tes.tcs", "../shaders/tes.tes");
    std::vector<f32> s_vertives{};
    i32 lat = 50;
    i32 lon = 50;
    const i32 radius{10};
    for (i32 i = 0; i <= lat; i++)
    {
        f32 phi{(f32)(i * 2 * M_PI / (lat - 1))};
        for (i32 j = 0; j <= lon; j++)
        {
            f32 theta{(f32)(j * 2 * M_PI / (lon - 1))};
            f32 x{(f32)(radius * sin(theta) * cos(phi))};
            f32 y{(f32)(radius * sin(theta) * sin(phi))};
            f32 z{(f32)(radius * cos(theta))};
            s_vertives.push_back(x);
            s_vertives.push_back(y);
            s_vertives.push_back(z);
        };
    };
    GLuint s_vbo, s_vao;
    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glBindVertexArray(s_vao);

    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, s_vertives.size() * sizeof(f32), &s_vertives[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    static f64 z_rotation{0.0};

    int tes_width{0};
    int tes_height{0};
    int nrChannels{0};
    unsigned char *data = stbi_load("./resources/imgs/map.png", &tes_width, &tes_height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tes_width, tes_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        tessHeightMapShader.setInt("heightMap", 0);
        std::cout << "Loaded heightmap of size " << tes_height << " x " << tes_width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    stbi_image_free(data);

    std::vector<f32> tes_vertices;
    unsigned resolution = 50;
    for (unsigned i = 0; i <= resolution - 1; i++)
    {
        for (unsigned j = 0; j <= resolution - 1; j++)
        {
            tes_vertices.push_back(-tes_width / 2.0f + tes_width * i / (float)resolution);   // v.x
            tes_vertices.push_back(-200);                                                    // v.y
            tes_vertices.push_back(-tes_height / 2.0f + tes_height * j / (float)resolution); // v.z
            tes_vertices.push_back(i / (float)resolution);                                   // u
            tes_vertices.push_back(j / (float)resolution);                                   // v

            tes_vertices.push_back(-tes_width / 2.0f + tes_width * (i + 1) / (float)resolution); // v.x
            tes_vertices.push_back(-200);                                                        // v.y
            tes_vertices.push_back(-tes_height / 2.0f + tes_height * j / (float)resolution);     // v.z
            tes_vertices.push_back((i + 1) / (float)resolution);                                 // u
            tes_vertices.push_back(j / (float)resolution);                                       // v

            tes_vertices.push_back(-tes_width / 2.0f + tes_width * i / (float)resolution);         // v.x
            tes_vertices.push_back(-200);                                                          // v.y
            tes_vertices.push_back(-tes_height / 2.0f + tes_height * (j + 1) / (float)resolution); // v.z
            tes_vertices.push_back(i / (float)resolution);                                         // u
            tes_vertices.push_back((j + 1) / (float)resolution);                                   // v

            tes_vertices.push_back(-tes_width / 2.0f + tes_width * (i + 1) / (float)resolution);   // v.x
            tes_vertices.push_back(-200);                                                          // v.y
            tes_vertices.push_back(-tes_height / 2.0f + tes_height * (j + 1) / (float)resolution); // v.z
            tes_vertices.push_back((i + 1) / (float)resolution);                                   // u
            tes_vertices.push_back((j + 1) / (float)resolution);                                   // v
        }
    }
    unsigned int tesVAO, tesVBO;
    glGenVertexArrays(1, &tesVAO);
    glBindVertexArray(tesVAO);

    glGenBuffers(1, &tesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tes_vertices.size(), &tes_vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ui32 counter{0};
    f64 dx{0.0f};
    while (!glfwWindowShouldClose(window))
    {
        // glPointSize(2);
        //  glfwSwapInterval(0);
        const f32 currentFrame{static_cast<f32>(glfwGetTime())};
        deltaTime = currentFrame - lastFrame;
        counter++;
        if (deltaTime >= 1.0f / 30.0f)
        {
            std::string fps{std::to_string((1.0f / deltaTime) * counter)};
            std::string ms{std::to_string((deltaTime / counter) * 1000)};
            std::string title{"FRAMES::[" + fps + "]" + "    " + "[" + ms + "]::MILLISECONDS"};
            glfwSetWindowTitle(window, title.c_str());
            lastFrame = currentFrame;
            counter = 0;
        };
        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 lmodel{glm::mat4(1.0f)};
        const glm::mat4 projection{glm::perspective(glm::radians(cam.Zoom), (f32)SCR_WIDTH / (f32)SCR_HEIGHT, 0.1f, 100000.0f)};
        const glm::mat4 view{cam.GetViewMatrix()};
        const glm::vec3 lightPos{light_x, light_y, light_z};

        tessHeightMapShader.use();
        tessHeightMapShader.setMat4("projection", projection);
        tessHeightMapShader.setMat4("view", view);
        tessHeightMapShader.setMat4("model", lmodel);
        tessHeightMapShader.setVec3("lightPos", glm::vec3{light_x, light_y, 0.0f});
        tessHeightMapShader.setVec3("viewPos", cam.Position);

        const i16 numStrips = (height - 1) / 1;
        const i16 numTrisPerStrip = (height / 1) * 2 - 2;

        glBindVertexArray(tesVAO);
        glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * resolution * resolution);
        glBindVertexArray(0);

        if (show_land)
        {
            terrain_shader.use();
            terrain_shader.setMat4("projection", projection);
            terrain_shader.setMat4("view", view);
            terrain_shader.setMat4("model", lmodel);
            terrain_shader.setVec3("lightPos", lightPos);
            terrain_shader.setVec3("viewPos", cam.Position);
            smooth_terrainVAO.bind();
            for (ui32 strip = 0; strip < numStrips; strip++)
            {
                glDrawElements(GL_TRIANGLE_STRIP,
                               numTrisPerStrip + 2,
                               GL_UNSIGNED_INT,
                               (void *)(sizeof(ui32) * (numTrisPerStrip + 2) * strip));
            };
        };
        if (show_water)
        {
            std::vector<f32> tvertices;
            std::vector<ui32> indices;
            f32 dhmap[height][width];
            if (!(last_magnitude == (i8)magnitude))
            {
                last_magnitude = (i8)magnitude;
            };
            for (i32 i = 0; i < height; i++)
            {
                for (i32 j = 0; j < width; j++)
                {
                    const f64 noise{tperlin.octave2D((f64)((i * 0.0035f)), (f64)((j * 0.0035f)), (i32)magnitude, per)};
                    dhmap[i][j] = (f64)(noise + test);
                };
            };
            float ttmap[height][width];
            for (i32 i = 0; i < height; i++)
            {
                for (i32 j = 0; j < width; j++)
                {
                    const f64 noise{tperlin.octave2D((f64)((i * dhmap[i][j] * 0.00045f + flying)), (f64)((j * dhmap[i][j] * 0.00045f + flying)), (i32)magnitude, per)};
                    ttmap[i][j] = (f32)(noise * test);
                };
            };
            for (i32 i = 0; i < height; i++)
            {
                for (i32 j = 0; j < width; j++)
                {
                    tvertices.push_back(-height / 2.0f + height * i / (f32)height); // vx
                    tvertices.push_back(ttmap[i][j] - 5);                           // vy
                    tvertices.push_back(-width / 2.0f + width * j / (f32)width);    // vz
                };
            };
            for (ui32 i = 0; i < height - 1; i += rez)
            {
                for (ui32 j = 0; j < width; j += rez)
                {
                    for (ui32 k = 0; k < 2; k++)
                    {
                        indices.push_back(j + width * (i + k * rez));
                    };
                };
            };
            gl::vertex_array waterVAO{};
            waterVAO.bind();

            gl::vertex_buffer<1> waterVBO{&tvertices[0], (ui32)(tvertices.size() * sizeof(f32))};

            waterVAO.vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
            waterVAO.enable_vertex_attrib_array(0);

            waterVAO.vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)(3 * sizeof(f32)));
            waterVAO.enable_vertex_attrib_array(1);

            gl::index_buffer<1> waterIBO{&indices[0], (ui32)(indices.size() * sizeof(ui32))};

            water_shader.use();
            water_shader.setMat4("projection", projection);
            water_shader.setMat4("view", view);
            lmodel = glm::translate(lmodel, glm::vec3{0.0, -85.0, 0.0});
            water_shader.setMat4("model", lmodel);
            water_shader.setVec3("lightPos", lightPos);
            water_shader.setVec3("viewPos", cam.Position);
            waterVAO.bind();
            for (ui32 strip = 0; strip < numStrips; strip++)
            {
                glDrawElements(GL_TRIANGLE_STRIP,
                               numTrisPerStrip + 2,
                               GL_UNSIGNED_INT,
                               (void *)(sizeof(ui32) * (numTrisPerStrip + 2) * strip));
            }
            lmodel = glm::translate(lmodel, glm::vec3{0.0, 0.0, 0.0});
            waterVAO.unbind();
        };
        // RE-------------------

        // tshader.use();
        // tshader.setMat4("projection", projection);
        // tshader.setMat4("view", view);
        // tshader.setVec3("lightPos", lightPos);
        // tshader.setVec3("viewPos", cam.Position);
        // voxel_terrainVAO.bind();
        // i32 it{0};
        // for (i32 i = 0; i < cube_m; i++)
        // {
        //     for (i32 j = 0; j < cube_m; j++)
        //     {
        //         it++;
        //         glm::mat4 tmodel{glm::mat4(1.0f)};
        //         tmodel = glm::translate(tmodel, terrainPositions[it]);
        //         tshader.setMat4("model", tmodel);
        //         tshader.setFloat("height", (f32)tmap[i][j]);
        //         glDrawArrays(GL_TRIANGLES, 0, 36);
        //     };
        // }
        // voxel_terrainVAO.unbind();

        // LIGHT-----------------------
        // z_rotation = (sin(glfwGetTime())) * 1000;
        f64 theta{dx += 0.005};
        i32 sradious{2000};
        light_x = sradious * cos(theta);
        light_y = sradious * sin(theta);
        // light_z = sin(glfwGetTime() * 2) * z_rotation;

        lmodel = glm::translate(lmodel, glm::vec3{light_x, light_y, 0.0f});
        lmodel = glm::rotate<f32>(lmodel, (f32)glfwGetTime() * 2, glm::vec3{10.0, 0.5, 0.02});
        lmodel = glm::scale(lmodel, glm::vec3{30.0f, 30.0f, 30.0f});
        light_shader.use();
        light_shader.setMat4("model", lmodel);
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("view", view);
        glBindVertexArray(s_vao);
        glDrawArrays(GL_POINTS, 0, s_vertives.size() / 3);

        // c_shader.use();
        // c_shader.setMat4("projection", projection);
        // c_shader.setMat4("view", view);
        // c_shader.setVec3("lightPos", lightPos);
        // c_shader.setVec3("viewPos", cam.Position);
        // cubeVAO.bind();
        // for (ui32 i = 0; i <= cube_m * cube_m; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     if (to_rotate)
        //     {
        //         model = glm::rotate(model,
        //                             (f32)glfwGetTime(),
        //                             glm::vec3(-1.0f, 0.5, 0.5f));
        //     };
        //     c_shader.setMat4("model", model);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // };

        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glDeleteVertexArrays(1, &cubeVAO.id);
    glDeleteBuffers(1, &cubeVBO.id);

    glDeleteVertexArrays(1, &tesVAO);
    glDeleteBuffers(1, &tesVBO);

    glfwTerminate();
    return 0;
};
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        show_land = false;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        show_land = true;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        show_water = false;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        show_water = true;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        swap_poly_mode(0);
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        swap_poly_mode(1);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        light_y += 10;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        light_y -= 10;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        light_x += 10;
        light_z += 10;
    };
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        light_x -= 10;
        light_z -= 10;
    };
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        cam.MovementSpeed += 1;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        wave_speed += 0.0001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        wave_speed -= 0.0001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        waves = true;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        swap_fshader(enums::shader_t::fragment, "../shaders/gshader.fs");
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        swap_fshader(enums::shader_t::vetex, "../shaders/shader.fs");
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        test += 1;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        height += 1;
        width += 1;
    };
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        height -= 1;
        width -= 1;
    };
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        per += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        per -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        magnitude += 1;
    change_magnitude(map, magnitude);
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        magnitude -= 1;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        r += 1;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        r -= 1;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        gen_sphere(cubePositions);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        to_rotate = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
};
void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
};
void mouse_callback(GLFWwindow *window, f64 xposIn, f64 yposIn)
{
    f32 xpos = static_cast<f32>(xposIn);
    f32 ypos = static_cast<f32>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    };
    f32 xoffset = xpos - lastX;
    f32 yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    cam.ProcessMouseMovement(xoffset, yoffset);
};
void scroll_callback(GLFWwindow *window, f64 xoffset, f64 yoffset)
{
    cam.ProcessMouseScroll(static_cast<f32>(yoffset));
};