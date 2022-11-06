#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"


#include "ssGUI/Backend//Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssLogger/ssLog.hpp"

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif

#include <windows.h>            /* must include this before GL/gl.h */

#include "glad/glad.h"
#include "glad/glad_wgl.h"

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

    void BackendDrawingWin32_OpenGL3_3::display()
    {       
        static float num = 0;
        
        /* rotate a triangle around */
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        //glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(0,  1);
        glColor3f(1.0f, 1.0f, 1.0f);
        //glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2i(-1, -1);
        glColor3f(1.0f, 1.0f, 1.0f);
        //glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(1 - num, -1);
        //glVertex2f(1 - 0.25, -1);
        glEnd();
        glFlush();

        num += 0.00001;
    }


    BackendDrawingWin32_OpenGL3_3::BackendDrawingWin32_OpenGL3_3() : BackendIndex(0)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    BackendDrawingWin32_OpenGL3_3::~BackendDrawingWin32_OpenGL3_3()
    {
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    void BackendDrawingWin32_OpenGL3_3::SaveState()
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssLOG_LINE("Failed to get MainWinodw");
            return;
        }

        if(!mainWindow->SetGLContext())
        {
            ssLOG_LINE("Failed to set GL state");
            return;
        }

        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
    }

    void BackendDrawingWin32_OpenGL3_3::RestoreState()
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssLOG_LINE("Failed to get MainWinodw");
            return;
        }

        if(!mainWindow->SetGLContext())
        {
            ssLOG_LINE("Failed to set GL state");
            return;
        }

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();
    }

    //function: DrawEntities
    //Draws the entity based on what is set in the _properties_. Returns true if drawn successfully. *Note that if you are not using <ssGUIManager>, you need to call <Render> at the end in order to render it*.
    bool BackendDrawingWin32_OpenGL3_3::DrawEntities( const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colors,
                                            const std::vector<int>& counts,
                                            const std::vector<ssGUI::DrawingProperty>& properties)
    {
        /*
        //Check correct backend index
        if(BackendIndex >= ssGUI::Backend::BackendManager::GetMainWindowCount())
            BackendIndex = 0;

        if(ssGUI::Backend::BackendManager::GetDrawingInterface(BackendIndex) != 
            static_cast<ssGUI::Backend::BackendDrawingInterface*>(this))
        {
            BackendIndex = ssGUI::Backend::BackendManager::GetDrawingInterfaceIndex(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
        }

        //Check if the main window is already closed
        if(ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->IsClosed())
            return false;

        //Start drawing
        int currentIndex = 0;
        for(int i = 0; i < counts.size(); i++)
        {
            bool result = true;
            //Drawing text
            if(properties[i].fontP != nullptr)
            {
                result = DrawShape(  vertices, texCoords, colors, currentIndex, currentIndex + counts[i],
                    *properties[i].fontP, properties[i].characterSize);
            }
            //Drawing image
            else if(properties[i].imageP != nullptr)
            {
                result = DrawShape(  vertices, texCoords, colors, currentIndex, currentIndex + counts[i],
                    *properties[i].imageP);
            }
            //Drawing shapes
            else 
            {
                result = DrawShape(  vertices, colors, currentIndex, currentIndex + counts[i]);
            }
            currentIndex += counts[i];

            if(!result)
                return false;
        }*/

        return true;
    }

    void BackendDrawingWin32_OpenGL3_3::Render(glm::u8vec3 clearColor)
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssLOG_LINE("Failed to get MainWinodw");
            return;
        }

        mainWindow->SetGLContext();

        SaveState();

        display();
        
        //Get the device context for the window
        HDC hDC = GetDC(static_cast<ssGUI::Backend::Win32_OpenGL_Handles*>(mainWindow->GetRawHandle())->WindowHandle);
        if(hDC == NULL)
        {
            ssLOG_LINE("Failed to GetDC");
            ssLOG_EXIT_PROGRAM();
        }
        SwapBuffers(hDC);

        RestoreState();
    }

    void BackendDrawingWin32_OpenGL3_3::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        
    }
    
    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int CharacterSize)
    {
        return true;        
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        return true;        
    }


    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors)
    {
        return true;        
    }

    //NOTE: End index is exclusive
    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int CharacterSize)
    {
        return true;        
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        return true;        
    }


    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex)
    {
        return true;        
    }



}

}