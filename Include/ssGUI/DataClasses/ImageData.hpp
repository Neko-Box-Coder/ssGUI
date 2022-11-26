#ifndef SSGUI_IMAGE_DATA
#define SSGUI_IMAGE_DATA

#include "ssGUI/Backend/BackendFactory.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::ImageData
    If you are using <LoadRawFromMemory>, remember to reload it again if there's any changes to the image memory.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::Backend::BackendImageInterface* BackendImage;    //See <GetBackendImageInterface>
    =================================================================
    ============================== C++ ==============================
    ImageData::ImageData() :    BackendImage(nullptr)
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
            ssGUI::Backend::BackendImageInterface* BackendImage;    //See <GetBackendImageInterface>

            ImageData(ImageData const& other);
        
        public:
            ImageData();
            virtual ~ImageData();

            //function: GetBackendImageInterface
            //Gets the pointer of the BackendImageInterface
            virtual ssGUI::Backend::BackendImageInterface* GetBackendImageInterface() const;

            //function: IsValid
            //See <BackendImageInterface::IsValid>
            virtual bool IsValid() const;

            //TODO: Add callback when the image is changed to allow <ssGUI::Image> widget to redraw

            //function: LoadFromPath
            //See <ssGUI::Backend::BackendImageInterface::LoadFromPath>
            virtual bool LoadFromPath(std::string path);

            //function: LoadImgFileFromMemory
            //See <ssGUI::Backend::BackendImageInterface::LoadImgFileFromMemory>
            virtual bool LoadImgFileFromMemory(void const * dataPtr, std::size_t size);

            //function: LoadRawFromMemory
            //See <BackendImageInterface::LoadRawFromMemory>
            bool LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize);
            
            //function: GetSize
            //See <ssGUI::Backend::BackendImageInterface::GetSize>
            virtual glm::ivec2 GetSize() const;

            //function: GetPixelPtr
            //See <ssGUI::Backend::BackendImageInterface::GetPixelPtr>
            virtual void* GetPixelPtr(ssGUI::ImageFormat& format);

            //function: Clone
            //Clones this ImageData object. Use this function instead of assignment operator or copy constructor.
            virtual ImageData* Clone();
    };
}



#endif