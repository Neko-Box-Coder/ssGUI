#include "ssGUI/Backend/Mocks/BackendImageMock.hpp"

#include "ssGUI/Backend/Mocks/MockMacro.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendImageMock::BackendImageMock(BackendImageMock const& other)
    {
        if(other.UnderlyingInterface != nullptr)
            UnderlyingInterface = other.UnderlyingInterface->Clone();
        else
            UnderlyingInterface = nullptr;
        
        LinkedBackendDrawing = std::vector<ssGUI::Backend::BackendDrawingInterface*>();
    }

    BackendImageMock::BackendImageMock(ssGUI::Backend::BackendImageInterface* imageInterface)
    {
        UnderlyingInterface = imageInterface;
    }

    BackendImageMock::~BackendImageMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
    }

    void* BackendImageMock::GetRawHandle()
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetRawHandle());
        return nullptr;
    }

    bool BackendImageMock::IsValid() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(IsValid());
        return true;
    }

    bool BackendImageMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(LoadFromPath(path));
        return true;
    }

    bool BackendImageMock::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(LoadImgFileFromMemory(dataPtr, size));
        return true;
    }

    bool BackendImageMock::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(LoadRawFromMemory(dataPtr, format, imageSize));
        return true;
    }
    
    glm::ivec2 BackendImageMock::GetSize() const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetSize());
        return glm::ivec2();
    }

    void* BackendImageMock::GetPixelPtr(ssGUI::ImageFormat& format) const
    {
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN(GetPixelPtr(format));
        return nullptr;
    }

    void BackendImageMock::UpdateCache()
    {
        SSGUI_MOCK_PASSTHROUGH(UpdateCache());
    }

    void BackendImageMock::Internal_AddBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        SSGUI_MOCK_PASSTHROUGH(Internal_AddBackendDrawingRecord(backendDrawing));
    
        for(int i = 0; i < LinkedBackendDrawing.size(); i++)
        {
            if(LinkedBackendDrawing[i] == backendDrawing)
                return;
        }
        
        LinkedBackendDrawing.push_back(backendDrawing);
    }

    void BackendImageMock::Internal_RemoveBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        SSGUI_MOCK_PASSTHROUGH(Internal_RemoveBackendDrawingRecord(backendDrawing));

        for(int i = 0; i < LinkedBackendDrawing.size(); i++)
        {
            if(LinkedBackendDrawing[i] == backendDrawing)
            {
                LinkedBackendDrawing.erase(LinkedBackendDrawing.begin() + i);
                return;
            }
        }
    }

    BackendImageInterface* BackendImageMock::Clone()
    {
        return new BackendImageMock(*this);
    }
}

}