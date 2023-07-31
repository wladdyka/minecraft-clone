//
// Created by wladdyka on 7/31/2023.
//

#include "model.h"

void Model::LoadModal(const std::string &fileName) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene) {
        printf("Model (%s) failed to load: %s", fileName.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
}

void Model::LoadMaterials(const aiScene *scene) {

}

void Model::LoadNode(aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
}
