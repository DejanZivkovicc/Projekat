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
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(const char *path);
std::vector<float> generateSphereVertices(float radius, unsigned int sectors, unsigned int stacks);

// Window settings
#define SCR_WIDTH (1280)
#define SCR_HEIGHT (800)

// Camera settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 6.0f);

glm::vec3 lightPos1(1.2f, 1.0f, 6.0f);
glm::vec3 lightPos2(1.8f, 0.5f, -3.2);

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

    Shader lightCubeShader("shaders/lightCubeShader.vs", "shaders/lightCubeShader.fs");
    Shader floorShader("shaders/floorShader.vs", "shaders/floorShader.fs");
    Shader grassShader("shaders/grassShader.vs", "shaders/grassShader.fs");
    Shader soadShader("shaders/soadShader.vs", "shaders/soadShader.fs");
    Shader blackBackgroundShader("shaders/blackBackground.vs", "shaders/blackBackground.fs");
    Shader sphereShader("shaders/sphereShader.vs", "shaders/sphereShader.fs");

    float lightCubeVertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
    };

    float floorVertices[] = {
            // positions              // texture Coords     // Normal Coords
            15.0f, -2.5f,  15.0f,       2.0f, 0.0f,         0.0f, 1.0f, 0.0f,
            -15.0f, -2.5f,  15.0f,      0.0f, 0.0f,         0.0f, 1.0f, 0.0f,
            -15.0f, -2.5f, -15.0f,      0.0f, 2.0f,         0.0f, 1.0f, 0.0f,

            15.0f, -2.5f,  15.0f,       2.0f, 0.0f,         0.0f, 1.0f, 0.0f,
            -15.0f, -2.5f, -15.0f,      0.0f, 2.0f,         0.0f, 1.0f, 0.0f,
            15.0f, -2.5f, -15.0f,       2.0f, 2.0f,         0.0f, 1.0f, 0.0f
    };

    float soadVertices[] = {
            0.0f,  0.5f,  0.0f,     0.0f,  0.0f,        0.0f, 1.0f, 0.0f,
            0.0f, -0.5f,  0.0f,     0.0f,  1.0f,        0.0f, 1.0f, 0.0f,
            1.0f, -0.5f,  0.0f,     1.0f,  1.0f,        0.0f, 1.0f, 0.0f,

            0.0f,  0.5f,  0.0f,     0.0f,  0.0f,        0.0f, 1.0f, 0.0f,
            1.0f, -0.5f,  0.0f,     1.0f,  1.0f,        0.0f, 1.0f, 0.0f,
            1.0f,  0.5f,  0.0f,     1.0f,  0.0f,        0.0f, 1.0f, 0.0f
    };

    float blackBackgroundVertices[] = {
            0.0f,  0.5f,  0.0f,
            0.0f, -0.5f,  0.0f,
            1.0f, -0.5f,  0.0f,

            0.0f,  0.5f,  0.0f,
            1.0f, -0.5f,  0.0f,
            1.0f,  0.5f,  0.0f
    };

    float grassVertices[] = {
            // positions         // texture Coords          // Normal Coords
            0.0f,  0.5f,  0.0f,     0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,     0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,     1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,     0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,     1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,     1.0f,  0.0f

            // texture coords have swapped y coordinates because texture is flipped upside down
    };

    // transparent vegetation locations
    std::vector<glm::vec3> vegetation{
            glm::vec3(-1.5f, -2.0f, -4.48f),
            glm::vec3( 1.5f, -2.0f, 0.51f),
            glm::vec3( 6.0f, -2.0f, 3.7f),
            glm::vec3(-2.3f, -2.0f, -7.3f),
            glm::vec3 (5.5f, -2.0f, -5.6f)

    };

    // light VAO
    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightCubeVertices), lightCubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // floor VAO
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // soad VAO
    unsigned int soadVAO, soadVBO;
    glGenVertexArrays(1, &soadVAO);
    glGenBuffers(1, &soadVBO);
      glBindVertexArray(soadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, soadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(soadVertices), soadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // blackBackground VAO
    unsigned int blackBackgroundVAO, blackBackgroundVBO;
    glGenVertexArrays(1, &blackBackgroundVAO);
    glGenBuffers(1, &blackBackgroundVBO);
    glBindVertexArray(blackBackgroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, blackBackgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackBackgroundVertices), blackBackgroundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // sphere VAO
    unsigned int sphereVAO, sphereVBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glBindVertexArray(sphereVAO);

    std::vector<float> sphereVertices = generateSphereVertices(1.0f, 36, 18); // Poluprečnik 1.0, 36 sektora i 18 slojeva
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // grass VAO
    unsigned int grassVAO, grassVBO;
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // load textures
    unsigned int floorTexture = loadTexture("textures/floor.png");
    unsigned int grassTexture = loadTexture("textures/grass.png");
    unsigned int soadTexture = loadTexture("textures/soad.jpg");
    unsigned int sphereTexture = loadTexture("textures/glass.jpg");

    // Shader configuration
    floorShader.use();
    floorShader.setInt("material.diffuse", 0);
    floorShader.setInt("material.specular", 1);

    soadShader.use();
    soadShader.setInt("material.diffuse", 0);
    soadShader.setInt("material.specular", 1);

    sphereShader.use();
    sphereShader.setInt("sphereTexture", 0);

    grassShader.use();
    grassShader.setInt("grassTexture", 0);

    /* ------------------------------------------------------------------------ */
    /* ----------------------------- RENDER LOOP ------------------------------ */
    /* -------------------------------------------------------------------------*/

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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


        // --------------------------------floor--------------------------------
        // Activating shader when setting uniforms/drawing objects
        floorShader.use();
        // floorShader.setVec3("light.position", lightPos);
        // floorShader.setVec3("viewPos", camera.Position);
        // light properties
//        floorShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//        floorShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//        floorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // Light cube
        floorShader.setVec3("light1.position", lightPos1);
        floorShader.setVec3("light1.ambient", 0.2f, 0.2f, 0.2f);
        floorShader.setVec3("light1.diffuse", 0.5f, 0.5f, 0.5f);
        floorShader.setVec3("light1.specular", 1.0f, 1.0f, 1.0f);

        // Pendulum
        floorShader.setVec3("light2.position", lightPos2);
        floorShader.setVec3("light2.ambient", 0.2f, 0.2f, 0.2f);
        floorShader.setVec3("light2.diffuse", 0.5f, 0.5f, 0.5f);
        floorShader.setVec3("light2.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        floorShader.setFloat("material.shininess", 64.0f);

        // View/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", view);

        // World transofrmation
        glm::mat4 model = glm::mat4(1.0f);
        floorShader.setMat4("model", model);

        // floor
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);

        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // --------------------------------soad--------------------------------
        soadShader.use();
        soadShader.setVec3("light.position", lightPos);
        soadShader.setVec3("viewPos", camera.Position);

        // light properties
        soadShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        soadShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        soadShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        soadShader.setFloat("material.shininess", 64.0f);

        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        soadShader.setMat4("projection", projection);
        soadShader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, soadTexture);
        glBindVertexArray(soadVAO);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.2f, 4.0f, 3.2f)); // a smaller cube
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, -2.0));
        soadShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // --------------------------------black background--------------------------------
        blackBackgroundShader.use();
        blackBackgroundShader.setMat4("projection", projection);
        blackBackgroundShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.2f, 4.0f, 3.2f)); // a smaller cube
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, -2.01));
//        model = glm::translate(model, lightPos);
//        model = glm::scale(model, glm::vec3(1.0f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(blackBackgroundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // --------------------------------Sphere--------------------------------
        // Ažuriranje vremena
        float time = glfwGetTime();
        float speed = 2.0f; // brzina oscilacije

        // Izračunavanje translacije
        float x = sin(time * speed) * 2.0f; // oscilacija levo-desno
        float y = abs(cos(time * speed)) * (-2.0f); // oscilacija gore-dole

        // Transformacija
        sphereShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, -3.0f));
