#ifndef SSGUI_IMAGE_DATA
#define SSGUI_IMAGE_DATA

#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"

namespace ssGUI
{
    class ImageData
    {
        private:
            ImageData& operator=(ImageData const& other) = default;

        protected:
            ImageData(ImageData const& other);
            ssGUI::Backend::BackendImageInterface* BackendImage;
        
        public:
            ImageData();
            virtual ~ImageData();

            virtual ssGUI::Backend::BackendImageInterface* GetBackendImageInterface();

            virtual bool IsValid() const;

            virtual bool LoadFromPath(std::string path);

            virtual bool LoadImgFileFromMemory(void* dataPtr, std::size_t size);

            virtual bool LoadRawFromMemory(void* dataPtr, int width, int height);
            
            virtual glm::ivec2 GetSize() const;

            virtual ImageData* Clone();
    };
}



#endif