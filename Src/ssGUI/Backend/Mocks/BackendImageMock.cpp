#include "ssGUI/Backend/Mocks/BackendImageMock.hpp"

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
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
        
        LinkedBackendDrawing = std::vector<DrawingInterface*>();
        OverrideObject = other.OverrideObject;
    }

    BackendImageMock::BackendImageMock(ImageInterface* imageInterface) : 
        UnderlyingInterface(imageInterface),
        LinkedBackendDrawing()
    {}

    BackendImageMock::~BackendImageMock()
    {
        if(UnderlyingInterface != nullptr)
            delete UnderlyingInterface;
            
        //Remove all linked backend drawing
        std::vector<DrawingInterface*> backends = LinkedBackendDrawing;
        for(int i = 0; i < backends.size(); i++)
            backends[i]->RemoveImageCache(this);
    }

    void* BackendImageMock::GetRawHandle() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetRawHandle(), void*);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetRawHandle(), void*);
        return nullptr;
    }

    bool BackendImageMock::IsValid() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, IsValid(), bool);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(IsValid(), bool);
        return true;
    }

    bool BackendImageMock::LoadFromPath(std::string path)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(path);
        CO_RETURN_IF_FOUND(OverrideObject, LoadFromPath(std::string), bool, path);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadFromPath(path), bool);
        return true;
    }

    bool BackendImageMock::LoadImgFileFromMemory(const void * dataPtr, std::size_t size)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(dataPtr, size);
        CO_RETURN_IF_FOUND( OverrideObject, 
                            LoadImgFileFromMemory(const void*, std::size_t), 
                            bool, 
                            dataPtr, 
                            size);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadImgFileFromMemory(dataPtr, size), bool);
        return true;
    }

    bool BackendImageMock::LoadRawFromMemory(   const void * dataPtr, 
                                                ImageFormat format, 
                                                glm::ivec2 imageSize)
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(dataPtr, format, imageSize);
        CO_RETURN_IF_FOUND( OverrideObject, 
                            LoadRawFromMemory(const void*, ImageFormat, glm::ivec2), 
                            bool, 
                            dataPtr, 
                            format, 
                            imageSize);
        
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(LoadRawFromMemory(dataPtr, format, imageSize), bool);
        return true;
    }
    
    glm::ivec2 BackendImageMock::GetSize() const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        CO_RETURN_IF_FOUND(OverrideObject, GetSize(), glm::ivec2);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetSize(), glm::ivec2);
        return glm::ivec2();
    }

    void* BackendImageMock::GetPixelPtr(ImageFormat& format) const
    {
        SSGUI_MOCK_LOG_FUNCTION_CALL(format);
        CO_MODIFY_ARGS_IF_FOUND(OverrideObject, GetPixelPtr(ImageFormat&), format);
        CO_RETURN_IF_FOUND(OverrideObject, GetPixelPtr(ImageFormat&), void*, format);
        SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(GetPixelPtr(format), void*);
        return nullptr;
    }

    void BackendImageMock::UpdateCache()
    {
        //NOTE: We don't want to pass the drawing backend to the underlying interface
        //      since the drawing backend holds a reference (pointer) to this instead
        
        SSGUI_MOCK_LOG_FUNCTION_CALL();
        std::vector<DrawingInterface*> backends = LinkedBackendDrawing;
        for(int i = 0; i < backends.size(); i++)
        {
            backends[i]->RemoveImageCache(this);
            backends[i]->AddImageCache(this);
        }
    }

    void BackendImageMock::Internal_AddBackendDrawingRecord(DrawingInterface* backendDrawing)
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

    void BackendImageMock::Internal_RemoveBackendDrawingRecord(DrawingInterface* backendDrawing)
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

    ImageInterface* BackendImageMock::Clone()
    {
        return new BackendImageMock(*this);
    }
}

}