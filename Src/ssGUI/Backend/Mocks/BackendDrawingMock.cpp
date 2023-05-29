#include "ssGUI/Backend/Mocks/BackendDrawingMock.hpp"
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

//#ifdef SSGUI_MAIN_BACKEND_SFML
//    #include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"
//#elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
//    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"
//#elif defined SSGUI_MAIN_BACKEND_X11_OPENGL
//    #include "ssGUI/Backend/X11_OpenGL3_3/BackendDrawingX11_OpenGL3_3.hpp"
//#endif

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
        SSGUI_MOCK_PASSTHROUGH(AddImageCache(backendImage));

        if(CachedImage.find(backendImage) == CachedImage.end())
        {
            CachedImage.insert(backendImage);
            
            //NOTE: backendImage->Internal_AddBackendDrawingRecord would be called by 
            //      UnderlyingInterface if it exists, hence the check
            if(UnderlyingInterface == nullptr)
                backendImage->Internal_AddBackendDrawingRecord(this);
        }
    }
    
    void BackendDrawingMock::RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        SSGUI_MOCK_PASSTHROUGH(RemoveImageCache(backendImage));
        
        if(CachedImage.find(backendImage) != CachedImage.end())
        {
            CachedImage.erase(backendImage);
        
            //NOTE: backendImage->Internal_AddBackendDrawingRecord would be called by 
            //      UnderlyingInterface if it exists, hence the check
            if(UnderlyingInterface == nullptr)
                backendImage->Internal_RemoveBackendDrawingRecord(this);
        }
    }
    
    void* BackendDrawingMock::GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetRawImageCacheHandle(backendImage));
        
        return CachedImage.find(backendImage) != CachedImage.end() ?  backendImage : nullptr;
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