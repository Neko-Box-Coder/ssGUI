#include "ssGUI/Backend/Mocks/BackendImageMock.hpp"

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
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

    BackendImageMock::BackendImageMock(ssGUI::Backend::BackendImageInterface* imageInterface) : UnderlyingInterface(imageInterface),
                                                                                                LinkedBackendDrawing()
    {}

    BackendImageMock::~BackendImageMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
            
        //Remove all linked backend drawing
        std::vector<ssGUI::Backend::BackendDrawingInterface*> backends = LinkedBackendDrawing;
        for(int i = 0; i < backends.size(); i++)
            backends[i]->RemoveImageCache(this);
    }

    void* BackendImageMock::GetRawHandle()
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND_WITHOUT_ARGS(OverrideObject, &BackendImageMock::GetRawHandle, void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle());
        return nullptr;
    }

    bool BackendImageMock::IsValid() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND_WITHOUT_ARGS(OverrideObject, &BackendImageMock::IsValid, bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsValid());
        return true;
    }

    bool BackendImageMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendImageMock::LoadFromPath, bool, path);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromPath(path));
        return true;
    }

    bool BackendImageMock::LoadImgFileFromMemory(void const * dataPtr, std::size_t size)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendImageMock::LoadImgFileFromMemory, bool, dataPtr, size);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadImgFileFromMemory(dataPtr, size));
        return true;
    }

    bool BackendImageMock::LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendImageMock::LoadRawFromMemory, bool, dataPtr, format, imageSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadRawFromMemory(dataPtr, format, imageSize));
        return true;
    }
    
    glm::ivec2 BackendImageMock::GetSize() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND_WITHOUT_ARGS(OverrideObject, &BackendImageMock::GetSize, glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetSize());
        return glm::ivec2();
    }

    void* BackendImageMock::GetPixelPtr(ssGUI::ImageFormat& format) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, &BackendImageMock::GetPixelPtr, void*, format);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetPixelPtr(format));
        return nullptr;
    }

    void BackendImageMock::UpdateCache()
    {
        //NOTE: We don't want to pass the drawing backend to the underlying interface
        //      since the drawing backend holds a reference (pointer) to this instead
        
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        std::vector<ssGUI::Backend::BackendDrawingInterface*> backends = LinkedBackendDrawing;
        for(int i = 0; i < backends.size(); i++)
        {
            backends[i]->RemoveImageCache(this);
            backends[i]->AddImageCache(this);
        }
    }

    void BackendImageMock::Internal_AddBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        //NOTE: We don't want to pass the drawing backend to the underlying interface
        //      since the drawing backend holds a reference (pointer) to this instead
    
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        for(int i = 0; i < LinkedBackendDrawing.size(); i++)
        {
            if(LinkedBackendDrawing[i] == backendDrawing)
                return;
        }
        
        LinkedBackendDrawing.push_back(backendDrawing);
    }

    void BackendImageMock::Internal_RemoveBackendDrawingRecord(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        //NOTE: We don't want to pass the drawing backend to the underlying interface
        //      since the drawing backend holds a reference (pointer) to this instead
    
        SSGUI_MOCK_LOG_FUNCTION_CALL();
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