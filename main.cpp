#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "filesystem.h"
#include "shader.h"
#include "PerlinNoise.hpp"
#include "defines.hpp"
#include <unistd.h>
#include <ctime>
#define SCR_WIDTH 1600
#define SCR_HEIGHT 1600

i32 magnitude = 4;
i64 cube_m = 1;
f64 per = 0.5;
f32 map[1000][1000];
void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height);
void mouse_callback(GLFWwindow *window, f64 xpos, f64 ypos);
void scroll_callback(GLFWwindow *window, f64 xoffset, f64 yoffset);
void processInput(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(-1.0f, 5.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
f32 yaw = -90.0f;
f32 pitch = 0.0f;
f32 lastX = 800.0f / 2.0;
f32 lastY = 600.0 / 2.0;
f32 fov = 100.0f;

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

bool to_rotate = false;
bool one = false;
glm::vec3 cubePositions[1000000]{};
template <size_t n = 50000>
void jump_y(glm::vec3 cube_pos[(i32)n])
{
    for (i32 i = 0; i < 1000; i++)
    {
        cube_pos[i] = glm::vec3{0.0f, (f32)i, 0.0f};
    };
    return;
};
void change_magnitude(f32 map[1000][1000], i32 magnitude)
{
    i32 iter = 0;
    const siv::PerlinNoise::seed_type seed = 123456u * magnitude;
    const siv::PerlinNoise perlin{seed};
    for (i32 i = 0; i < cube_m; i++)
    {
        for (i32 j = 0; j < cube_m; j++)
        {
            const double noise = perlin.octave2D((f64)((i * 0.003)), (f64)((j * 0.003)), (i32)magnitude, per);
            map[i][j] = (f32)(noise * 250);
        };
    };
#define h_magnitude 25
    for (i32 i = 0; i < cube_m; i++)
    {
        for (i32 j = 0; j < cube_m; j++)
        {
            iter++;
            cubePositions[iter] = glm::vec3{(f32)j, (f32)map[i][j], (f32)i};
        };
    }
    return;
};
int main()
{
    const siv::PerlinNoise::seed_type seed = 123456u;
    const siv::PerlinNoise perlin{seed};
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JORDEN-IO", NULL, NULL);
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
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("camera.vs", "camera.fs");
    f32 colors[40] = {
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};
#define SHOWV 0
    f32 vertices[200] = {
#ifdef SHOWV

        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        -0.5f,
        1.0f,

        0.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        -0.5f,

        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,

        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,

        -0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        -0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        -0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,

        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
#endif

        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
#ifdef SHOWV

        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        -0.5f,
        0.5f,
        0.5f,
        0.0f,
        0.0f,
        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,

#endif
    };
    i32 iter = 0;
// #define STAIRS 0
#define CLOSE 0
#ifndef CLOSE
    for (i32 k = 0; k < 100; k += 5)
    {
        for (i32 i = 0; i < 100; i += 5)
        {
            for (i32 j = 0; j < 100; j += 5)
            {
                iter++;
#ifdef STAIRS
                cubePositions[iter] = glm::vec3{(f32)j, iter % 1 == 0 ? (f32)j : (f32)iter, (f32)i};
#else
                cubePositions[iter] = glm::vec3{(f32)i, (f32)j, (f32)k};
#endif
            }
        }
    }
#else
#define MAG 4
    for (i32 i = 0; i < cube_m; i++)
    {
        for (i32 j = 0; j < cube_m; j++)
        {
            const f64 noise = perlin.octave2D_01((f64)((i * 0.025)), (f64)((j * 0.0025)), (i32)magnitude);
            map[i][j] = (f32)(noise * 550);
        };
    };
#define h_magnitude 25
    i32 inner_iter = iter;
    for (i32 i = 0; i < cube_m; i++)
    {
        for (i32 j = 0; j < cube_m; j++)
        {
            iter++;
            cubePositions[iter] = glm::vec3{(f32)j, (f32)map[i][j], (f32)i};
        };
    }
//     iter = 0;
//     for (i32 k = 0; k < 10; k++)
//     {
//         for (i32 i = 0; i < 10; i++)
//         {
//             for (i32 j = 0; j < 10; j++)
//             {
//                 iter++;
// #ifdef STAIRS
//                 cubePositions[iter] = glm::vec3{(f32)j, iter % 1 == 0 ? (f32)j : (f32)iter, (f32)i};
// #else
//                 cubePositions[iter] = glm::vec3{(f32)i, (f32)k, (f32)j};
// #endif
//             }
//         }
//     }
#endif
    ui32 VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    ourShader.use();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<f32>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(fov), (f32)SCR_WIDTH / (f32)SCR_HEIGHT, 0.1f, 10000.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        glBindVertexArray(VAO);
        for (ui32 i = 0; i <= cube_m * cube_m; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, (cubePositions[i]));
            f32 angle = 10.0f * i;
            // if (one)
            // {
            //     for (i32 i = 1; i <= 1000; i++)
            //     {
            //         cubePositions[i] = glm::vec3{};
            //     }
            // };
            if (to_rotate)
            {
                angle = 30.0f * i * std::rand();
                model = glm::rotate(model,
                                    (f32)glfwGetTime(),
                                    // * std::rand() + std::rand(),
                                    // glm::radians(angle)
                                    glm::vec3(-1.0f, 0.3f, 0.5f));
            };
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    f32 cameraSpeed = static_cast<f32>(8.5 * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        cameraSpeed += 1;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        cube_m += 1;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cube_m -= 1;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        per += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        per -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        magnitude += 1;
    change_magnitude(map, magnitude);
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        magnitude -= 1;
    change_magnitude(map, magnitude);
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        for (i32 i = 0; i < 200; i++)
            for (i32 j = 0; j < 200; j++)
                cubePositions[i] = glm::vec3{static_cast<f32>(j), 0.0f, static_cast<f32>(i)};
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        jump_y<>(cubePositions);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        one = true;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        to_rotate = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow *window, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, f64 xposIn, f64 yposIn)
{
    f32 xpos = static_cast<f32>(xposIn);
    f32 ypos = static_cast<f32>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    f32 xoffset = xpos - lastX;
    f32 yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    f32 sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow *window, f64 xoffset, f64 yoffset)
{
    fov -= (f32)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
};