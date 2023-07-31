#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/importer.hpp>

#include "mesh/mesh.h"
#include "shader/shader.h"
#include "window/window.h"
#include "camera/camera.h"
#include "text/text.h"
#include "texture/texture.h"
#include "light/light.h"
#include "material/material.h"
#include "light/directional-light.h"
#include "light/spot-light.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera* camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void CreateShader() {
    auto* generalShader = new Shader();
    generalShader->CreateFromFile("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    shaderList.push_back(generalShader);

    auto* textShader = new Shader();
    textShader->useTransparentBlending = true;
    textShader->CreateFromFile("shaders/textVertexShader.glsl", "shaders/textFragmentShader.glsl");
    shaderList.push_back(textShader);
}

void CalculateAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
                             unsigned int vLength, unsigned int normalOffset) {
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects() {
    unsigned int indices[] = {
           0, 3, 1,
           1, 3, 2,
           2, 3, 0,
           0, 1, 2
    };

    GLfloat vertices[] = {
            //	  x           y          z			     u	        v			     nx	        ny       nz
            -1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
            0, 2, 1,
            1, 2, 3
    };

    GLfloat floorVertices[] = {
            -10.0f, 0.0f, -10.0f,      0.0f, 0.0f,           0.0f,-1.0f, 0.0f,
            10.0f, 0.0f, -10.0f,      10.0f, 0.0f,        0.0f,-1.0f, 0.0f,
            -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,        0.0f,-1.0f, 0.0f,
            10.0f, 0.0f, 10.0f,    10.0f, 10.0f,       0.0f,-1.0f, 0.0f
    };

    CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh *object1 = new Mesh();
    object1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(object1);

    Mesh *object2 = new Mesh();
    object2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(object2);

    Mesh *object3 = new Mesh();
    object3->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(object3);
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
    brickTexture.LoadTextureAlpha();

    std::string dirtTexturePath = "textures/dirt.png";
    dirtTexture = Texture(dirtTexturePath.c_str());
    dirtTexture.LoadTextureAlpha();

    std::string plainTexturePath = "textures/plain.png";
    plainTexture = Texture(plainTexturePath.c_str());
    plainTexture.LoadTextureAlpha();

    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    mainLight = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.1f,glm::vec3(9.0f, -1.0f, -9.0f));

    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.1f, 0.1f, glm::vec3(4.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    pointLights[1] = PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 0.1f, glm::vec3(-4.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f);

    unsigned int spotLightCount = 1;
    spotLights[0] = SpotLight(
            glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.2f,
            glm::vec3(0.0f, 1.5f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            1.0f, 0.0f, 0.0f, 20.0f
    );

    GLuint uniformModelId{}, uniformProjectionId{}, uniformView{}, uniformEyePosition{},
        uniformSpecularIntensity{}, uniformShininess{};
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

        uniformEyePosition = shaderList[0]->GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
        uniformShininess = shaderList[0]->GetShininessLocation();

        glm::vec3 lowerLight = camera->getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlashlight(lowerLight, camera->getCameraDirection());

        shaderList[0]->SetDirectionalLight(&mainLight);
        shaderList[0]->SetPointLights(pointLights, pointLightCount);
        shaderList[0]->SetSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjectionId, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));

        brickTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 4.0f, -2.5f));
        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        plainTexture.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

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
