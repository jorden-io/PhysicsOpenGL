
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
#include "physics/vector3.hpp"
#include "physics/movingObject.hpp"
#include "math/matrix4.hpp"
#define SCR_WIDTH 1600
#define SCR_HEIGHT 1600
unsigned int sphereVAO = 0;
unsigned int indexCount;
double xpos, ypos;
bool shot = false;
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    // glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}
void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

f32 light_x{10.0f};
f32 light_y{10.0f};
f32 light_z{10.0f};

Camera cam(glm::vec3(0.0, 20.0, 3.0f));
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
f32 flying{0.0f};
f32 theta{1};
class Bullet
{
public:
    glm::vec3 bullet_positionONCE{};
    glm::vec3 bullet_velocityONCE{};
    glm::vec3 bullet_position{};
    glm::vec3 bullet_velocity{};
    f32 btheta{1};
    bool was_shot = false;
    Bullet(){};
};
Bullet current_bullet{};
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
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_E);

    // glfwSetCursorPosCallback(window, cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);
    // c_shader.set_paths("../shaders/shader.vs", "../shaders/shader.fs");

    const i8 rez{1};
    float vertices[] = {
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

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glEnableVertexAttribArray(0);

    const siv::PerlinNoise::seed_type tseed{1234567u};
    const siv::PerlinNoise tperlin{tseed};
    const gl::shader light_shader{"../shaders/light_shader.vs", "../shaders/light_shader.fs"};
    const gl::shader tshader{"../shaders/tshader.vs", "../shaders/tshader.fs"};
    const gl::shader water_shader{"../shaders/water_shader.vs", "../shaders/water_shader.fs"};
    const gl::shader terrain_shader{"../shaders/terrain_shader.vs", "../shaders/terrain_shader.fs"};

    Shader tessHeightMapShader("../shaders/tes.vs", "../shaders/tes.fs", nullptr,
                               "../shaders/tes.tcs", "../shaders/tes.tes");
    ui32 counter{0};
    renderSphere();
    Physics::MovingObject object{{0, 0, 0}, {0.0, 0.0, 0.0}, {0.01, 0.01, 0.01}};
    Math::Matrix4Dimensional<f64> matrix(1.0f);

    std::vector<f32> terrain_vertices;
    std::vector<ui32> terrain_indices;
    i32 height = 200;
    i32 width = 200;
    for (i32 i = 0; i < height; i++)
    {
        for (i32 j = 0; j < width; j++)
        {
            terrain_vertices.push_back(-height / 2.0f + height * i / (f32)height);        // vx
            terrain_vertices.push_back((8 * cos(sqrt((i * i) * 0.02 + (j * j) * 0.02)))); // vy
            terrain_vertices.push_back(-width / 2.0f + width * j / (f32)width);           // vz
            terrain_vertices.push_back(-height / 2.0f + height * i / (f32)height);        // vx
            terrain_vertices.push_back((8 * cos(sqrt((i * i) * 0.02 + (j * j) * 0.02)))); // vy
            terrain_vertices.push_back(-width / 2.0f + width * j / (f32)width);           // vz
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
    gl::vertex_array smooth_terrainVAO{};
    smooth_terrainVAO.bind();

    gl::vertex_buffer smooth_terrainVBO{&terrain_vertices[0], (ui32)(terrain_vertices.size() * sizeof(f32))};

    smooth_terrainVAO.vertex_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)0);
    smooth_terrainVAO.enable_vertex_attrib_array(0);

    smooth_terrainVAO.vertex_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)0);
    smooth_terrainVAO.enable_vertex_attrib_array(1);

    gl::index_buffer<1> smooth_terrainIBO{&terrain_indices[0], (ui32)(terrain_indices.size() * sizeof(i32))};
    smooth_terrainVAO.unbind();
    theta = 1;
    glm::vec3 ball_posCLONE = cam.Position;
    glm::vec3 ballVelocityCLONE = cam.Front;
    Bullet bullets[11]{};
    i32 BULLETAMMOUNT = 10;
    for (i32 i = 0; i < BULLETAMMOUNT; i++)
    {
        bullets[i] = Bullet();
    };
    while (!glfwWindowShouldClose(window))
    {
        // glfwGetCursorPos(window, &xpos, &ypos);
        const f32 currentFrame{static_cast<f32>(glfwGetTime())};
        deltaTime = currentFrame - lastFrame;
        counter++;
        if (deltaTime >= 1.0f / 30.0f)
        {
            std::string fps{std::to_string((1.0f / deltaTime) * counter)};
            std::string ms{std::to_string((deltaTime / counter) * 1000)};
            // std::string title{"FRAMES::[" + fps + "]" + "    " + "[" + ms + "]::MILLISECONDS"};
            std::string title = !shot ? ("x-" + std::to_string(cam.Position.x) + " y-" + std::to_string(cam.Position.y) + " z-" + std::to_string(cam.Position.z) + "[" + std::to_string(cam.Front.x * theta) + " " + std::to_string(cam.Front.y) + " " + std::to_string(cam.Front.z)) : "shot" + std::to_string(ball_posCLONE.y);
            glfwSetWindowTitle(window, title.c_str());
            lastFrame = currentFrame;
            counter = 0;
        };
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 lmodel{glm::mat4(1.0f)};
        glm::mat4 model{glm::mat4(1.0f)};
        glm::mat4 wmodel{glm::mat4(1.0f)};
        const glm::mat4 projection{glm::perspective(glm::radians(cam.Zoom), (f32)SCR_WIDTH / (f32)SCR_HEIGHT, 0.1f, 100000.0f)};
        const glm::mat4 view{cam.GetViewMatrix()};
        i32 sradious{300};
        // light_x = 2;
        // light_y = 2;
        // light_z = 2;
        // sradious -= theta;
        // light_x = sradious * (sin(theta));
        // light_y = sradious * (cos(theta)) + 300;
        // light_x = sradious * (sin((theta)));
        // light_y = sradious * -(cos(sin(theta)));
        light_z = 0.0f;
        Physics::Vector3Dimensional lvector{light_x, light_y, light_z};
        Physics::Vector3Dimensional lfriction{light_x, light_y, light_z};
        lfriction.scale(-1);
        lfriction.normalize();
        lfriction.scale(0.1);
        // light_z = sradious * cos(theta);
        lmodel = glm::translate(lmodel, glm::vec3{light_x, 10, light_z});
        const glm::vec3 lightPos{light_x, light_y, light_z};
        // lmodel = glm::rotate<f32>(lmodel, (f32)glfwGetTime() * 2, glm::vec3{10.0, 0.5, 0.02});
        lmodel = glm::scale(lmodel, glm::vec3{1.8f, 1.8f, 1.8f});
        light_shader.use();
        light_shader.setMat4("model", lmodel);
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("view", view);
        glBindVertexArray(sphereVAO);
        renderSphere();
        // const i32 mass = object.mass;
        // Physics::Vector3Dimensional wind{0.001f, 0.0f, 0.002f};
        // Physics::Vector3Dimensional gravity{0.0f, 0.1f * mass, 0.0f};
        // const f32 friction_constant = 10.1f;
        // Physics::Vector3Dimensional friction_vector{object.get_vector_copy(Physics::MovingObject::Vectors::Velocity)};
        // friction_vector.scale(-1);
        // friction_vector.normalize();
        // friction_vector.scale(friction_constant);

        // Physics::Vector3Dimensional distance{object.location_vector.magnitude() - Physics::Vector3Dimensional{light_x, light_y, light_z}.magnitude()};
        // object.accumulate_force(friction_vector);
        // object.accumulate_force(gravity);
        // object.accumulate_force(wind);
        // object.location_vector = lvector;
        // object.accumulate_force(lfriction);
        // object.accumulate_force(wind);

        // object.copy_vector_data(Physics::Vector3Dimensional{light_x, light_y, light_z});
        // Physics::Vector3Dimensional copied_location_vector(object.get_vector_copy(Physics::MovingObject::Vectors::Location));
        // Physics::Vector3Dimensional copied_velocity_vector(object.get_vector_copy(Physics::MovingObject::Vectors::Velocity));
        // const f32 v_angle{Physics::DirectionVector3Dimensional::dot_product(copied_location_vector, Physics::Vector3Dimensional{light_x, light_y, light_z}) / (copied_location_vector.magnitude() * Physics::Vector3Dimensional{light_x, light_y, light_z}.magnitude())};
        // object.angle = atan2(light_x, light_z);
        // object.update();
        // object.checkEdges();
        i32 screen_height = SCR_HEIGHT;
        i32 screen_width = SCR_WIDTH;

        const f32 mid_x = 0.5f;
        const f32 mid_y = 0.5f;

        float normalizedX = 2.0f * mid_x / screen_width - 1.0f;
        float normalizedY = 1.0f - 2.0f * mid_y / screen_height;

        glm::mat4 invProjectionMatrix = glm::inverse(projection);
        glm::mat4 invViewMatrix = glm::inverse(view);

        glm::vec4 cameraCoords = invProjectionMatrix * glm::vec4(normalizedX, normalizedY, -1.0f, 1.0f);
        cameraCoords.z = -1.0f;
        cameraCoords.w = 0.0f;

        glm::vec4 worldCoords = invViewMatrix * cameraCoords;

        glm::vec3 rayDirection = glm::normalize(glm::vec3(worldCoords));

        // glm::vec3 rayOrigin = glm::vec3(cam.Position.x * glfwGetTime(), cam.Position.y * glfwGetTime(), cam.Position.z * glfwGetTime());

        // model = glm::translate(model, glm::vec3{object.location_vector.x, object.location_vector.y, object.location_vector.z});
        glm::vec3 grav = glm::vec3(0, -0.008, 0);
        current_bullet = bullets[BULLETAMMOUNT];
        if (!current_bullet.was_shot)
        {
            current_bullet.btheta = 1;
            current_bullet.bullet_positionONCE = cam.Position;
            current_bullet.bullet_velocityONCE = cam.Front;
        }
        if (current_bullet.was_shot)
        {
            current_bullet.btheta += 0.1;
            BULLETAMMOUNT -= 1;
        }
        current_bullet.bullet_position = current_bullet.bullet_positionONCE;
        current_bullet.bullet_velocity += current_bullet.bullet_velocityONCE;
        current_bullet.bullet_velocity *= current_bullet.btheta;

        // glm::vec3 ball_pos;
        // glm::vec3 ballVelocity;
        // if (!shot)
        // {
        //     theta = 1;
        //     ball_posCLONE = cam.Position;
        //     ballVelocityCLONE = cam.Front;
        // };
        // ball_pos = ball_posCLONE;
        // ballVelocity += ballVelocityCLONE;
        // ballVelocity *= theta;
        // glm::vec3 updatedBallPosition = glm::vec3{(ball_pos.x + ballVelocity.x), (ball_pos.y + ballVelocity.y), (ball_pos.z + ballVelocity.z)};

        glm::vec3 updatedBallPosition = glm::vec3{(current_bullet.bullet_position.x + current_bullet.bullet_velocity.x), (current_bullet.bullet_position.y + current_bullet.bullet_velocity.y), (current_bullet.bullet_position.z + current_bullet.bullet_velocity.z)};

        // ballVelocity = glm::vec3{0};
        model = glm::translate(model, glm::vec3{updatedBallPosition.x, updatedBallPosition.y, updatedBallPosition.z});
        model = glm::scale(model, {0.2f, 0.2f, 0.2f});
        // model = glm::rotate(model, atan2(object.velocity_vector.x, object.velocity_vector.y), glm::vec3{0, 0, 1});
        const f32 dist = glm::sqrt(((glm::pow((light_x - updatedBallPosition.x), 2)) + (glm::pow((10 - updatedBallPosition.y), 2)) + (glm::pow((light_z - updatedBallPosition.z), 2))));
        if (current_bullet.was_shot)
        {
            current_bullet.btheta += dist < 2.4 ? 0 : 0.8;
        };
        std::string title = !shot ? ("x-" + std::to_string(cam.Position.x) + " y-" + std::to_string(cam.Position.y) + " z-" + std::to_string(cam.Position.z) + "[" + std::to_string(cam.Front.x * theta) + " " + std::to_string(cam.Front.y) + " " + std::to_string(cam.Front.z)) : "shot" + std::to_string(BULLETAMMOUNT);
        glfwSetWindowTitle(window, title.c_str());
        tshader.use();
        // Math::Matrix4Dimensional<f64>::rotateY(&matrix, glfwGetTime(), Physics::Vector3Dimensional{22});
        tshader.setMat4("projection", projection);
        tshader.setMat4("model", model);
        // tshader.setMat4("model", glm::mat4(1.0f));
        tshader.setMat4("view", view);
        tshader.setVec3("lightPos", glm::vec3{light_x, 10, light_z});
        tshader.setVec3("viewPos", cam.Position);
        tshader.setFloat("height", 10);
        f32 ii = 0.0;
        // for (f32 x = 0; x < 30; x += 0.2)
        // {
        //     for (f32 y = 0; y < 30; y += 0.2)
        //     {
        //         ii += 0.8;
        //         model = glm::mat4(1.0f);
        //         // theta += 0.2;
        //         // model = glm::translate(model, glm::vec3{i, j, 2 * sin(sqrt((i * i) + (glfwGetTime() * 15) + (j* j) + (glfwGetTime() * 15)))});
        //         // model = glm::translate(model, glm::vec3{j, 2 * sin(sqrt((i * i) + (glfwGetTime() * 100) + (j * j) + (glfwGetTime() * 100))), i});
        //         model = glm::translate(model, {x, (sin(glm::sqrt(cos(x * x) + sin(y * y)))), y});

        //         tshader.setMat4("model", model);
        //         // glBindVertexArray(sphereVAO);
        //         // renderSphere();
        //         glBindVertexArray(cubeVAO);
        //         glDrawArrays(GL_TRIANGLES, 0, 36);
        //     }
        // };
        // const i16 numStrips = (height - 1) / 1;
        // const i16 numTrisPerStrip = (height / 1) * 2 - 2;
        // smooth_terrainVAO.bind();
        // for (ui32 strip = 0; strip < numStrips; strip++)
        // {
        //     glDrawElements(GL_TRIANGLE_STRIP,
        //                    numTrisPerStrip + 2,
        //                    GL_UNSIGNED_INT,
        //                    (void *)(sizeof(ui32) * (numTrisPerStrip + 2) * strip));
        // };
        renderSphere();
        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // wmodel = glm::translate(wmodel, glm::vec3{0, 0, 0});
        // wmodel = glm::scale(wmodel, glm::vec3{40});
        // water_shader.use();
        // water_shader.setMat4("model", wmodel);
        // water_shader.setMat4("projection", projection);
        // water_shader.setMat4("view", view);
        // water_shader.setVec3("lightPos", glm::vec3{light_x, light_y, light_z});
        // water_shader.setVec3("viewPos", cam.Position);
        // glPolygonMode(GL_LINE, GL_FRONT_AND_BACK);
        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    };
    glfwTerminate();
    return 0;
};
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        current_bullet.was_shot = true;
        current_bullet.btheta += 0.6;
    }
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
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        per += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        per -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        magnitude -= 1;
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