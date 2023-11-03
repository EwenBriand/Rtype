/*
** EPITECH PROJECT, 2023
** graph
** File description:
** main.cpp
*/

#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Window.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "Uniform.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "ShaderBuildSystem.hpp"
#include "Rectangle.hpp"

#define DRAW_RECTANGLE 0
#define WITH_UNIFORM 1
#define WITH_TEXTURE 2
#define WITH_SPRITE 3

#define MODE WITH_SPRITE

#if MODE == DRAW_RECTANGLE

int main() {
    // draw a simple rectangle using the vao, vbo and ebo classes
    lg::Window window(800, 600, "LightGraph");
    float vertices[] {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.5f,  0.5f, 0.0f, // top
        -0.5f,  0.5f, 0.0f  // bottom
    };

    unsigned int indices[] {
        0, 1, 2,
        2, 3, 0
    };

    lg::VAO vao;
    lg::VBO vbo(vertices, sizeof(vertices));
    lg::EBO ebo(indices, sizeof(indices));

    lg::ShaderProgram program("default");
    program.AddShader("../shaders/old/default.vert", 0, GL_VERTEX_SHADER);
    program.AddShader("../shaders/old/default.frag", 1, GL_FRAGMENT_SHADER);
    program.Link();

    while (!window.ShouldClose()) {
        window.PollEvents();
        window.Clear();
        program.Use();
        vao.Bind();
        program.AssignVBO(vbo);
        ebo.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        window.Draw();
    }

    return 0;
}

#endif
#if MODE == WITH_UNIFORM

int main() {
    lg::Window window(800, 600, "LightGraph");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f,  0.5f, 0.0f  // top
    };

    float textCoords[] = {
        0.0f, 0.0f, // left
        1.0f, 0.0f, // right
        0.5f,  1.0f  // top
    };

    lg::VAO vao;
    lg::VBO vbo(vertices, sizeof(vertices));

    lg::ShaderProgram program("default");
    program.AddShader("../shaders/default_with_uniform.vert", 0, GL_VERTEX_SHADER);
    program.AddShader("../shaders/old/default.frag", 1, GL_FRAGMENT_SHADER);
    program.Link();

    lg::Uniform<float> u_time("u_time", 0.0f, program);

    while (!window.ShouldClose()) {
        window.PollEvents();
        window.Clear();
        program.Use();
        u_time << (float)glfwGetTime();
        vao.Bind();
        program.AssignVBO(vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        window.Draw();
    }
    return 0;
}

#endif
#if MODE == WITH_TEXTURE

