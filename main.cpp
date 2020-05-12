#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <vector>

#include <Shader.h>
#include <point_cloud_builder.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  75.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // Start GLFW.
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // Requied for Mac OS likely comment out on Windows
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // Use core profile which means we need to supply our own vertex and fragment shaders.
    // But this let's us skip every 4th float which contains the LiDAR intensity which is
    // not required for a visual demo.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Oxford RobotCar LiDAR Viewer", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    // Further setup and callback settings for movement.
    glewExperimental = GL_TRUE;
    glewInit();
    glfwSetCursorPosCallback(window, mouse_callback);
    // Not tested mouse scrolling. Only have a trackpad.
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    Shader shader("../res/basic_shader.shader");
    PointCloudBuilder cloudBuilder;

    // Get all the time stamps:
    std::vector<std::string> stamps = cloudBuilder.loadTimesStamps("../data/velodyne_left.timestamps");
    unsigned int framesVAO[stamps.size()], frameBuffersVBO[stamps.size()];
    // Create VBOs and VAOs from the binary files
    glGenVertexArrays(stamps.size(), framesVAO);
    glGenBuffers(stamps.size(), frameBuffersVBO);

    int i = 0;
    int vertexCount;

    while (!glfwWindowShouldClose(window))
    {
        // The following three lines track the cumulative time between frames. Different systems will have different graphics
        // capabilities and therefore frame rates will vary. Change the "deltaTime > 0.1f" below to change the frame rate of
        // the LiDAR viewer.
        float currentFrame = glfwGetTime();
        deltaTime += currentFrame - lastFrame;
        lastFrame = currentFrame;

        // The if scope below will load the next LiDAR frame and render it. Change the frame rate by altering the
        // conditional statement. Lower numbers give higher frame rates.
        if (deltaTime > 0.1f){
            // Get the number of vertices in the next LiDAR frame.
            vertexCount = cloudBuilder.vertexCountOfCloud(stamps[i]);
            // Load the next .bin file and get pointer to it.
            float* q = cloudBuilder.getPointCLoud(stamps[i]);
            // Create new vertex array and buffer.
            glBindVertexArray(framesVAO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, frameBuffersVBO[i]);
            // Fill the buffer with the data.
            glBufferData(GL_ARRAY_BUFFER, (vertexCount * sizeof(float)), q, GL_STATIC_DRAW);

            // Set the attribute pointer to outline the data structure.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Bind it for rendering.
            glBindVertexArray(framesVAO[i]);
            // Reset deltaTime, and increment i
            deltaTime = 0.0f;
            i++;
            // Reset i if we have reached the final file. This will make a video loop.
            if(i == stamps.size()) {
                i = 0;
            }
        }

        processInput(window);
        // Black background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        shader.SetUniformMat4f("projection", projection);

        // View transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.SetUniformMat4f("view", view);

        // Leave the model matrix as identity because we aren't moving the model.
        glm::mat4 model = glm::mat4(1.0f);
        shader.SetUniformMat4f("model", model);
        // Draw each LiDAR dot in a light colour.
        shader.SetUniform4f("u_Colour", 0.8f, 0.8f, 0.9f, 1.0f);
        // And finally, draw the scene.
        glDrawArrays(GL_POINTS, 0, vertexCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Clean up.
    glDeleteVertexArrays(1, framesVAO);
    glDeleteBuffers(1, frameBuffersVBO);

    glfwTerminate();
    return 0;
}

/*
 *  Standard OpenGL mouse and keyboard controls.
 */
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // Change camera speed to alter movement sensitivity.
    float cameraSpeed = 0.5;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}