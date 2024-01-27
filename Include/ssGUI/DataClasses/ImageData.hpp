#ifndef SSGUI_IMAGE_DATA_H
#define SSGUI_IMAGE_DATA_H

#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include <functional>
#include <vector>
//#include <iostream>

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::ImageData
    If you are using <LoadRawFromMemory>, remember to reload it again if there's any changes to the image memory.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::Backend::ImageInterface* BackendImage;                        //See <GetImageInterface>
        std::vector<std::function<void(ssGUI::ImageData*)>> DataChangedCallbacks;   //See <AddDataChangedCallback>
        std::vector<bool> DataChangedCallbacksValid;                                //See <AddDataChangedCallback>
    =================================================================
    ============================== C++ ==============================
    ImageData::ImageData() :    BackendImage(nullptr),
                                DataChangedCallbacks(),
                                DataChangedCallbacksValid()
    {
        BackendImage = ssGUI::Backend::Factory::CreateImageInterface();
    }
    =================================================================
    */
    class ImageData
    {
        private:
            ImageData& operator=(ImageData const& other) = default;

        protected:
            ssGUI::Backend::ImageInterface* BackendImage;                        //See <GetImageInterface>
            std::vector<std::function<void(ssGUI::ImageData*)>> DataChangedCallbacks;   //See <AddDataChangedCallback>
            std::vector<bool> DataChangedCallbacksValid;                                //See <AddDataChangedCallback>

            ImageData(ImageData const& other);
        
        public:
            ImageData();
            virtual ~ImageData();

            //function: GetImageInterface
            //Gets the pointer of the ImageInterface
            virtual ssGUI::Backend::ImageInterface* GetImageInterface() const;

            //function: IsValid
            //See <ImageInterface::IsValid>
            virtual bool IsValid() const;

            //function: AddDataChangedCallback
            //Adds a callback function when the image data is changed.
            //Note that this can't capture data change if ImageInterface function is called or
            //if the image data is directly modified from the pointer without calling <NotifyImageDataChange>.
            virtual int AddDataChangedCallback(std::function<void(ssGUI::ImageData*)> callback);
            
            //function: RemoveDataChangedCallback
            //Removes the callback function when the image data is changed
            virtual void RemoveDataChangedCallback(int id);

            //function: NotifyImageDataChange
            //Notifies that the image data has changed.
            virtual void NotifyImageDataChange();

            //function: LoadFromPath
            //See <ssGUI::Backend::ImageInterface::LoadFromPath>
            virtual bool LoadFromPath(std::string path);

            //function: LoadImgFileFromMemory
            //See <ssGUI::Backend::ImageInterface::LoadImgFileFromMemory>
            virtual bool LoadImgFileFromMemory(void const * dataPtr, std::size_t size);

            //function: LoadRawFromMemory
            //See <ssGUI::Backend::ImageInterface::LoadRawFromMemory>
            bool LoadRawFromMemory(void const * dataPtr, ssGUI::ImageFormat format, glm::ivec2 imageSize);
            
            //function: GetSize
            //See <ssGUI::Backend::ImageInterface::GetSize>
            virtual glm::ivec2 GetSize() const;

            //TODO: Change this to be returning const pointer
            //function: GetPixelPtr
            //See <ssGUI::Backend::ImageInterface::GetPixelPtr>
            //If there's any modification to the pixels, you need to call <NotifyImageDataChange>
            //in order to notify any image data changes. 
            virtual void* GetPixelPtr(ssGUI::ImageFormat& format) const;

            //function: UpdateCache
            //See <ssGUI::Backend::ImageInterface::UpdateCache>
            virtual void UpdateCache();

            //function: Clone
            //Clones this ImageData object. Use this function instead of assignment operator or copy constructor.
            //If copyCallbacks is true, then all the image data changed callbacks will be copied.
            virtual ImageData* Clone(bool copyCallbacks);
            
            friend std::ostream& operator<<(std::ostream& stream, const ImageData& other);
    };
    
}



#endif