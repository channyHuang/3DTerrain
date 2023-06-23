#include "osgManager.h"

osg::ref_ptr<osg::Geometry> createAxis() {
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());

    osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 10.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 10.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 10.0));
    geometry->setVertexArray (vertices.get());

    osg::ref_ptr<osg::Vec4Array> colors (new osg::Vec4Array());
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    geometry->setColorArray (colors.get(), osg::Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6));
    return geometry.release();
}

osg::Texture2D *createTexture(const std::string &fileName)
{
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage(osgDB::readRefImageFile(fileName));
    texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
    texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    texture->setMaxAnisotropy(16.0f);
    return texture.release();
}

OsgManager* OsgManager::instance = nullptr;

OsgManager::OsgManager() {

}

OsgManager::~OsgManager() {
    m_pSceneRoot->removeChildren(0, m_pSceneRoot->getNumChildren());
}

void OsgManager::reset() {
    m_pSceneRoot->removeChildren(0, m_pSceneRoot->getNumChildren());
}

void OsgManager::setViewer(osg::ref_ptr<osgViewer::Viewer> pViewer) {
    m_pViewer = pViewer;
    m_pSceneRoot = new osg::Switch;
    m_pSceneRoot->addChild(createAxis());
    m_pViewer->setSceneData(m_pSceneRoot);
}

void OsgManager::switchScene() {
    if (m_nSceneIndex >= m_pSceneRoot->getNumChildren()) {
        m_pSceneRoot->setAllChildrenOn();
        m_nSceneIndex = 0;
    } else {
        m_pSceneRoot->setSingleChildOn(m_nSceneIndex);
        m_nSceneIndex++;
    }
}

void OsgManager::sltGenerateMeshSuc(TerrainStruct::Arrays surface, Vector3i pos) {
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    osg::Vec3Array *vvertex = new osg::Vec3Array;
    osg::Vec3Array *vnormal = new osg::Vec3Array;
    for (int i = 0; i < surface.positions.size(); ++i) {
        vvertex->push_back(osg::Vec3(surface.positions[i].x, surface.positions[i].y, surface.positions[i].z));
        vnormal->push_back(osg::Vec3(surface.normals[i].x, surface.normals[i].y, surface.normals[i].z));
    }
    geom->setVertexArray(vvertex);
    geom->setNormalArray(vnormal);
    std::vector<unsigned int> indices;
    for (int i = 0; i < surface.indices.size(); ++i) {
        indices.push_back(surface.indices[i]);
    }
    geom->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, indices.size(), indices.data()));

    osg::ref_ptr<osg::StateSet> ss = geom->getOrCreateStateSet();
    ss->setTextureAttributeAndModes(0, createTexture("resource/barkD_texture.bmp"));

    m_pSceneRoot->addChild(geom);
}

void OsgManager::testShow() {
    osg::Geometry* polyGeom = new osg::Geometry();

    // note, anticlockwise ordering.
    osg::Vec3 myCoords[] =
    {
        osg::Vec3(-1.22908f,0.0f,1.0f),
        osg::Vec3(-1.22908f,0.0f,-1.0f),
        osg::Vec3(1.22908f,0.0f,-1.0f),
        osg::Vec3(1.22908f,0.0f,1.0f)
    };

    int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

    // pass the created vertex array to the points geometry object.
    polyGeom->setVertexArray(new osg::Vec3Array(numCoords,myCoords));

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    polyGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


    // Set the normal in the same way as the color.
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    polyGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::Vec2 myTexCoords[] =
    {
        osg::Vec2(0,1),
        osg::Vec2(0,0),
        osg::Vec2(1,0),
        osg::Vec2(1,1)
    };

    int numTexCoords = sizeof(myTexCoords)/sizeof(osg::Vec2);

    // pass the created tex coord array to the points geometry object,
    // and use it to set texture unit 0.
    polyGeom->setTexCoordArray(0,new osg::Vec2Array(numTexCoords,myTexCoords));

    // we'll use indices and DrawElements to define the primitive this time.
    unsigned short myIndices[] =
    {
        0,
        1,
        3,
        2
    };

    int numIndices = sizeof(myIndices)/sizeof(unsigned short);

    // There are three variants of the DrawElements osg::Primitive, UByteDrawElements which
    // contains unsigned char indices, UShortDrawElements which contains unsigned short indices,
    // and UIntDrawElements which contains ... unsigned int indices.
    // The first parameter to DrawElements is
    polyGeom->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP,numIndices,myIndices));

    //m_pSceneRoot->addChild(polyGeom);
    m_pViewer->setSceneData(polyGeom);
}
