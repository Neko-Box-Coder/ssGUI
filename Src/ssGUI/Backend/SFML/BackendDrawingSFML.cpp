#include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"
#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "SFML/OpenGL.hpp"

#include "ssGUI/DataClasses/ImageData.hpp"

#ifdef SSGUI_FONT_BACKEND_FREE_TYPE
    #include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"
#endif

namespace ssGUI
{

namespace Backend
{
    BackendDrawingSFML::BackendDrawingSFML(BackendDrawingSFML const& other)
    {
        BackendIndex = other.BackendIndex;   
        CharTexturesQueue = other.CharTexturesQueue;
        CharTextures = other.CharTextures;
    }

    BackendDrawingSFML::BackendDrawingSFML() :  BackendIndex(0),
                                                CharTexturesQueue(),
                                                CharTextures()
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    BackendDrawingSFML::~BackendDrawingSFML()
    {
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    void BackendDrawingSFML::SaveState()
    {
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        targetWindow->setActive(true);

        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();

        //This is not working for whatever reason
        //targetWindow->pushGLStates();
    }

    void BackendDrawingSFML::RestoreState()
    {
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        targetWindow->setActive(true);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();

        //This is not working for whatever reason
        //targetWindow->popGLStates();
    }

    bool BackendDrawingSFML::DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colors,
                                            const std::vector<int>& counts,
                                            const std::vector<ssGUI::DrawingProperty>& properties)
    {
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
                result = DrawShape( vertices, texCoords, colors, properties[i].character, currentIndex, 
                                    currentIndex + counts[i], *properties[i].fontP, properties[i].characterSize);
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

    void BackendDrawingSFML::Render(glm::u8vec3 clearColor)
    {
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());

        targetWindow->display();
        targetWindow->clear(sf::Color(clearColor.r, clearColor.g, clearColor.b, 255));        
    }

    void BackendDrawingSFML::ClearBackBuffer(glm::u8vec3 clearColor)
    {
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());

        targetWindow->clear(sf::Color(clearColor.r, clearColor.g, clearColor.b, 255));        
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const uint32_t character,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int characterSize)
    {
        return DrawShape(vertices, texCoords, colors, character, 0, vertices.size(), font, characterSize);
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const ssGUI::Backend::BackendImageInterface& image)
    {
        return DrawShape(vertices, texCoords, colors, 0, vertices.size(), image);
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colors)
    {
        return DrawShape(vertices, colors, 0, vertices.size());
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const uint32_t character,
                                        int startIndex, int endIndex,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int characterSize)
    {
        if(!font.IsValid())
            return false;

        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::TriangleFan, endIndex - startIndex);

        #ifdef SSGUI_FONT_BACKEND_SFML
            sf::IntRect charUV = ((ssGUI::Backend::BackendFontSFML&)font).GetSFMLFont()->getGlyph(character, characterSize, false).textureRect;
        #endif

        for(int i = startIndex; i < endIndex; i++)
        {
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));
            outputShape[i - startIndex].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            
            #ifdef SSGUI_FONT_BACKEND_SFML
            outputShape[i - startIndex].texCoords += sf::Vector2f(charUV.left, charUV.top);
            #endif
            
            outputShape[i - startIndex].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        }

        #ifdef SSGUI_FONT_BACKEND_SFML            
            targetWindow->draw(outputShape, &((ssGUI::Backend::BackendFontSFML&)font).GetSFMLFont()->getTexture(characterSize));
        #elif defined SSGUI_BACKEND_FONT_FREE_TYPE
            //TODO: Fix this and don't use const cast
            ssGUI::Backend::BackendFontInterface* rawFont = const_cast<ssGUI::Backend::BackendFontInterface*>(&font);

            //load character and check size
            ssGUI::CharacterRenderInfo info = rawFont->GetCharacterRenderInfo(character, characterSize);
            if(!info.Valid)
                return false;

            CharTextureIdentifier id = CharTextureIdentifier(   rawFont, 
                                                                static_cast<CharSize>(characterSize), 
                                                                static_cast<CharCode>(character));
            
            //If we don't have this character stored, add it
            if(CharTextures.find(id) == CharTextures.end())
            {                
                ssGUI::Backend::BackendFontFreeType* freeTypeFont = static_cast<ssGUI::Backend::BackendFontFreeType*>(rawFont); 
                ssGUI::ImageData imgData;

                //If failed to import, clean up and exit
                if(!freeTypeFont->GetCharacterImage(character, characterSize, imgData))
                {
                    return false;
                }
                
                #ifdef SSGUI_IMAGE_BACKEND_SFML
                CharTextures[id] = *static_cast<sf::Texture*>(imgData.GetBackendImageInterface()->GetRawHandle());
                #else
                ssLOG_LINE("Calling non implmented backend");
                return false;
                #endif
            }

            //Draw the character texture
            targetWindow->draw(outputShape, &CharTextures[id]);
        #else
            ssLOG_LINE("Invalid backend, exiting");
            ssLOG_EXIT_PROGRAM();
        #endif

        return true;
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        int startIndex, int endIndex,
                                        const ssGUI::Backend::BackendImageInterface& image)
    {
        if(!image.IsValid())
            return false;

        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::TriangleFan, endIndex - startIndex);

        for(int i = startIndex; i < endIndex; i++)
        {
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));//targetWindow->mapPixelToCoords(sf::Vector2i(round(vertices[i].x), round(vertices[i].y)));
            outputShape[i - startIndex].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            outputShape[i - startIndex].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        }

        targetWindow->draw(outputShape, (sf::Texture*)(((ssGUI::Backend::BackendImageSFML&)image).GetRawHandle()));

        return true;
    }


    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colors,
                                        int startIndex, int endIndex)
    {      
        if(vertices.size() != colors.size() || vertices.empty())
        {
            // std::cout<<"vertices size: "<<vertices.size()<<"\n";
            // std::cout<<"colors size: "<<colors.size()<<"\n";
            return false;
        }
        
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());

        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::TriangleFan, endIndex - startIndex);

        for(int i = startIndex; i < endIndex; i++)
        {                        
            outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));//targetWindow->mapPixelToCoords(sf::Vector2i(round(vertices[i].x), round(vertices[i].y)));
            outputShape[i - startIndex].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        }
        
        targetWindow->draw(outputShape);
        
        return true;
    }
}

}