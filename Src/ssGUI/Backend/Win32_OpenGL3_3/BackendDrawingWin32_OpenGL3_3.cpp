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

    void BackendDrawingWin32_OpenGL3_3::UpdateViewPortAndModelViewIfNeeded()
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();

        if(mainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }

        if(mainWindow->GetRenderSize() == LastMainWindowSize)
            return;

        LastMainWindowSize = mainWindow->GetRenderSize();
        
        mainWindow->SetGLContext();
        glm::ivec2 renderSize = mainWindow->GetRenderSize();
        GL_CHECK_ERROR( glViewport(0, 0, renderSize.x, renderSize.y); );
        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW););
        GL_CHECK_ERROR( glLoadIdentity(); );
        glm::mat4x4 orthoMat = glm::ortho<float>(0.f, renderSize.x, renderSize.y, 0.f, 0, 10);
        GL_CHECK_ERROR( glMultMatrixf(glm::value_ptr(orthoMat)); );
    }

    BackendDrawingWin32_OpenGL3_3::BackendDrawingWin32_OpenGL3_3(BackendDrawingWin32_OpenGL3_3 const& other)
    {
        //TODO
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Not implemented");
        ssLOG_EXIT_PROGRAM();
    }
    
    BackendDrawingWin32_OpenGL3_3::BackendDrawingWin32_OpenGL3_3() :    BackendIndex(0),
                                                                        LastMainWindowSize(-1, -1),
                                                                        CharTextures(),
                                                                        ImageTextures()
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    BackendDrawingWin32_OpenGL3_3::~BackendDrawingWin32_OpenGL3_3()
    {
        //Notify each image
        for(auto it = ImageTextures.begin(); it != ImageTextures.end(); it++)
            it->first->Internal_RemoveBackendDrawingRecord(this);
        
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    void BackendDrawingWin32_OpenGL3_3::SaveState()
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }

        if(!mainWindow->SetGLContext())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set GL state");
            return;
        }

        GL_CHECK_ERROR( glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS); );
        GL_CHECK_ERROR( glPushAttrib(GL_ALL_ATTRIB_BITS); );
        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW); );
        GL_CHECK_ERROR( glPushMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_PROJECTION); );
        GL_CHECK_ERROR( glPushMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_TEXTURE); );
        GL_CHECK_ERROR( glPushMatrix(); );
    }

    void BackendDrawingWin32_OpenGL3_3::RestoreState()
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to get MainWinodw");
            return;
        }

        if(!mainWindow->SetGLContext())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to set GL state");
            return;
        }

        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW); );
        GL_CHECK_ERROR( glPopMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_PROJECTION); );
        GL_CHECK_ERROR( glPopMatrix(); );
        GL_CHECK_ERROR( glMatrixMode(GL_TEXTURE); );
        GL_CHECK_ERROR( glPopMatrix(); );
        GL_CHECK_ERROR( glPopClientAttrib(); );
        GL_CHECK_ERROR( glPopAttrib(); );
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawEntities(const std::vector<ssGUI::DrawingEntity>& entities)
    {
        //Check if the main window is already closed
        if(GetMainWindow()->IsClosed())
            return false;

        UpdateViewPortAndModelViewIfNeeded();

        //Start drawing
        for(int i = 0; i < entities.size(); i++)
        {
            bool result = true;
            //Drawing text
            if(entities[i].BackendFont != nullptr)
            {
                result = DrawShape( entities[i].Vertices, 
                                    entities[i].TexCoords, 
                                    entities[i].Colors, 
                                    entities[i].Character, 
                                    *entities[i].BackendFont, 
                                    entities[i].CharacterSize);
            }
            //Drawing image
            else if(entities[i].BackendImage != nullptr)
            {
                result = DrawShape( entities[i].Vertices, 
                                    entities[i].TexCoords, 
                                    entities[i].Colors,
                                    *entities[i].BackendImage);
            }
            //Drawing shapes
            else 
            {
                result = DrawShape( entities[i].Vertices, 
                                    entities[i].Colors);
            }

            if(!result)
                return false;
        }

        return true;
    }

    void BackendDrawingWin32_OpenGL3_3::Render(glm::u8vec3 clearColor)
    {   
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
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

        GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );

        GL_CHECK_ERROR( glClearColor(   static_cast<float>(clearColor.r) / 255.f, 
                                        static_cast<float>(clearColor.g) / 255.f, 
                                        static_cast<float>(clearColor.b) / 255.f, 
                                        255.f); );
    }

    void BackendDrawingWin32_OpenGL3_3::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        //Seems like this is causing memory leak
        //GL_CHECK_ERROR( glClearColor(1, 1, 1, 1); );
        //GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );
    }

    void BackendDrawingWin32_OpenGL3_3::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        if(!backendImage->IsValid())
            return;
    
        if(ImageTextures.find(backendImage) != ImageTextures.end())
            return;
            
        {
            //Get image
            ssGUI::ImageFormat format;
            const void* rawPtr = backendImage->GetPixelPtr(format);

            //Convert it to rgba32
            uint8_t* rgba32Img = new uint8_t[backendImage->GetSize().x * backendImage->GetSize().y * 4];
            if(!ssGUI::ImageUtil::ConvertToRGBA32(static_cast<void*>(rgba32Img), rawPtr, format, backendImage->GetSize()))
            {
                delete[] rgba32Img;
                return;
            }

            GLuint textureId = 0;

            GL_CHECK_ERROR( glGenTextures(1, &textureId); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );
            
            //Save it to gpu
            GL_CHECK_ERROR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backendImage->GetSize().x, backendImage->GetSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                            rgba32Img); );

            ImageTextures[backendImage] = textureId;

            delete[] rgba32Img;
            
            backendImage->Internal_AddBackendDrawingRecord(this);
        }
        
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
    }
    
    void BackendDrawingWin32_OpenGL3_3::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        if(ImageTextures.find(backendImage) == ImageTextures.end())
            return;

        ImageTextures.erase(backendImage);
        backendImage->Internal_RemoveBackendDrawingRecord(this);
    }
    
    void* BackendDrawingWin32_OpenGL3_3::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        if(ImageTextures.find(backendImage) == ImageTextures.end())
            return nullptr;
        
        return &ImageTextures[backendImage];
    }
    
    bool BackendDrawingWin32_OpenGL3_3::DrawShape(  const std::vector<glm::vec2>& vertices, 
                                                    const std::vector<glm::vec2>& texCoords,
                                                    const std::vector<glm::u8vec4>& colors,
                                                    const uint32_t character,
                                                    const ssGUI::Backend::BackendFontInterface& font,
                                                    int characterSize)
    {
        if(!font.IsValid())
            return false;
    
        auto& rawFont = const_cast<ssGUI::Backend::BackendFontInterface&>(font);

        ssGUI::ImageData charImgData;
        if(!rawFont.GetCharacterImage(character, characterSize, charImgData))
            return false;

        GetMainWindow()->SetGLContext();
    
        glm::ivec2 imgSize = charImgData.GetSize();
        CharTextureIdentifier curIdentifier = CharTextureIdentifier(&rawFont, characterSize, character);

        if(CharTextures.find(curIdentifier) == CharTextures.end())
        {
            GLuint textureId = 0;

            GL_CHECK_ERROR( glGenTextures(1, &textureId); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );

            //Convert it to rgba32
            uint8_t* rgba32Img = new uint8_t[charImgData.GetSize().x * charImgData.GetSize().y * 4];
            ssGUI::ImageFormat format;
            void* rawPixel = charImgData.GetPixelPtr(format);
            
            if(!ssGUI::ImageUtil::ConvertToRGBA32(static_cast<void*>(rgba32Img), rawPixel, format, imgSize))
            {
                delete[] rgba32Img;
                GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
                return false;
            }
            
            GL_CHECK_ERROR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSize.x, imgSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                            rgba32Img); );

            CharTextures[curIdentifier] = textureId;

            delete[] rgba32Img;
        }
        else
        {
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, CharTextures[curIdentifier]); );
        }
         
        GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D); );
        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0; i < vertices.size(); i++)
        {
            glm::vec2 texCoord = texCoords[i];

            texCoord.x /= imgSize.x;
            texCoord.y /= imgSize.y;
            
            glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            glTexCoord2f(texCoord.x, texCoord.y);
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            glVertex3f(round(vertices[i].x), round(vertices[i].y), 0);
        }

        GL_CHECK_ERROR( glEnd(); );
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
        GL_CHECK_ERROR( glFlush(); );

        return true;
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawShape(  const std::vector<glm::vec2>& vertices, 
                                                    const std::vector<glm::vec2>& texCoords,
                                                    const std::vector<glm::u8vec4>& colors,
                                                    const ssGUI::Backend::BackendImageInterface& image)
    {
        GetMainWindow()->SetGLContext();

        if(!image.IsValid())
            return false;

        //GL_CHECK_ERROR( glUseProgram(ProgramId); );
        
        AddImageCache(const_cast<ssGUI::Backend::BackendImageInterface*>(&image));
        
        glm::ivec2 imgSize = image.GetSize();
        
        //TODO: Remove const cast
        //If unable to add to cache, return
        if(ImageTextures.find(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)) == ImageTextures.end())
            return false;
        else
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, ImageTextures[const_cast<ssGUI::Backend::BackendImageInterface*>(&image)]); );

        //render it
        GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D); );
        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0; i < vertices.size(); i++)
        {
            glm::vec2 texCoord = texCoords[i];

            texCoord.x /= imgSize.x;
            texCoord.y /= imgSize.y;
            glTexCoord2f(texCoord.x, texCoord.y);

            glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);

            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            glVertex3f(round(vertices[i].x), round(vertices[i].y), 0);
        }


        GL_CHECK_ERROR( glEnd(); );
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );
        GL_CHECK_ERROR( glFlush(); );

        return true;
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawShape(  const std::vector<glm::vec2>& vertices, 
                                                    const std::vector<glm::u8vec4>& colors)
    {
        GetMainWindow()->SetGLContext();

        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        glBegin(GL_TRIANGLE_FAN);

        for(int i = 0; i < vertices.size(); i++)
        {
            glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);   
            glVertex3f(vertices[i].x, vertices[i].y, 0);
        }

        GL_CHECK_ERROR( glEnd(); );
        GL_CHECK_ERROR( glFlush(); );

        return true;
    }
}

}