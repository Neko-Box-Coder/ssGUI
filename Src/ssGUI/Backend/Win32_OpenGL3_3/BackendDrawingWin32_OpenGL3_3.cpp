#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"

#include "ssGUI/HelperClasses/ImageUtil.hpp"

#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

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
    ssGUI::Backend::BackendMainWindowInterface* BackendDrawingWin32_OpenGL3_3::GetMainWindow()
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

    void BackendDrawingWin32_OpenGL3_3::InitializeOpenGLCommonIfNeeded()
    {
        if(OpenGLCommon == nullptr)
        {
            ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
            OpenGLCommon = new ssGUI::Backend::OpenGL3_3_Common(mainWindow);
        }
    }

    //void BackendDrawingWin32_OpenGL3_3::UpdateViewPortAndModelViewIfNeeded()
    //{
    //    ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();

    //    if(mainWindow == nullptr)
    //    {
    //        ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
    //        return;
    //    }

    //    if(mainWindow->GetRenderSize() == LastMainWindowSize)
    //        return;

    //    LastMainWindowSize = mainWindow->GetRenderSize();
        
    //    mainWindow->SetGLContext();
    //    glm::ivec2 renderSize = mainWindow->GetRenderSize();
    //    GL_CHECK_ERROR( glViewport(0, 0, renderSize.x, renderSize.y); );
    //    GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW););
    //    GL_CHECK_ERROR( glLoadIdentity(); );
    //    glm::mat4x4 orthoMat = glm::ortho<float>(0.f, renderSize.x, renderSize.y, 0.f, 0, 10);
    //    GL_CHECK_ERROR( glMultMatrixf(glm::value_ptr(orthoMat)); );
    //}

    BackendDrawingWin32_OpenGL3_3::BackendDrawingWin32_OpenGL3_3(BackendDrawingWin32_OpenGL3_3 const& other)
    {
        //TODO
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Not implemented");
        ssLOG_EXIT_PROGRAM();
    }
    
    BackendDrawingWin32_OpenGL3_3::BackendDrawingWin32_OpenGL3_3() :    BackendIndex(0),
                                                                        LastMainWindowSize(-1, -1),
                                                                        CharTextures(),
                                                                        ImageTextures(),
                                                                        OpenGLCommon(nullptr)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    BackendDrawingWin32_OpenGL3_3::~BackendDrawingWin32_OpenGL3_3()
    {
        //Notify each image
        for(auto it = ImageTextures.begin(); it != ImageTextures.end(); it++)
            it->first->Internal_RemoveBackendDrawingRecord(this);
        
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    
        if(OpenGLCommon != nullptr)
            delete OpenGLCommon;
    }

    void BackendDrawingWin32_OpenGL3_3::SaveState()
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->SaveState();
    }

    void BackendDrawingWin32_OpenGL3_3::RestoreState()
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->RestoreState();
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities)
    {
        InitializeOpenGLCommonIfNeeded();

        return OpenGLCommon->DrawEntities(entities);
    }

    void BackendDrawingWin32_OpenGL3_3::DrawToBackBuffer()
    {
        OpenGLCommon->DrawToBackBuffer();
    }

    void BackendDrawingWin32_OpenGL3_3::Render(glm::u8vec3 clearColor)
    {   
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        InitializeOpenGLCommonIfNeeded();
        //ClearBackBuffer(clearColor);
        DrawToBackBuffer();
        
        if(mainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }
        
        //Get the device context for the window        
        HDC hDC = GetDC(static_cast<ssGUI::Backend::Win32_OpenGL_Handles*>(mainWindow->GetRawHandle())->WindowHandle);
        if(hDC == NULL)
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to GetDC");
            ssLOG_EXIT_PROGRAM();
        }
        SwapBuffers(hDC);
    
        GL_CHECK_ERROR( glClearColor(   static_cast<float>(clearColor.r) / 255.f, 
                                        static_cast<float>(clearColor.g) / 255.f, 
                                        static_cast<float>(clearColor.b) / 255.f, 
                                        255.f); );

        GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );
    }

    void BackendDrawingWin32_OpenGL3_3::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        //Seems like this is causing memory leak
        //GL_CHECK_ERROR( glClearColor(1, 1, 1, 1); );
        //GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );
    }

    void BackendDrawingWin32_OpenGL3_3::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        InitializeOpenGLCommonIfNeeded();
        if(!OpenGLCommon->AddImageCache(backendImage))
        {
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Failed to add image cache");
            return;
        }
        
        backendImage->Internal_AddBackendDrawingRecord(this);
    }
    
    void BackendDrawingWin32_OpenGL3_3::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        InitializeOpenGLCommonIfNeeded();
        OpenGLCommon->RemoveImageCache(backendImage);
        backendImage->Internal_RemoveBackendDrawingRecord(this);
    }
    
    void* BackendDrawingWin32_OpenGL3_3::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        return nullptr;
    }
}

}