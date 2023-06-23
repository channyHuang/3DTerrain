#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

#include <unordered_set>

#include "commonMath/boxi.h"

#include "voxels/voxelmap.h"
#include "meshgeneratormanager.h"
#include "osgManager.h"

class TerrainManager
{
public:

    ~TerrainManager();

    static TerrainManager *getInstance() {
        if (instance == nullptr) {
            instance = new TerrainManager;
        }
        return instance;
    }

    void make_block_dirty(const Vector3i& bpos);
    void make_voxel_dirty(const Vector3i& pos);
    void make_area_dirty(const Boxi& box);

//signals:
    //void generateMeshSuc(Arrays surface, Vector3i pos);

    SignalSlots::Signal<void(TerrainStruct::Arrays, Vector3i)> sigGenerateMeshSuc;

//public slots:
    void _notification(int p_what);

private:
    TerrainManager();

    void _process();
    void start_updater();
    void stop_updater();

    static TerrainManager* instance;
    VoxelMap *_map = nullptr;
    MeshGeneratorManager *_block_updater = nullptr;
    std::unordered_set<Vector3i, Vector3iHash> _blocks_pending_update;
    std::vector<TerrainStruct::OutputBlock> _blocks_pending_main_thread_update;
    Vector3 viewer_pos;
    Vector3 viewer_direction;
};

#endif
