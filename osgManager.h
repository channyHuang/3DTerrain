#ifndef OSGMANAGER_H
#define OSGMANAGER_H

#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/ref_ptr>

#include "controler/terrainCommonStruct.h"

class OsgManager {
public:
    static OsgManager* getInstance() {
        if (instance == nullptr) {
            instance = new OsgManager();
        }
        return instance;
    }

    void setViewer(osg::ref_ptr<osgViewer::Viewer> pViewer);
    void switchScene();
    void reset();
    void testShow();

    void sltGenerateMeshSuc(TerrainStruct::Arrays, Vector3i);
    void sltShowMeshWithShader(TerrainStruct::Arrays, Vector3i);

private:
    OsgManager();
    ~OsgManager();

private:
    static OsgManager* instance;

    int m_nSceneIndex = 0;
    osg::ref_ptr<osg::Switch> m_pSceneRoot;
    osg::ref_ptr<osgViewer::Viewer> m_pViewer;
};

#endif
