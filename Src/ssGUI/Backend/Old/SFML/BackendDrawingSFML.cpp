#include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"
#include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
#include "SFML/OpenGL.hpp"

#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/HelperClasses/ImageUtil.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

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
        
        #ifndef SSGUI_FONT_BACKEND_SFML
            CharTextures = other.CharTextures;
        #endif
        
        //We don't want to copy the GPU texture, so just clear it
        #ifndef SSGUI_IMAGE_BACKEND_SFML
            //Notify each image to delink
            for(auto it = ImageTextures.begin(); it != ImageTextures.end(); it++)
            {
                ssGUI_DEBUG(ssGUI_TAG_BACKEND, "Removing link: "<<it->first);
                it->first->Internal_RemoveBackendDrawingRecord(this);
            }
            ImageTextures.clear();
        #endif
        //TODO: AddDrawingInterface
        ssGUI_ERROR(ssGUI_TAG_BACKEND, "Not implemented");
        ssLOG_EXIT_PROGRAM();
    }

    BackendDrawingSFML::BackendDrawingSFML() :  BackendIndex(0),
                                                #ifndef SSGUI_FONT_BACKEND_SFML
                                                    CharTextures(),
                                                #endif
                                                #ifndef SSGUI_IMAGE_BACKEND_SFML
                                                    ImageTextures(),
                                                #endif
                                                endVar(nullptr)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::DrawingInterface*>(this));
    }

    BackendDrawingSFML::~BackendDrawingSFML()
    {
        #ifndef SSGUI_IMAGE_BACKEND_SFML
            //Notify each image
            for(auto it = ImageTextures.begin(); it != ImageTextures.end(); it++)
                it->first->Internal_RemoveBackendDrawingRecord(this);
        #endif
        
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::DrawingInterface*>(this));
    }

    void BackendDrawingSFML::SaveDrawingState()
    {
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        bool result = targetWindow->setActive(true);

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

    void BackendDrawingSFML::RestoreDrawingState()
    {
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        bool result = targetWindow->setActive(true);

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

    bool BackendDrawingSFML::CreateDrawingEntities(const std::vector<ssGUI::DrawingEntity>& entities)
    {
        //Check correct backend index
        if(BackendIndex >= ssGUI::Backend::BackendManager::GetMainWindowCount())
            BackendIndex = 0;
        
        if(ssGUI::Backend::BackendManager::GetDrawingInterface(BackendIndex) != 
            static_cast<ssGUI::Backend::DrawingInterface*>(this))
        {
            BackendIndex = ssGUI::Backend::BackendManager::GetDrawingInterfaceIndex(static_cast<ssGUI::Backend::DrawingInterface*>(this));
        }

        //Check if the main window is already closed
        if(ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->IsClosed())
            return false;

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

    void BackendDrawingSFML::DrawToBackBuffer()
    {
        //NOTE: Doesn't need to do anything, SFML is handling for us
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
    
    
    void BackendDrawingSFML::AddImageCache(ssGUI::Backend::ImageInterface* backendImage)
    {
        #if !defined(SSGUI_IMAGE_BACKEND_SFML)
            if(!backendImage->IsValid())
                return;
        
            //If we don't have the image stored on the GPU, do it
            if(ImageTextures.find(backendImage) == ImageTextures.end())
            {            
                ssGUI::ImageFormat imgFmt;
                void* imgRawPtr = backendImage->GetPixelPtr(imgFmt);
                sf::Image img;
                bool result = false;
                
                uint8_t* convertedRawImg = new uint8_t[backendImage->GetSize().x * backendImage->GetSize().y * 4];

                result = ssGUI::ImageUtil::FormatToRGBA32(convertedRawImg, imgRawPtr, imgFmt, backendImage->GetSize());
                if(!result)
                {
                    delete[] convertedRawImg;
                    ssGUI_WARNING(ssGUI_TAG_BACKEND, "Failed to convert image");
                    return;
                }
                else
                {
                    img.create(sf::Vector2u(backendImage->GetSize().x, backendImage->GetSize().y), convertedRawImg);
                    delete[] convertedRawImg;
                }
                    
                //Create texture
                result = ImageTextures[backendImage].loadFromImage(img);
                
                //Failed to upload to gpu for whatever reason   
                if(!result)
                {
                    //Cleanup the failed texture
                    ImageTextures.erase(backendImage);
                    return;
                }
                
                //Add link for removing image
                backendImage->Internal_AddBackendDrawingRecord(this);
            }
        #endif
    }
    
    void BackendDrawingSFML::RemoveImageCache(ssGUI::Backend::ImageInterface* backendImage)
    {
        #ifndef SSGUI_IMAGE_BACKEND_SFML
            if(ImageTextures.find(backendImage) != ImageTextures.end())
            {
                ImageTextures.erase(backendImage);
                backendImage->Internal_RemoveBackendDrawingRecord(this);
            }
        #endif
    }
    
    void* BackendDrawingSFML::GetRawImageCacheHandle(ssGUI::Backend::ImageInterface* backendImage)
    {
        #if !defined(SSGUI_IMAGE_BACKEND_SFML)
            if(ImageTextures.find(backendImage) == ImageTextures.end())
                return nullptr;

            return &ImageTextures[backendImage];
        #else
            return backendImage->GetRawHandle();
        #endif
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const uint32_t character,
                                        const ssGUI::Backend::FontInterface& font,
                                        int characterSize)
    {
        if(!font.IsValid())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Trying to draw invalid font");
            return false;
        }
        
        if(vertices.size() != texCoords.size() || texCoords.size() != colors.size())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Each vertex mush have a texture coordinates and color attribute");
            return false;
        }

        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::PrimitiveType::TriangleFan, vertices.size());

        #ifdef SSGUI_FONT_BACKEND_SFML
            sf::IntRect charUV = ((ssGUI::Backend::BackendFontSFML&)font).GetSFMLFont()->getGlyph(character, characterSize, false).textureRect;
        #endif

        for(int i = 0; i < vertices.size(); i++)
        {
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            outputShape[i].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));
            outputShape[i].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            outputShape[i].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);

            #ifdef SSGUI_FONT_BACKEND_SFML
                outputShape[i].texCoords += sf::Vector2f(charUV.left, charUV.top);
            #endif            
        }

        //Using SFML font interface
        #ifdef SSGUI_FONT_BACKEND_SFML
            targetWindow->draw(outputShape, &((ssGUI::Backend::BackendFontSFML&)font).GetSFMLFont()->getTexture(characterSize));
        //Using generic font interface
        #else
            //TODO: Fix this and don't use const cast
            ssGUI::Backend::FontInterface* rawFont = const_cast<ssGUI::Backend::FontInterface*>(&font);

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
                //ssGUI::Backend::FontInterface* freeTypeFont = static_cast<ssGUI::Backend::BackendFontFreeType*>(rawFont); 
                ssGUI::ImageData imgData;

                //If failed to import, clean up and exit
                if(!rawFont->GetCharacterImage(character, characterSize, imgData))
                {
                    ssGUI_WARNING(ssGUI_TAG_BACKEND, "Failed to get character image");
                    return false;
                }
                
                #ifdef SSGUI_IMAGE_BACKEND_SFML
                    CharTextures[id] = *static_cast<sf::Texture*>(imgData.GetImageInterface()->GetRawHandle());
                #else

                    ssGUI::ImageFormat imgFmt;
                    void* imgRawPtr = imgData.GetPixelPtr(imgFmt);
                    sf::Image img;
                    bool result = false;
                    
                    
                    uint8_t* convertedRawImg = new uint8_t[imgData.GetSize().x * imgData.GetSize().y * 4];

                    result = ssGUI::ImageUtil::FormatToRGBA32(convertedRawImg, imgRawPtr, imgFmt, imgData.GetSize());
                    if(!result)
                    {
                        delete[] convertedRawImg;
                        ssGUI_WARNING(ssGUI_TAG_BACKEND, "Failed to convert image");
                        return false;
                    }
                    else
                    {
                        img.create(sf::Vector2u(imgData.GetSize().x, imgData.GetSize().y), convertedRawImg);
                        delete[] convertedRawImg;
                    }
                        
                    //Create texture
                    result = CharTextures[id].loadFromImage(img);
                    
                    //Failed to upload to gpu for whatever reason   
                    if(!result)
                    {
                        //Cleanup the failed texture
                        CharTextures.erase(id);
                        return false;
                    }                
                #endif
            }

            //Draw the character texture
            targetWindow->draw(outputShape, &CharTextures[id]);
        #endif

        return true;
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const ssGUI::Backend::ImageInterface& image)
    {
        if(!image.IsValid())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Trying to draw invalid image");
            return false;
        }

        if(vertices.size() != texCoords.size() || texCoords.size() != colors.size())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Each vertex mush have a texture coordinates and color attribute");
            return false;
        }

        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());
        
        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::PrimitiveType::TriangleFan, vertices.size());

        for(int i = 0; i < vertices.size(); i++)
        {
            //The reason for rounding the position is because it seems like the UV is shifting in floating points, at least for now
            outputShape[i].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));//targetWindow->mapPixelToCoords(sf::Vector2i(round(vertices[i].x), round(vertices[i].y)));
            outputShape[i].texCoords = sf::Vector2f(texCoords[i].x, texCoords[i].y);
            outputShape[i].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        }
        
        //Using SFML image interface
        #ifdef SSGUI_IMAGE_BACKEND_SFML
            targetWindow->draw(outputShape, (sf::Texture*)(((ssGUI::Backend::BackendImageSFML&)image).GetRawHandle()));
        //Using generic Image interface
        #else            
            //TODO: Again same thing, change const to avoid const cast
            auto imgPtr = const_cast<ssGUI::Backend::ImageInterface*>(&image);

            AddImageCache(imgPtr);
            
            //If we don't have the image stored on the GPU, do it
            if(ImageTextures.find(imgPtr) == ImageTextures.end())
                return false;

            //Draw the texture
            targetWindow->draw(outputShape, &ImageTextures[imgPtr]);
        #endif

        return true;
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colors)
    {
        if(vertices.size() != colors.size())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Each vertex mush have a color attribute");
            return false;
        }

        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());

        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::PrimitiveType::TriangleFan, vertices.size());

        for(int i = 0; i < vertices.size(); i++)
        {                        
            outputShape[i].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));//targetWindow->mapPixelToCoords(sf::Vector2i(round(vertices[i].x), round(vertices[i].y)));
            outputShape[i].color = sf::Color(colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        }
        
        targetWindow->draw(outputShape);
        
        return true;
    }
}

}