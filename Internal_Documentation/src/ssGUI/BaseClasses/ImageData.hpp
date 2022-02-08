#ifndef SSGUI_IMAGE_DATA
#define SSGUI_IMAGE_DATA

#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ImageData
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::Backend::BackendImageInterface* BackendImage;
    =================================================================
    ============================== C++ ==============================
    ImageData::ImageData()
    {
        BackendImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
    }
    =================================================================
    */
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

            //function: GetBackendImageInterface
            //Gets the pointer of the BackendImageInterface
            virtual ssGUI::Backend::BackendImageInterface* GetBackendImageInterface();

            //function: IsValid
            //See <BackendImageInterface::IsValid>
            virtual bool IsValid() const;

            //function: LoadFromPath
            //See <BackendImageInterface::LoadFromPath>
            virtual bool LoadFromPath(std::string path);

            //function: LoadImgFileFromMemory
            //See <BackendImageInterface::LoadImgFileFromMemory>
            virtual bool LoadImgFileFromMemory(void* dataPtr, std::size_t size);

            //function: LoadRawFromMemory
            //See <BackendImageInterface::LoadRawFromMemory>
            virtual bool LoadRawFromMemory(void* dataPtr, int width, int height);

            //function: GetSize
            //See <BackendImageInterface::GetSize>
            virtual glm::ivec2 GetSize() const;

            //function: Clone
            //Clones this ImageData object. Use this function instead of assignment operator or copy constructor.
            virtual ImageData* Clone();
    };
}



#endif