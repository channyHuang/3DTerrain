#ifndef MESHGENERATORMANAGER_H
#define MESHGENERATORMANAGER_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>

#include "commonFunc/threadPool.h"
#include "commonFunc/signalSlots.h"

#include "terrainCommonStruct.h"

class MeshGeneratorManager
{
public:
    explicit MeshGeneratorManager();
    ~MeshGeneratorManager();

    void push(TerrainStruct::Input input);
    void pop(TerrainStruct::Output &output);

    int get_minimum_padding() const { return _minimum_padding; }
    int get_maximum_padding() const { return _maximum_padding; }

// signals:
    SignalSlots::Signal<void(TerrainStruct::OutputBlock)> sigMeshGenSuc;

// public slots:
    void sltMeshGenSuc(TerrainStruct::OutputBlock output);

private:
    std::vector<TerrainStruct::OutputBlock> blocks;
    int index = 0;
    const int maxIndex = 10000;
    int _minimum_padding = 2;
    int _maximum_padding = 2;
    std::mutex mutex;

    ThreadPool pool;
};

#endif // MESHGENERATORMANAGER_H
