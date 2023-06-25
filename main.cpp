//#include <QCoreApplication>

#include <iostream>
#include <ctime>
#include <chrono>

#define GL_SILENCE_DEPRECATION

#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

//#include "nativefiledialog/include/nfd.h"
#include "generator/terraingenerator_roblox.h"

#include "OsgImGuiHandler.h"

#include "osgManager.h"

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    return a.exec();
//}

class ImGuiInitOperation : public osg::Operation
{
public:
    ImGuiInitOperation()
        : osg::Operation("ImGuiInitOperation", false)
    {
    }

    void operator()(osg::Object* object) override
    {
        osg::GraphicsContext* context = dynamic_cast<osg::GraphicsContext*>(object);
        if (!context)
            return;

        if (!ImGui_ImplOpenGL3_Init("#version 410"))
        {
            std::cout << "ImGui_ImplOpenGL3_Init() failed\n";
        }
    }
};

class ImGuiDemo : public OsgImGuiHandler
{
public:
    ImGuiDemo() {
        memset(m_cFileName, 0, 128);
        memcpy(m_cFileName, "test.obj", 8);

        TerrainManager::getInstance()->_notification(Notification_Enter);

        m_lastTime = std::chrono::steady_clock::now();
    }


protected:

    void drawUi() override
    {
        // ImGui code goes here...
        //ImGui::ShowDemoWindow();
        ImGui::Begin("Hello, world!");
        ImGui::InputText("file name", m_cFileName, 128);
        ImGui::SameLine();
        if (ImGui::Button("Open File")) {
//            nfdchar_t *filename = nullptr;
//            nfdresult_t result = NFD_OpenDialog("fbx,glb,gltf,obj,stl", nullptr, &filename);
//            if (result != NFD_OKAY)
//            {
//                free(filename);
//                return;
//            }
        }

        ImGui::Separator();
        ImGui::Text("size:");
        ImGui::InputInt("size x", &vSize[0]);
        ImGui::InputInt("size y", &vSize[1]);
        ImGui::InputInt("size z", &vSize[2]);

        ImGui::Separator();
        ImGui::Text("center:");
        ImGui::InputInt("center x", &vCenter[0]);
        ImGui::InputInt("center y", &vCenter[1]);
        ImGui::InputInt("center z", &vCenter[2]);

        ImGui::Separator();
        ImGui::Checkbox("use biomes", &m_stBiomeParams.use_biomes);
        if (m_stBiomeParams.use_biomes) {
            for (int i = 0; i < m_vBiomesStr.size(); ++i) {
                ImGui::Checkbox(m_vBiomesStr[i].c_str(), &m_vBiomesCheck[i]);

                if (m_vBiomesCheck[i]) {
                    m_stBiomeParams.biomes_be_checked &= (1 << i);
                } else {
                    m_stBiomeParams.biomes_be_checked ^= (1 << i);
                }
            }
        }

        ImGui::Separator();
        if (ImGui::Button("Generate")) {
            VoxelBrush *brush = new VoxelBrush(TerrainManager::getInstance(), VoxelMap::getInstance());
            TerrainGenerator_Roblox::getInstance()->setRange(Vector3(vCenter[0], vCenter[1], vCenter[2]), Vector3(vSize[0], vSize[1], vSize[2]));
            TerrainGenerator_Roblox::getInstance()->generateTerrainByBiomes(brush, m_stBiomeParams);
        }

        if (ImGui::Button("Reset")) {
            OsgManager::getInstance()->reset();
        }

        if (ImGui::Button("Test")) {
            OsgManager::getInstance()->testShow();
        }
        ImGui::End();

        auto curTime = std::chrono::steady_clock::now();
        auto count = std::chrono::duration_cast<std::chrono::seconds>(curTime - m_lastTime).count();
        if (count > 1) {
            TerrainManager::getInstance()->_notification(Notification_Process);
            m_lastTime = curTime;
        }
    }

public:
    char m_cFileName[128];
    int vSize[3] = {10, 10, 10};
    int vCenter[3] = {0};
    TerrainGenerator_Roblox::BiomesParam m_stBiomeParams;
    std::chrono::steady_clock::time_point m_lastTime;
    std::vector<std::string> m_vBiomesStr = {    "Water",
                                                 "Marsh",
                                                 "Plains",
                                                 "Hills",
                                                 "Dunes",
                                                 "Canyons",
                                                 "Mountains",
                                                 "Lavaflow",
                                                 "Arctic"
                                            };
    bool m_vBiomesCheck[9] = {false, false, false, false, false, false, false, false, false};
};

int main() {
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    viewer->apply(new osgViewer::SingleWindow(100, 100, 640, 480));
    viewer->setRealizeOperation(new ImGuiInitOperation);
    viewer->addEventHandler(new ImGuiDemo);

    viewer->getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    viewer->getCamera()->getGraphicsContext()->getState()->resetVertexAttributeAlias(true);

    OsgManager::getInstance()->setViewer(viewer);
    viewer->getCamera()->getGraphicsContext()->getState()->setCheckForGLErrors(osg::State::NEVER_CHECK_GL_ERRORS);

    return viewer->run();
}
