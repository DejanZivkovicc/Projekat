#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(const char *path);

// Window settings
#define SCR_WIDTH (800)
#define SCR_HEIGHT (600)

// Camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Colors", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader cubeShader("testShaders/cubeShader.vs", "testShaders/cubeShader.fs");

    float vertices[] = {
//            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//            0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
//            0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
//            0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//            0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
//            0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//            0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
//            0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
//            0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
//
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
//            0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
//            0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//            0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
//            0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            15.0f, -0.5f,  15.0f,  2.0f, 0.0f,
            -15.0f, -0.5f,  15.0f,  0.0f, 0.0f,
            -15.0f, -0.5f, -15.0f,  0.0f, 2.0f,

            15.0f, -0.5f,  15.0f,  2.0f, 0.0f,
            -15.0f, -0.5f, -15.0f,  0.0f, 2.0f,
            15.0f, -0.5f, -15.0f,  2.0f, 2.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, -4.0f, -2.0f),
            glm::vec3(2.0f, -4.0f, -2.0f),
            glm::vec3(4.0f, -4.0f, -2.0f),

            glm::vec3(0.0f, -4.0f, -4.0f),
            glm::vec3(2.0f, -4.0f, -4.0f),
            glm::vec3(4.0f, -4.0f, -4.0f),

            glm::vec3(0.0f, -4.0f, -6.0f),
            glm::vec3(2.0f, -4.0f, -6.0f),
            glm::vec3(4.0f, -4.0f, -6.0f)
    };

    unsigned int cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int grassTexture = loadTexture("textures/grass.png");
    cubeShader.use();
    cubeShader.setInt("grassTexture", 0);

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeShader.use();
        cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        // View/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

//        // World transformation
//        glm::mat4 model = glm::mat4(1.0f);
//        // model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
//        cubeShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);

        glBindVertexArray(cubeVAO);
//        for(unsigned int i = 0; i < 9; i++){
//            // Calculate the model matrix for each object and pass it to shader before drawing
//            glm::mat4 model = glm::mat4(1.0);
//
//            model = glm::translate(model, cubePositions[i]);
//            model = glm::scale(model, glm::vec3(2.0f));
//
//            cubeShader.setMat4("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }
        // floor
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glm::mat4 model = glm::mat4(1.0f);
        cubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}