int main() {
    lg::Window window(800, 600, "LightGraph");

    // position and texture coordinates
    // float vertices[] = {
    //     // positions          // texture coords
    //     0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right
    //     0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    //     -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
    // };

    float vertices[] = {
        // positions
        0.5f, 0.5f, 0.0f,    // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f, 0.0f,    // top left
    };

    float textCoords[] = {
        // texture coords
        1.0f, 1.0f, // top right
        1.0f, 0.0f, // bottom right
        0.0f, 0.0f, // bottom left
        0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    // creating a VAO to store the VBO and EBO for quick access
    lg::VAO vao;

    // a VBO to store the positions of the vertices
    lg::VBO vbo(vertices, 4 * 3 * sizeof(float));
    vbo.SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    // a VBO to store the texture coordinates
    lg::VBO vbo2(textCoords, 4 * 2 * sizeof(float));
    vbo2.SetVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

    // an EBO to store the indices of the vertices that make triangles
    lg::EBO ebo(indices, sizeof(indices));

    // creating a shader program
    lg::ShaderProgram program("default");
    program.AddShader("../shaders/textures.vert", 0, GL_VERTEX_SHADER);
    program.AddShader("../shaders/textures.frag", 1, GL_FRAGMENT_SHADER);
    program.Link();

    // creating a texture. it will be bound to the texture unit 0 in the shader
    lg::Texture texture("../../assets/textures/wall.jpg");

    while (!window.ShouldClose()) {
        if (window.IsKeyPressedOnce(GLFW_KEY_W))
            window.ToggleWireframe();
        if (window.IsKeyPressedOnce(GLFW_KEY_ESCAPE))
            break;
        window.PollEvents();
        window.Clear();

        texture.Bind();
        vao.Bind();
        program.Use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.Draw();
    }
}

#endif
#if MODE == WITH_SPRITE

int main() {
    lg::Window window(800, 600, "LightGraph");
    lg::Sprite sprite("../../assets/textures/wall.jpg");
    lg::Camera camera;
    float t = 0;

    camera.SetPosition(glm::vec3(0, 0, -1));
    camera.SetTarget(glm::vec3(0, 0, 0));
    // camera.SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 10=.0f);
    lg::ShaderBuildSystem::Get().BuildAll();
    // print the source
    std::cout << lg::ShaderBuildSystem::Get()["spriteShader"].GetVertexShader().GetSrc() << std::endl;

    while (not window.ShouldClose()) {
        t += 0.01f;
        if (window.IsKeyPressedOnce(GLFW_KEY_W))
            window.ToggleWireframe();

        window.PollEvents();
        window.Clear();

        auto prog = *(lg::ShaderBuildSystem::Get()["spriteShader"].Get());
        prog.Use();
        camera.UpdateView(prog);
        sprite.Draw();
        window.Draw();
    }
    return 0;
}

// int main() {
//     lg::Window window(800, 600, "LightGraph");
//     lg::Rectangle rect = lg::Rectangle({0, 0}, {0.5, 0.5}, {1, 0, 1, 1}, 0.1, {0, 1, 0, 1});

//     lg::ShaderBuildSystem::Get().BuildAll();

//     while (not window.ShouldClose()) {
//         if (window.IsKeyPressedOnce(GLFW_KEY_W))
//             window.ToggleWireframe();
//         window.PollEvents();
//         window.Clear();
//         rect.Draw();
//         window.Draw();
//     }
// }


// int main() {
//     glm::vec3 unitCubeVertices[] = {
//         // front
//         glm::vec3(-0.5f, -0.5f,  0.5f), // bottom left
//         glm::vec3( 0.5f, -0.5f,  0.5f), // bottom right
//         glm::vec3( 0.5f,  0.5f,  0.5f), // top right
//         glm::vec3(-0.5f,  0.5f,  0.5f), // top left
//         // back
//         glm::vec3(-0.5f, -0.5f, -0.5f), // bottom left
//         glm::vec3( 0.5f, -0.5f, -0.5f), // bottom right
//         glm::vec3( 0.5f,  0.5f, -0.5f), // top right
//         glm::vec3(-0.5f,  0.5f, -0.5f), // top left
//     };

//     int unitCubeTriangleIndexes[] = {
//         // front
//         0, 1, 2,
//         2, 3, 0,
//         // right
//         1, 5, 6,
//         6, 2, 1,
//         // back
//         7, 6, 5,
//         5, 4, 7,
//         // left
//         4, 0, 3,
//         3, 7, 4,
//         // bottom
//         4, 5, 1,
//         1, 0, 4,
//         // top
//         3, 2, 6,
//         6, 7, 3
//     };

//     glm::vec3 cameraPosition(1.0f, 1.0f, 1.0f);
//     glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
//     glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
//     float cameraFov = 3.14159f / 2.0f;

//     float pixelCoordsLayoutSrc[800 * 600 * 2];
//     for (int i = 0; i < 800 * 600; i++) {
//         int x = i % 800;
//         int y = i / 800;
//         pixelCoordsLayoutSrc[i * 2] = ((float)x / 800.0f - 0.5f);
//         pixelCoordsLayoutSrc[i * 2 + 1] = (float)y / 600.0f - 0.5f;
//     }

//     lg::Window window(800, 600, "raytracer");

//     lg::ShaderProgram program("default");
//     program.AddShader("../shaders/raytracing.vs", 0, GL_VERTEX_SHADER);
//     program.AddShader("../shaders/raytracing.fs", 1, GL_FRAGMENT_SHADER);
//     program.Link();

//     lg::VAO vao;
//     lg::VBO vbo(pixelCoordsLayoutSrc, sizeof(pixelCoordsLayoutSrc));
//     vbo.SetVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

//     std::cout << "Starting render loop" << std::endl;
//     while (not window.ShouldClose()) {
//         window.PollEvents();
//         window.Clear();
//         program.Use();
//         vao.Bind();
//         program.AssignVBO(vbo);
//         glDrawArrays(GL_POINTS, 0, 800 * 600);
//         window.Draw();
//     }
// }

#endif
