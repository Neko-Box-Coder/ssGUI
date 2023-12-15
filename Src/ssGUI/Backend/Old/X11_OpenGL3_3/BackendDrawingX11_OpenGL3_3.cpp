#include "ssGUI/Backend/X11_OpenGL3_3/BackendDrawingX11_OpenGL3_3.hpp"

#include "ssGUI/HelperClasses/ImageUtil.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"

#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/Backend/X11_OpenGL3_3/BackendMainWindowX11_OpenGL3_3.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//DEBUG
    #include <fstream>

#define GL_CHECK_ERROR(x)\
{\
    GLenum err;\
    x;\
    if((err = glGetError()) != GL_NO_ERROR)\
    {\
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed: "<<err);\
    }\
}

namespace ssGUI
{

namespace Backend
{
    BackendDrawingX11_OpenGL3_3::BackendDrawingX11_OpenGL3_3(BackendDrawingX11_OpenGL3_3 const& other)
    {
        //TODO
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Not implemented");
        ssLOG_EXIT_PROGRAM();
    }
    
    //TODO: Move to common method
    ssGUI::Backend::BackendMainWindowInterface* BackendDrawingX11_OpenGL3_3::GetMainWindow()
    {
        //Check correct backend index
        if(BackendIndex >= ssGUI::Backend::BackendManager::GetMainWindowCount())
            BackendIndex = 0;
        
        if(ssGUI::Backend::BackendManager::GetDrawingInterface(BackendIndex) != 
            static_cast<ssGUI::Backend::BackendDrawingInterface*>(this))
        {
            BackendIndex = ssGUI::Backend::BackendManager::GetDrawingInterfaceIndex(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
        }

        return ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex);
    }
    
    void BackendDrawingX11_OpenGL3_3::InitializeOpenGLCommonIfNeeded()
    {
        if(OpenGLCommon == nullptr)
        {
            ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
            OpenGLCommon = new ssGUI::Backend::OpenGL3_3_Common(mainWindow);
        }
    }

    BackendDrawingX11_OpenGL3_3::BackendDrawingX11_OpenGL3_3() :    BackendIndex(0),
                                                                    LastMainWindowSize(-1, -1),
                                                                    CharTextures(),
                                                                    ImageTextures(),
                                                                    OpenGLCommon(nullptr)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));   
    }

    BackendDrawingX11_OpenGL3_3::~BackendDrawingX11_OpenGL3_3()
    {
        //Notify each image
        for(auto it = ImageTextures.begin(); it != ImageTextures.end(); it++)
            it->first->Internal_RemoveBackendDrawingRecord(this);
    
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
        
        if(OpenGLCommon != nullptr)
            delete OpenGLCommon;
    }
    
    void BackendDrawingX11_OpenGL3_3::SaveDrawingState()
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->SaveDrawingState();
    }

    void BackendDrawingX11_OpenGL3_3::RestoreDrawingState()
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->RestoreDrawingState();
    }

    bool BackendDrawingX11_OpenGL3_3::DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities)
    {
        InitializeOpenGLCommonIfNeeded();
        return OpenGLCommon->DrawEntities(entities);
    }
    
    void BackendDrawingX11_OpenGL3_3::DrawToBackBuffer()
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->DrawToBackBuffer();
    }

    void BackendDrawingX11_OpenGL3_3::Render(glm::u8vec3 clearColor)
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        InitializeOpenGLCommonIfNeeded();
        DrawToBackBuffer();
        
        if(mainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }

        auto* rawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(0)->GetRawHandle());
                
        glXSwapBuffers(rawHandle->WindowDisplay, rawHandle->WindowId);
        ClearBackBuffer(clearColor);
    }

    void BackendDrawingX11_OpenGL3_3::ClearBackBuffer(glm::u8vec3 clearColor)
    {
        InitializeOpenGLCommonIfNeeded();
        GL_CHECK_ERROR( glClearColor(   static_cast<float>(clearColor.r) / 255.f, 
                                        static_cast<float>(clearColor.g) / 255.f, 
                                        static_cast<float>(clearColor.b) / 255.f, 
                                        255.f); );

        GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );
    }
    
    void BackendDrawingX11_OpenGL3_3::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        InitializeOpenGLCommonIfNeeded();
        if(!OpenGLCommon->AddImageCache(backendImage))
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to add image cache");
            return;
        }
        
        backendImage->Internal_AddBackendDrawingRecord(this);
    }
    
    void BackendDrawingX11_OpenGL3_3::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->RemoveImageCache(backendImage);
        backendImage->Internal_RemoveBackendDrawingRecord(this);
    }
    
    void* BackendDrawingX11_OpenGL3_3::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        return nullptr;
    }
}

}