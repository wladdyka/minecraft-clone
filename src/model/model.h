//
// Created by wladdyka on 7/31/2023.
//

#ifndef OPENGLCOURSE_MODEL_H
#define OPENGLCOURSE_MODEL_H

#include <vector>
#include <string>

#include <assimp\importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "../mesh/mesh.h"
#include "../texture/texture.h"

class Model {
public:
    Model();
    ~Model();

    void LoadModel(const std::string &fileName);
    void RenderModel();
    void ClearModel();
private:
    void LoadNode(aiNode *node, const aiScene *scene);
    void LoadMesh(aiMesh *mesh, const aiScene *scene);
    void LoadMaterials(const aiScene *scene);

    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToText;
};


#endif //OPENGLCOURSE_MODEL_H
