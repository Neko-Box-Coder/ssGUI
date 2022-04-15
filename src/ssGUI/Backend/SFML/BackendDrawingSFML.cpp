#include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"

namespace ssGUI::Backend
{
    BackendDrawingSFML::BackendDrawingSFML() : BackendIndex(0)
    {
        ssGUI::Backend::BackendManager::AddDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    BackendDrawingSFML::~BackendDrawingSFML()
    {
        ssGUI::Backend::BackendManager::RemoveDrawingInterface(static_cast<ssGUI::Backend::BackendDrawingInterface*>(this));
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colours,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int characterSize)
    {
        return DrawShape(vertices, texCoords, colours, 0, vertices.size(), font, characterSize);
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colours,
                                        const ssGUI::Backend::BackendImageInterface& image)
    {
        return DrawShape(vertices, texCoords, colours, 0, vertices.size(), image);
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colours)
    {
        return DrawShape(vertices, colours, 0, vertices.size());
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colours,
                                        int startIndex, int endIndex,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int CharacterSize)
    {        
        if(!((ssGUI::Backend::BackendFontSFML&)font).IsValid())
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
            outputShape[i - startIndex].color = sf::Color(colours[i].r, colours[i].g, colours[i].b, colours[i].a);
        }

        targetWindow->draw(outputShape, &((ssGUI::Backend::BackendFontSFML&)font).GetSFMLFont()->getTexture(CharacterSize));

        return true;
    }

    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colours,
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
            outputShape[i - startIndex].color = sf::Color(colours[i].r, colours[i].g, colours[i].b, colours[i].a);
        }

        targetWindow->draw(outputShape, (sf::Texture*)(((ssGUI::Backend::BackendImageSFML&)image).GetRawHandle()));

        return true;
    }


    bool BackendDrawingSFML::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colours,
                                        int startIndex, int endIndex)
    {      
        if(vertices.size() != colours.size() || vertices.empty())
        {
            // std::cout<<"vertices size: "<<vertices.size()<<"\n";
            // std::cout<<"colours size: "<<colours.size()<<"\n";
            return false;
        }
        
        sf::RenderWindow* targetWindow = static_cast<sf::RenderWindow*>
            (ssGUI::Backend::BackendManager::GetMainWindowInterface(BackendIndex)->GetRawHandle());

        // create an array of 3 vertices that define a triangle primitive
        sf::VertexArray outputShape(sf::TriangleFan, endIndex - startIndex);

        for(int i = startIndex; i < endIndex; i++)
        {                        
            outputShape[i - startIndex].position = sf::Vector2f(round(vertices[i].x), round(vertices[i].y));//targetWindow->mapPixelToCoords(sf::Vector2i(round(vertices[i].x), round(vertices[i].y)));
            outputShape[i - startIndex].color = sf::Color(colours[i].r, colours[i].g, colours[i].b, colours[i].a);
        }
        
        targetWindow->draw(outputShape);
        
        return true;
    }

    bool BackendDrawingSFML::DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colours,
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
                result = DrawShape(  vertices, texCoords, colours, currentIndex, currentIndex + counts[i],
                                    *properties[i].fontP, properties[i].characterSize);
            }
            //Drawing image
            else if(properties[i].imageP != nullptr)
            {
                result = DrawShape(  vertices, texCoords, colours, currentIndex, currentIndex + counts[i],
                                    *properties[i].imageP);
            }
            //Drawing shapes
            else 
            {
                result = DrawShape(  vertices, colours, currentIndex, currentIndex + counts[i]);
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
}