//        model = glm::translate(model, lightPos);
        model = glm::translate(model, lightPos2);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        sphereShader.setMat4("model", model);
        sphereShader.setMat4("view", view);
        sphereShader.setMat4("projection", projection);

        // Renderovanje
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sphereTexture);

        glBindVertexArray(sphereVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sphereVertices.size() / 3);

        // --------------------------------vegetation--------------------------------
        grassShader.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        grassShader.setMat4("projection", projection);
        grassShader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glBindVertexArray(grassVAO);

        for (unsigned int i = 0; i < vegetation.size(); i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, vegetation[i]);
            grassShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // --------------------------------lamp object--------------------------------
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(1.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Utility function for loading a 2D texture from file
unsigned int loadTexture(char const * path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data){
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
    else{
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// Function for generating sphere vertex
std::vector<float> generateSphereVertices(float radius, unsigned int sectors, unsigned int stacks) {
    std::vector<float> vertices;

    float x, y, z, xy;                          // pozicije verteksa
    float sectorStep = 2 * M_PI / sectors;      // ugao po longitudinalnoj liniji
    float stackStep = M_PI / stacks;            // ugao po latitudinalnoj liniji
    float sectorAngle, stackAngle;

    for(unsigned int i = 0; i <= stacks; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        // pocevsi od pi/2 do -pi/2
        xy = radius * cosf(stackAngle);             // precnik * kosinus od latituda
        z = radius * sinf(stackAngle);              // precnik * sinus od latituda

        for(unsigned int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;           // longitudinalni ugao

            // pozicije verteksa
            x = xy * cosf(sectorAngle);             // x = r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // y = r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    return vertices;
}
