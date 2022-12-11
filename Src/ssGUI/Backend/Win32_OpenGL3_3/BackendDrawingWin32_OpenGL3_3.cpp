#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"

#include "ssGUI/HelperClasses/ImageConversion.hpp"

#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/BackendManager.hpp"
#include "ssLogger/ssLog.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


//TODO: Remvoe this, this is only for testing purposes
#include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"


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
        ssLOG_LINE("Failed: "<<err);\
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

    //TODO: Move this to somewhere else
    void BackendDrawingWin32_OpenGL3_3::InitiateDrawIfNeeded()
    {
        if(!FirstDrawCall)
            return;

        FirstDrawCall = false;

        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssLOG_LINE("Failed to get MainWinodw");
            return;
        }

        //SaveState();
        
        mainWindow->SetGLContext();

        //TODO: Check render size
        glm::ivec2 renderSize = mainWindow->GetRenderSize();
        //ssLOG_LINE("renderSize: "<<renderSize.x<<", "<<renderSize.y);
        GL_CHECK_ERROR( glViewport(0, 0, renderSize.x, renderSize.y); );
        //glViewport(0, 0, render, 1000);
        
        GL_CHECK_ERROR( glMatrixMode(GL_MODELVIEW););
        GL_CHECK_ERROR( glLoadIdentity(); );
        
        //Set coord
        glm::mat4x4 orthoMat = glm::ortho<float>(0.f, renderSize.x, renderSize.y, 0.f, 0, 100000);
        GL_CHECK_ERROR( glMultMatrixf(glm::value_ptr(orthoMat)); );

        //CreateShaders();
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
            ssLOG_LINE("Failed to get MainWinodw");
            return;
        }

        if(!mainWindow->SetGLContext())
        {
            ssLOG_LINE("Failed to set GL state");
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

    //function: DrawEntities
    //Draws the entity based on what is set in the _properties_. Returns true if drawn successfully. *Note that if you are not using <ssGUIManager>, you need to call <Render> at the end in order to render it*.
    bool BackendDrawingWin32_OpenGL3_3::DrawEntities( const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colors,
                                            const std::vector<int>& counts,
                                            const std::vector<ssGUI::DrawingProperty>& properties)
    {
        //Check if the main window is already closed
        if(GetMainWindow()->IsClosed())
            return false;

        //Check if the window already has the texture program
        // if(WindowTexturePrograms.find(GetMainWindow()) == WindowTexturePrograms.end())
        //     CreateTextureShaders();

        //TODO: Set program if needed

        InitiateDrawIfNeeded();

        //Start drawing
        int currentIndex = 0;
        for(int i = 0; i < counts.size(); i++)
        {
            bool result = true;
            //Drawing text
            if(properties[i].fontP != nullptr)
            {
                result = DrawShape(  vertices, texCoords, colors, properties[i].character, currentIndex, currentIndex + counts[i],
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
        }

        return true;
    }

    void BackendDrawingWin32_OpenGL3_3::Render(glm::u8vec3 clearColor)
    {
        /*static bool firstDraw = true;
        if(!firstDraw)
            return;

        firstDraw = false;*/
        
        ssGUI::Backend::BackendMainWindowInterface* mainWindow = GetMainWindow();
        
        if(mainWindow == nullptr)
        {
            ssLOG_LINE("Failed to get MainWinodw");
            return;
        }

        //display();
        
        //Get the device context for the window
        
        HDC hDC = GetDC(static_cast<ssGUI::Backend::Win32_OpenGL_Handles*>(mainWindow->GetRawHandle())->WindowHandle);
        if(hDC == NULL)
        {
            ssLOG_LINE("Failed to GetDC");
            ssLOG_EXIT_PROGRAM();
        }
        SwapBuffers(hDC);
        ssLOG_LINE("Swapped");

        GL_CHECK_ERROR( glClearColor(1, 1, 1, 1); );
        //glClearColor(0, 0, 0, 1);
        GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );

        //RestoreState();

        FirstDrawCall = true;
    }

    void BackendDrawingWin32_OpenGL3_3::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        GL_CHECK_ERROR( glClearColor(1, 1, 1, 1); );
        GL_CHECK_ERROR( glClear(GL_COLOR_BUFFER_BIT); );
    }

    void BackendDrawingWin32_OpenGL3_3::RemoveImageLinking(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        //TODO
    }

    
    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize)
    {
        //TODO: Implement this
        return true;
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        //TODO: Implement this
        return true;
    }


    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors)
    {
        //TODO: Implement this
        return true;        
    }

    //NOTE: End index is exclusive
    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize)
    {
        auto& rawFont = const_cast<ssGUI::Backend::BackendFontInterface&>(font);
        auto freetypeHandle = static_cast<FreeTypeHandles*>(rawFont.GetRawHandle());
        
        rawFont.GetCharacterRenderInfo(character, characterSize);
        FT_UInt glyphIndex = FT_Get_Char_Index(freetypeHandle->FontFace, static_cast<FT_ULong>(character));
        FT_Error error = FT_Load_Glyph(freetypeHandle->FontFace, glyphIndex, freetypeHandle->FontFlags);

        if(error)
        {
            ssLOG_LINE("Failed to load glyph");
            return false;
        }

        //error = FT_Render_Glyph(freetypeHandle->FontFace->glyph, FT_RENDER_MODE_NORMAL);
        //if(error)
        //{
        //    ssLOG_LINE("Failed to FT_Render_Glyph");
        //    return false;
        //}

        GetMainWindow()->SetGLContext();

        //GL_CHECK_ERROR( glUseProgram(ProgramId); );
    
        glm::ivec2 imgSize = glm::ivec2(freetypeHandle->FontFace->glyph->bitmap.width, freetypeHandle->FontFace->glyph->bitmap.rows);
        if(CharTextures.find(character) == CharTextures.end())
        {
            GLuint textureId = 0;

            GL_CHECK_ERROR( glGenTextures(1, &textureId); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); );

            uint8_t* CharImg = new uint8_t[imgSize.x * imgSize.y * 4];
            for(int y = 0; y < imgSize.y; y++)
            {
                for(int x = 0; x < imgSize.x; x++)
                {
                    int curIndex = y * imgSize.x + x;
                    uint8_t pixValue = freetypeHandle->FontFace->glyph->bitmap.buffer[curIndex];
                    CharImg[curIndex * 4] = 255;
                    CharImg[curIndex * 4 + 1] = 255;
                    CharImg[curIndex * 4 + 2] = 255;
                    CharImg[curIndex * 4 + 3] = pixValue;
                }
            }




            GL_CHECK_ERROR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSize.x, imgSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                            CharImg); );

            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );

            //delete[] testImg;
            //testImg2 = testImg;

            CharTextures[character] = textureId;

            delete[] CharImg;
            ssLOG_LINE("Called");
        }
        
        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, CharTextures[character]); );

        //GLubyte* images = new GLubyte[imgSize.x * imgSize.y * 4];
        //glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, images);



        //glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, texDat);
        //if((err = glGetError()) != GL_NO_ERROR)
        //{
        //    ssLOG_LINE("Failed: "<<err);
        //}

        //GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );

         
        GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D); );
        //GL_CHECK_ERROR( glEnable(GL_DEPTH_TEST) );
        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        ssLOG_LINE("Image Size: "<<imgSize.x<<", "<<imgSize.y);

        //glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLE_FAN);

        //glm::vec2 imgSize = texCoords[startIndex + 2];
        //glColor4f(1, 1, 1, 1.0f);

        for(int i = startIndex; i < endIndex; i++)
        {
            //glVertexAttrib2f(0, vertices[i].x, vertices[i].y);

            
            glm::vec2 texCoord = texCoords[i];

            texCoord.x /= imgSize.x;
            texCoord.y /= imgSize.y;
            
            glm::ivec2 texCoordI = texCoord;

            //glTexCoord1f()
            //glColor4b(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            
            
            switch(i)
            {
            case 4:
            texCoord = glm::vec2(0, 0);
            break;
            case 5:
            texCoord = glm::vec2(1, 0);
            break;
            case 6:
            texCoord = glm::vec2(1, 1);
            break;
            case 7:
            texCoord = glm::vec2(0, 1);
            break;
            }
            //glTexCoord2i(texCoordI.x, texCoordI.y);
            glTexCoord2f(texCoord.x, texCoord.y);

            glColor4f(0, 0, 0, 1);

            glVertex3f(vertices[i].x, vertices[i].y, 0);


            //glVertexAttrib4f(1, colors[i].r, colors[i].g, colors[i].b, colors[i].a);

            //glColor4i(0, 0, 0, 1);
            
            //glVertexAttrib2f(2, texCoord.x, texCoord.y);
            
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            //outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));
            //outputShape[i - startIndex].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            //outputShape[i - startIndex].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);          
        
            ssLOG_LINE("vertices["<<i<<"]: "<<vertices[i].x<<", "<<vertices[i].y);
            ssLOG_LINE("uv["<<i<<"]: "<<texCoordI.x<<", "<<texCoordI.y);
        }

        //GL_CHECK_ERROR( glDrawArrays(GL_TRIANGLE_FAN, 0, endIndex - startIndex); );

        GL_CHECK_ERROR( glEnd(); );

        //GL_CHECK_ERROR( glDisable(GL_TEXTURE_2D); );

        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );

        GL_CHECK_ERROR( glFlush(); );
        //delete[] images;


        return true;
    }

    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image)
    {
        GetMainWindow()->SetGLContext();

        //GL_CHECK_ERROR( glUseProgram(ProgramId); );
        glm::ivec2 imgSize = image.GetSize();
        //TODO: Remove const cast
        if(ImageTextures.find(const_cast<ssGUI::Backend::BackendImageInterface*>(&image)) == ImageTextures.end())
        {
            //Get image
            ssGUI::ImageFormat format;
            const void* rawPtr = image.GetPixelPtr(format);

            //Convert it to rgba32
            uint8_t* rgba32Img = new uint8_t[image.GetSize().x * image.GetSize().y * 4];
            ssGUI::ImageConversion::ConvertToRGBA32<uint8_t>(static_cast<void*>(rgba32Img), rawPtr, format, image.GetSize());
            
            GLuint textureId = 0;

            GL_CHECK_ERROR( glGenTextures(1, &textureId); );
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); );
            GL_CHECK_ERROR( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); );
            
            //Save it to gpu
            GL_CHECK_ERROR( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSize.x, imgSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                            rgba32Img); );

            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, textureId); );


            ImageTextures[const_cast<ssGUI::Backend::BackendImageInterface*>(&image)] = textureId;

            delete[] rgba32Img;
        }
        else
        {
            GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, ImageTextures[const_cast<ssGUI::Backend::BackendImageInterface*>(&image)]); );
        }

        //render it
        GL_CHECK_ERROR( glEnable(GL_TEXTURE_2D); );
        //GL_CHECK_ERROR( glEnable(GL_DEPTH_TEST) );
        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );

        ssLOG_LINE("Image Size: "<<imgSize.x<<", "<<imgSize.y);

        //glBegin(GL_TRIANGLES);
        glBegin(GL_TRIANGLE_FAN);

        //glm::vec2 imgSize = texCoords[startIndex + 2];
        //glColor4f(1, 1, 1, 1.0f);

        for(int i = startIndex; i < endIndex; i++)
        {
            //glVertexAttrib2f(0, vertices[i].x, vertices[i].y);

            
            glm::vec2 texCoord = texCoords[i];

            texCoord.x /= imgSize.x;
            texCoord.y /= imgSize.y;
            
            glm::ivec2 texCoordI = texCoord;

            //glTexCoord1f()
            //glColor4b(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            
            
            /*
            switch(i)
            {
            case 4:
            texCoord = glm::vec2(0, 0);
            break;
            case 5:
            texCoord = glm::vec2(1, 0);
            break;
            case 6:
            texCoord = glm::vec2(1, 1);
            break;
            case 7:
            texCoord = glm::vec2(0, 1);
            break;
            }*/
            //glTexCoord2i(texCoordI.x, texCoordI.y);
            glTexCoord2f(texCoord.x, texCoord.y);

            glColor4ub(colors[i].r, colors[i].g, colors[i].b, colors[i].a);

            glVertex3f(vertices[i].x, vertices[i].y, 0);


            //glVertexAttrib4f(1, colors[i].r, colors[i].g, colors[i].b, colors[i].a);

            //glColor4i(0, 0, 0, 1);
            
            //glVertexAttrib2f(2, texCoord.x, texCoord.y);
            
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            //outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));
            //outputShape[i - startIndex].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            //outputShape[i - startIndex].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);          
        
            ssLOG_LINE("vertices["<<i<<"]: "<<vertices[i].x<<", "<<vertices[i].y);
            ssLOG_LINE("uv["<<i<<"]: "<<texCoordI.x<<", "<<texCoordI.y);
        }

        //GL_CHECK_ERROR( glDrawArrays(GL_TRIANGLE_FAN, 0, endIndex - startIndex); );

        GL_CHECK_ERROR( glEnd(); );

        //GL_CHECK_ERROR( glDisable(GL_TEXTURE_2D); );

        GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );

        GL_CHECK_ERROR( glFlush(); );

        return true;        
    }


    bool BackendDrawingWin32_OpenGL3_3::DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex)
    {
        return true;
        
        GetMainWindow()->SetGLContext();

        GL_CHECK_ERROR( glEnable(GL_BLEND); );
        GL_CHECK_ERROR( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); );


        //glBegin(GL_QUADS);
        glBegin(GL_TRIANGLE_FAN);

        for(int i = startIndex; i < endIndex; i++)
        {
            //glVertexAttrib2f(0, vertices[i].x, vertices[i].y);

            ssLOG_LINE("vertices["<<i<<"]: "<<vertices[i].x<<", "<<vertices[i].y);
            //glm::vec2 texCoord = texCoords[i];

            //texCoord.x /= imgSize.x;
            //texCoord.y /= imgSize.y;
            
            //glm::ivec2 texCoordI = texCoord;

            //glTexCoord1f()
            //glColor4b(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            glColor4f(1, 0, 0, 1.0f);
            //glVertexAttrib4f(1, colors[i].r, colors[i].g, colors[i].b, colors[i].a);

            //glColor4i(1, 1, 1, 1);
            
            //glVertexAttrib2f(2, texCoord.x, texCoord.y);
            
            //glTexCoord2i(texCoordI.x, texCoordI.y);
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            //outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));
            //outputShape[i - startIndex].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            //outputShape[i - startIndex].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);          
            glVertex3f(vertices[i].x, vertices[i].y, 0);
        }

        //GL_CHECK_ERROR( glDrawArrays(GL_TRIANGLE_FAN, 0, endIndex - startIndex); );

        GL_CHECK_ERROR( glEnd(); );

        //GL_CHECK_ERROR( glDisable(GL_TEXTURE_2D); );

        //GL_CHECK_ERROR( glBindTexture(GL_TEXTURE_2D, 0); );

        GL_CHECK_ERROR( glFlush(); );

        return true;        
    }



}

}