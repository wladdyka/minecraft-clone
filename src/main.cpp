#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/importer.hpp>
#include <noise/noise.h>
#include <noise/noiseutils.h>
#include <array>
#include <stdexcept>
#include <random>

#include "mesh/mesh.h"
#include "shader/shader.h"
#include "window/window.h"
#include "camera/camera.h"
#include "text/text.h"
#include "texture/texture.h"
#include "material/material.h"
#include "light/directional-light.h"
#include "model/model.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera* camera;

Texture cobblestoneTexture;
Texture stoneTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;

Material shinyMaterial;
Material dullMaterial;

Model xwing;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

constexpr int chunkSizeX = 32;
constexpr int chunkSizeY = 64;
constexpr int chunkSizeZ = 32;

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

void DrawChunk(int chunkPosX, int chunkPosZ) {
    //printf("drawing chunk x:%i z:%i\n", chunkPosX, chunkPosZ);

    std::array<std::array<std::array<unsigned int, chunkSizeZ>, chunkSizeY>, chunkSizeX> blocks = {};

    std::vector<unsigned int> indices = {};
    std::vector<GLfloat> vertices = {};

    /* terrain generation part start */
    noise::module::Perlin perlinModule;

    perlinModule.SetSeed(chunkPosX * 1000 + chunkPosZ);
    perlinModule.SetOctaveCount(2);
    perlinModule.SetFrequency(0.02);
    perlinModule.SetPersistence (1.0);
    //perlinModule.SetLacunarity (2.20703125);
    //perlinModule.SetNoiseQuality (QUALITY_STD);

    double groundLevel = 30;

    // set block type here
    for (unsigned int x = 0; x < chunkSizeX; x++) {
        for (unsigned int y = 0; y < chunkSizeY; y++) {
            for (unsigned int z = 0; z < chunkSizeZ; z++) {
                double terrain = perlinModule.GetValue(x, 0, z) * 10;
                blocks.at(x).at(y).at(z) = y <= terrain + groundLevel ? 1 : 0;
            }
        }
    }

    /* terrain generation part end */

    // figure out what vertices to add to a mesh
    unsigned int vertexProcessed = 0;

    unsigned int chunkOffsetX = chunkPosX * chunkSizeX;
    unsigned int chunkOffsetZ = chunkPosZ * chunkSizeZ;

    for (unsigned int x = 0; x < chunkSizeX; x++) {
        for (unsigned int y = 0; y < chunkSizeY; y++) {
            for (unsigned int z = 0; z < chunkSizeZ; z++) {
                auto blockX = (GLfloat)(x + chunkOffsetX);
                auto blockZ = (GLfloat)(z + chunkOffsetZ);
                auto blockY = (GLfloat)y;

                //printf("drawing block at x:%f y:%f z:%f\n", blockX, blockY, blockZ);

                bool currentBlockNotAir = blocks.at(x).at(y).at(z) != 0;

                // drawing top face
                bool isBlockAboveAir;
                try {
                    isBlockAboveAir = blocks.at(x).at(y + 1).at(z) == 0;
                } catch (const std::out_of_range& e) {
                    isBlockAboveAir = y == chunkSizeY - 1;
                }

                if (currentBlockNotAir && isBlockAboveAir) {
                    // drawing ceiling face
                    indices.insert(indices.end(), {
                            vertexProcessed, vertexProcessed + 1, vertexProcessed + 2,
                            vertexProcessed + 3, vertexProcessed + 1, vertexProcessed + 2
                    });

                    vertices.insert(vertices.end(), {
                            blockX,           blockY + 1.0f,     blockZ,           0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
                            blockX,           blockY + 1.0f,     blockZ + 1.0f,    1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
                            blockX + 1.0f,    blockY + 1.0f,     blockZ,           0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
                            blockX + 1.0f,    blockY + 1.0f,     blockZ + 1.0f,    1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
                    });
                    vertexProcessed += 4;
                }

                // drawing front face
                bool isBlockInFrontAir;
                try {
                    isBlockInFrontAir = blocks.at(x).at(y).at(z - 1) == 0;
                } catch (const std::out_of_range& e) {
                    isBlockInFrontAir = z == 0;
                }

                if (currentBlockNotAir && isBlockInFrontAir) {
                    // drawing front face
                    indices.insert(indices.end(), {
                            vertexProcessed, vertexProcessed + 1, vertexProcessed + 2,
                            vertexProcessed + 3, vertexProcessed + 1, vertexProcessed + 2
                    });

                    vertices.insert(vertices.end(), {
                            blockX,           blockY,            blockZ,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                            blockX + 1.0f,    blockY,            blockZ,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                            blockX,           blockY + 1.0f,     blockZ,    0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                            blockX + 1.0f,    blockY + 1.0f,     blockZ,    1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                    });
                    vertexProcessed += 4;
                }

                // drawing back face
                bool isBlockInBackAir;
                try {
                    isBlockInBackAir = blocks.at(x).at(y).at(z + 1) == 0;
                } catch (const std::out_of_range& e) {
                    isBlockInBackAir = z == chunkSizeZ - 1;
                }

                if (currentBlockNotAir && isBlockInBackAir) {
                    // drawing back face
                    indices.insert(indices.end(), {
                            vertexProcessed, vertexProcessed + 1, vertexProcessed + 2,
                            vertexProcessed + 3, vertexProcessed + 1, vertexProcessed + 2
                    });

                    vertices.insert(vertices.end(), {
                            blockX,           blockY,            blockZ + 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                            blockX + 1.0f,    blockY,            blockZ + 1.0f,    1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                            blockX,           blockY + 1.0f,     blockZ + 1.0f,    0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                            blockX + 1.0f,    blockY + 1.0f,     blockZ + 1.0f,    1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                    });
                    vertexProcessed += 4;
                }

                // drawing left face
                bool isBlockOnLeftAir;
                try {
                    isBlockOnLeftAir = blocks.at(x - 1).at(y).at(z) == 0;
                } catch (const std::out_of_range& e) {
                    isBlockOnLeftAir = x == 0;
                }

                if (currentBlockNotAir && isBlockOnLeftAir) {
                    // drawing left face
                    indices.insert(indices.end(), {
                            vertexProcessed, vertexProcessed + 1, vertexProcessed + 2,
                            vertexProcessed + 3, vertexProcessed + 1, vertexProcessed + 2
                    });

                    vertices.insert(vertices.end(), {
                            blockX,     blockY,            blockZ,            0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
                            blockX,     blockY + 1.0f,     blockZ,            1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
                            blockX,     blockY,            blockZ + 1.0f,     0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
                            blockX,     blockY + 1.0f,     blockZ + 1.0f,     0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
                    });
                    vertexProcessed += 4;
                }

                // drawing right face
                bool isBlockOnRightAir;
                try {
                    isBlockOnRightAir = blocks.at(x + 1).at(y).at(z) == 0;
                } catch (const std::out_of_range& e) {
                    isBlockOnRightAir = x == chunkSizeX - 1;
                }

                if (currentBlockNotAir && isBlockOnRightAir) {
                    // drawing right face
                    indices.insert(indices.end(), {
                            vertexProcessed, vertexProcessed + 1, vertexProcessed + 2,
                            vertexProcessed + 3, vertexProcessed + 1, vertexProcessed + 2
                    });

                    vertices.insert(vertices.end(), {
                            blockX + 1.0f,     blockY,            blockZ,            0.0f, 1.0f,    1.0f, 0.0f, 0.0f,
                            blockX + 1.0f,     blockY + 1.0f,     blockZ,            1.0f, 1.0f,    1.0f, 0.0f, 0.0f,
                            blockX + 1.0f,     blockY,            blockZ + 1.0f,     0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
                            blockX + 1.0f,     blockY + 1.0f,     blockZ + 1.0f,     0.0f, 1.0f,    1.0f, 0.0f, 0.0f,
                    });
                    vertexProcessed += 4;
                }

                // drawing bottom face
                bool isBlockBelowAir;
                try {
                    isBlockBelowAir = blocks.at(x).at(y - 1).at(z) == 0;
                } catch (const std::out_of_range& e) {
                    isBlockBelowAir = y == 0;
                }

                if (currentBlockNotAir && isBlockBelowAir) {
                    indices.insert(indices.end(), {
                            vertexProcessed, vertexProcessed + 1, vertexProcessed + 2,
                            vertexProcessed + 3, vertexProcessed + 1, vertexProcessed + 2
                    });

                    vertices.insert(vertices.end(), {
                            blockX,           blockY,     blockZ,           0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
                            blockX,           blockY,     blockZ + 1.0f,    1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
                            blockX + 1.0f,    blockY,     blockZ,           0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
                            blockX + 1.0f,    blockY,     blockZ + 1.0f,    1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
                    });
                    vertexProcessed += 4;
                }
            }
        }
    }

    if (vertices.empty() || indices.empty()) {
        return;
    }

    Mesh *cube = new Mesh();
    cube->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
    meshList.push_back(cube);

    //printf("chunk drawn -----\n");
}

