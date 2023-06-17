#include "ssGUI/Backend/Mocks/BackendDrawingMock.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendDrawingMock::BackendDrawingMock(BackendDrawingMock const& other)
    {
        ssGUI_ERROR(ssGUI_BACKEND_TAG, "Not Implemented");
        ssLOG_EXIT_PROGRAM(1);
        //std::copy(std::begin(other.Vertices), std::end(other.Vertices), std::begin(Vertices));
        //std::copy(std::begin(other.TexCoords), std::end(other.TexCoords), std::begin(TexCoords));
        //std::copy(std::begin(other.Colors), std::end(other.Colors), std::begin(Colors));
        //std::copy(std::begin(other.Counts), std::end(other.Counts), std::begin(Counts));
        //std::copy(std::begin(other.Properties), std::end(other.Properties), std::begin(Properties));
        //std::copy(std::begin(other.ClearedColor), std::end(other.ClearedColor), std::begin(ClearedColor));
    }

    BackendDrawingMock::BackendDrawingMock(ssGUI::Backend::BackendDrawingInterface* drawingInterface) :
        UnderlyingInterface(drawingInterface),
        SavedStateCount(0),
        CurrentDrawingBuffer(0),
        Vertices(),
        TexCoords(),
        Colors(),
        Counts(),
        Properties(),
        ClearedColor(),
        CachedImage()
    {
        ClearedColor[0] = glm::u8vec3(255, 255, 255);
        ClearedColor[1] = glm::u8vec3(255, 255, 255);
    }

    BackendDrawingMock::~BackendDrawingMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;

        for(auto it = CachedImage.begin(); it != CachedImage.end(); it++)
            (*it)->Internal_RemoveBackendDrawingRecord(this);
    }
    
    int BackendDrawingMock::GetStateCounter() const
    {
        return SavedStateCount;    
    }
    
    const std::vector<glm::vec2>& BackendDrawingMock::GetDrawnVertices() const
    {
        return Vertices[CurrentDrawingBuffer];
    }
    
    const std::vector<glm::vec2>& BackendDrawingMock::GetRenderedVertices() const
    {
        return Vertices[(CurrentDrawingBuffer + 1) % 2];
    }
    
    const std::vector<glm::vec2>& BackendDrawingMock::GetDrawnTexCoords() const
    {
        return TexCoords[CurrentDrawingBuffer];
    }
    
    const std::vector<glm::vec2>& BackendDrawingMock::GetRenderedTexCoords() const
    {
        return TexCoords[(CurrentDrawingBuffer + 1) % 2];
    }
    
    const std::vector<glm::u8vec4>& BackendDrawingMock::GetDrawnColors() const
    {
        return Colors[CurrentDrawingBuffer];
    }
    
    const std::vector<glm::u8vec4>& BackendDrawingMock::GetRenderedColors() const
    {
        return Colors[(CurrentDrawingBuffer + 1) % 2];
    }
    
    const std::vector<int>& BackendDrawingMock::GetDrawnCounts() const
    {
        return Counts[CurrentDrawingBuffer];
    }
    
    const std::vector<int>& BackendDrawingMock::GetRenderedCounts() const
    {
        return Counts[(CurrentDrawingBuffer + 1) % 2];
    }
    
    const std::vector<ssGUI::DrawingProperty>& BackendDrawingMock::GetDrawnProperties() const
    {
        return Properties[CurrentDrawingBuffer];
    }
    
    const std::vector<ssGUI::DrawingProperty>& BackendDrawingMock::GetRenderedProperties() const
    {
        return Properties[(CurrentDrawingBuffer + 1) % 2];
    }
    
    const glm::u8vec3& BackendDrawingMock::GetDrawnClearedColor() const
    {
        return ClearedColor[CurrentDrawingBuffer];
    }
    
    const glm::u8vec3& BackendDrawingMock::GetRenderedClearedColor() const
    {
        return ClearedColor[(CurrentDrawingBuffer + 1) % 2];
    }
    
    void BackendDrawingMock::SaveState()
    {
        SSGUI_MOCK_PASSTHROUGH(SaveState());
        SavedStateCount++;
    }

    void BackendDrawingMock::RestoreState()
    {
        SSGUI_MOCK_PASSTHROUGH(SaveState());
        SavedStateCount--;
    }

    bool BackendDrawingMock::DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                            const std::vector<glm::vec2>& texCoords,
                                            const std::vector<glm::u8vec4>& colors,
                                            const std::vector<int>& counts,
                                            const std::vector<ssGUI::DrawingProperty>& properties)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_IF_FALSE(DrawEntities(vertices, texCoords, colors, counts, properties));

        Vertices[CurrentDrawingBuffer].insert(Vertices[CurrentDrawingBuffer].end(), vertices.begin(), vertices.end());
        TexCoords[CurrentDrawingBuffer].insert(TexCoords[CurrentDrawingBuffer].end(), texCoords.begin(), texCoords.end());
        Colors[CurrentDrawingBuffer].insert(Colors[CurrentDrawingBuffer].end(), colors.begin(), colors.end());
        Counts[CurrentDrawingBuffer].insert(Counts[CurrentDrawingBuffer].end(), counts.begin(), counts.end());
        Properties[CurrentDrawingBuffer].insert(Properties[CurrentDrawingBuffer].end(), properties.begin(), properties.end());

        for(int i = 0; i < properties.size(); i++)
        {
            if(properties[i].imageP != nullptr)
                AddImageCache(properties[i].imageP);
        }

        return true;
    }

    void BackendDrawingMock::Render(glm::u8vec3 clearColor)
    {
        SSGUI_MOCK_PASSTHROUGH(Render(clearColor));

        CurrentDrawingBuffer = (CurrentDrawingBuffer + 1) % 2;

        Vertices[CurrentDrawingBuffer].clear();
        TexCoords[CurrentDrawingBuffer].clear();
        Colors[CurrentDrawingBuffer].clear();
        Counts[CurrentDrawingBuffer].clear();
        Properties[CurrentDrawingBuffer].clear();
        ClearedColor[CurrentDrawingBuffer] = clearColor;
    }

    void BackendDrawingMock::ClearBackBuffer(glm::u8vec3 clearColor) 
    {
        SSGUI_MOCK_PASSTHROUGH(ClearBackBuffer(clearColor));
        
        Vertices[CurrentDrawingBuffer].clear();
        TexCoords[CurrentDrawingBuffer].clear();
        Colors[CurrentDrawingBuffer].clear();
        Counts[CurrentDrawingBuffer].clear();
        Properties[CurrentDrawingBuffer].clear();
        ClearedColor[CurrentDrawingBuffer] = clearColor;
    }
    
    void BackendDrawingMock::AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        //NOTE: The underlying interface will get an extra AddImageCache call
        SSGUI_MOCK_PASSTHROUGH(AddImageCache(backendImage));

        if(CachedImage.find(backendImage) == CachedImage.end())
        {
            CachedImage.insert(backendImage);
            backendImage->Internal_AddBackendDrawingRecord(this);
        }
    }
    
    void BackendDrawingMock::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        //NOTE: The underlying interface will get an extra RemoveImageCache call
        SSGUI_MOCK_PASSTHROUGH(RemoveImageCache(backendImage));
        
        if(CachedImage.find(backendImage) != CachedImage.end())
        {
            CachedImage.erase(backendImage);
            backendImage->Internal_RemoveBackendDrawingRecord(this);
        }
    }
    
    void* BackendDrawingMock::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawImageCacheHandle(backendImage), void*);
        return CachedImage.find(backendImage) != CachedImage.end() ? backendImage : nullptr;
    }

    bool BackendDrawingMock::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const uint32_t character,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int characterSize)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "This function is not supposed to be called");
        return true;
    }

    bool BackendDrawingMock::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const ssGUI::Backend::BackendImageInterface& image)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "This function is not supposed to be called");
        return true;
    }


    bool BackendDrawingMock::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colors)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "This function is not supposed to be called");
        return true;        
    }

    //NOTE: End index is exclusive
    bool BackendDrawingMock::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const uint32_t character,
                                        int startIndex, int endIndex,
                                        const ssGUI::Backend::BackendFontInterface& font,
                                        int characterSize)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "This function is not supposed to be called");
        return true;        
    }

    bool BackendDrawingMock::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        int startIndex, int endIndex,
                                        const ssGUI::Backend::BackendImageInterface& image)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "This function is not supposed to be called");
        return true;        
    }


    bool BackendDrawingMock::DrawShape( const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::u8vec4>& colors,
                                        int startIndex, int endIndex)
    {
        ssGUI_WARNING(ssGUI_BACKEND_TAG, "This function is not supposed to be called");
        return true;        
    }
}

}