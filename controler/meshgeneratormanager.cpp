#include "meshgeneratormanager.h"

#include "generator/voxel_mesher_surfacenet.h"

MeshGeneratorManager::MeshGeneratorManager()
{
    pool.setMaxThreadCount(1);

    sigMeshGenSuc.connect(this, &MeshGeneratorManager::sltMeshGenSuc);
}

MeshGeneratorManager::~MeshGeneratorManager() {

}

void MeshGeneratorManager::sltMeshGenSuc(TerrainStruct::OutputBlock output) {
    std::unique_lock<std::mutex> lock(mutex);
    blocks.push_back(output);
    lock.unlock();
}

void MeshGeneratorManager::push(TerrainStruct::Input input) {
    for (int i = 0; i < input.blocks.size(); ++i) {
        pool.enqueue([this, input, i]{
            TerrainStruct::OutputBlock output;

            const TerrainStruct::InputBlock &block = input.blocks[i];
            TerrainStruct::OutputBlock &outputData = output;
            TerrainStruct::MeshInput in = {*block.voxels, 0, block.position};
            output.position = block.position;
            VoxelMesherSurfaceNets* mesher = new VoxelMesherSurfaceNets;
            mesher->build(outputData.smooth_surfaces, in);

            this->sigMeshGenSuc(output);
        });
    }
}

void MeshGeneratorManager::pop(TerrainStruct::Output &output) {
    output.blocks.swap(blocks);
    blocks.clear();
}