int main() {
    const unsigned int width = 1920, height = 1080;

    auto* mainWindow = new Window(width, height);
    mainWindow->Init();

    auto* textSystem = new Text(width, height);

    int chunksCountX = 4;
    int chunksCountZ = 4;
    for (int x = 0; x < chunksCountX; x++) {
        for (int z = 0; z < chunksCountZ; z++) {
            DrawChunk(x, z);
        }
    }

    CreateShader();
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 20.0f, 0.4f);

    std::string cobblestoneTexturePath = "textures/cobblestone.png";
    cobblestoneTexture = Texture(cobblestoneTexturePath.c_str());
    cobblestoneTexture.LoadTexture();

    std::string stoneTexturePath = "textures/stone.jpg";
    stoneTexture = Texture(stoneTexturePath.c_str());
    stoneTexture.LoadTexture();

    std::string dirtTexturePath = "textures/dirt.jpg";
    dirtTexture = Texture(dirtTexturePath.c_str());
    dirtTexture.LoadTexture();

    /*
    std::string plainTexturePath = "textures/plain.png";
    plainTexture = Texture(plainTexturePath.c_str());
    plainTexture.LoadTextureAlpha();
    */

    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    xwing = Model();
    xwing.LoadModel("models/x-wing.obj");

    mainLight = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.6f,glm::vec3(9.0f, -1.0f, -9.0f));

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

        shaderList[0]->SetDirectionalLight(&mainLight);

        glm::mat4 model(1.0f);
        glUniformMatrix4fv(uniformProjectionId, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

        // start of objects rendering

        for (auto & i : meshList) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -5.0f, -5.0f));
            glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
            stoneTexture.UseTexture();
            dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
            i->RenderMesh();
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        xwing.RenderModel();

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
