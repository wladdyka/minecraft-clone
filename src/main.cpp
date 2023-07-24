#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh/mesh.h"
#include "shader/shader.h"
#include "window/window.h"
#include "camera/camera.h"
#include "text/text.h"
#include "texture/texture.h"
#include "light/light.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera* camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void CreateShader() {
    auto* generalShader = new Shader();
    generalShader->CreateFromFile("shaders/shader.vertex", "shaders/shader.fragment");
    shaderList.push_back(generalShader);

    auto* textShader = new Shader();
    textShader->CreateFromFile("shaders/text.vertex", "shaders/text.fragment");
    shaderList.push_back(textShader);
}

void CreateObjects() {
    unsigned int indices[] = {
           0, 3, 1,
           1, 3, 2,
           2, 3, 0,
           0, 1, 2
    };

    GLfloat vertices[] = {
            //     x,         y,          z,          u,        v,
            -1.0f,-1.0f,  0.0f,  0.0f, 0.0f,
            0.0f,-1.0f,  1.0f,  0.5f, 0.0f,
            1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 1.0f
    };

    Mesh *object1 = new Mesh();
    object1->CreateMesh(vertices, indices, 20, 12);
    meshList.push_back(object1);

    Mesh *object2 = new Mesh();
    object2->CreateMesh(vertices, indices, 20, 12);
    meshList.push_back(object2);
}

int main() {
    const unsigned int width = 1920, height = 1080;

    auto* mainWindow = new Window(width, height);
    mainWindow->Init();

    auto* textSystem = new Text(width, height);

    CreateObjects();
    CreateShader();
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.4f);

    std::string brickTexturePath = "textures/brick.png";
    brickTexture = Texture(brickTexturePath.c_str());
    brickTexture.LoadTexture();

    std::string dirtTexturePath = "textures/dirt.png";
    dirtTexture = Texture(dirtTexturePath.c_str());
    dirtTexture.LoadTexture();

    mainLight = Light(glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);

    GLuint uniformModelId, uniformProjectionId, uniformView, uniformAmbientIntensity, uniformAmbientColour;
    glm::mat4 projection = glm::perspective(45.0f, mainWindow->aspectRation(), 0.1f, 100.0f);
    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

    while(!mainWindow->getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        camera->keyControl(mainWindow->getKeys(), deltaTime);
        camera->mouseControl(mainWindow->getMouseChangeX(), mainWindow->getMouseChangeY());

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* drawing 3d Content */
        shaderList[0]->UseShader();
        uniformModelId = shaderList[0]->GetModelLocation();
        uniformProjectionId = shaderList[0]->GetProjectionLocation();
        uniformView = shaderList[0]->GetViewLocation();
        uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
        uniformAmbientColour = shaderList[0]->GetAmbientColourLocation();

        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        //model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjectionId, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

        brickTexture.UseTexture();
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 2.0f, -5.0f));
        //model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        meshList[1]->RenderMesh();

        glUseProgram(0);

        /* drawing text on screen */
        shaderList[1]->UseShader();
        glUniformMatrix4fv(glGetUniformLocation(shaderList[1]->GetId(), "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));
        textSystem->RenderText(shaderList[1]->GetId(), "freetype text!", 100.0f, 1.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        glUseProgram(0);

        mainWindow->swapBuffers();
        glfwPollEvents();
    }

    return 0;
}
