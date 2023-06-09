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
        OverrideObject = other.OverrideObject;
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
        FO_RETURN_IF_FOUND(OverrideObject, GetRawHandle(), void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle());
        return nullptr;
    }

    bool BackendImageMock::IsValid() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, IsValid(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsValid());
        return true;
    }

    bool BackendImageMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(path);
        FO_RETURN_IF_FOUND(OverrideObject, LoadFromPath(std::string), bool, path);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromPath(path));
        return true;
    }

    bool BackendImageMock::LoadImgFileFromMemory(const void * dataPtr, std::size_t size)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(dataPtr, size);
        FO_RETURN_IF_FOUND(OverrideObject, LoadImgFileFromMemory(const void*, std::size_t), bool, dataPtr, size);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadImgFileFromMemory(dataPtr, size));
        return true;
    }

    bool BackendImageMock::LoadRawFromMemory(const void * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(dataPtr, format, imageSize);
        FO_RETURN_IF_FOUND(OverrideObject, LoadRawFromMemory(const void*, ssGUI::ImageFormat, glm::ivec2), bool, dataPtr, format, imageSize);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadRawFromMemory(dataPtr, format, imageSize));
        return true;
    }
    
    glm::ivec2 BackendImageMock::GetSize() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        FO_RETURN_IF_FOUND(OverrideObject, GetSize(), glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetSize());
        return glm::ivec2();
    }

    void* BackendImageMock::GetPixelPtr(ssGUI::ImageFormat& format) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(format);
        FO_ARGUMENTS_IF_FOUND(OverrideObject, GetPixelPtr(ssGUI::ImageFormat&), format);
        FO_RETURN_IF_FOUND(OverrideObject, GetPixelPtr(ssGUI::ImageFormat&), void*, format);
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
    
        SSGUI_MOCK_LOG_FUNCTION_CALL(backendDrawing);
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
    
        SSGUI_MOCK_LOG_FUNCTION_CALL(backendDrawing);
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