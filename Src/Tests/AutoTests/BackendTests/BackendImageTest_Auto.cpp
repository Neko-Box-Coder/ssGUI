#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Factory.hpp"
#include "ssTest.hpp"

#include <fstream>
#include <thread>

ssGUI::Backend::BackendImageInterface* TestImage = nullptr;
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\..\\..\\Resources\\";
#else
    std::string ResourcesFolderPath = "../../Resources/";
#endif

ssTEST_INIT();

ssTEST_SET_UP
{
    TestImage = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(TestImage);
}

ssTEST("GetRawHandleTest")
{
    #ifdef SSGUI_IMAGE_BACKEND_STB_IMAGE
        ssTEST_OUTPUT_SKIP();
    #else
        ssTEST_OUTPUT_ASSERT(TestImage->GetRawHandle() != nullptr);
    #endif
}

ssTEST("IsValidTest")
{
    ssTEST_OUTPUT_ASSERT(!TestImage->IsValid());
    ssTEST_OUTPUT_ASSERT(TestImage->LoadFromPath(ResourcesFolderPath+"sd.png"));
}

ssTEST("LoadFromPathTest")
{
    ssTEST_OUTPUT_ASSERT("Loading", TestImage->LoadFromPath(ResourcesFolderPath+"sd.png"));
    ssTEST_OUTPUT_ASSERT("Validation", TestImage->GetSize().x == 293 && TestImage->GetSize().y == 293);
}

ssTEST("LoadImgFileFromMemoryTest")
{
    std::ifstream ifd(ResourcesFolderPath+"WindowIcon.png", std::ios::binary | std::ios::ate);
    size_t size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);
    
    char* buffer = new char[size];
    ifd.read(buffer, size);
    ifd.close();
    
    ssTEST_OUTPUT_ASSERT("Loading", TestImage->LoadImgFileFromMemory(buffer, size));
    ssTEST_OUTPUT_ASSERT("Validation", TestImage->GetSize().x == 48 && TestImage->GetSize().y == 48);
    
    delete[] buffer;
};

//This is retarded how MSVC not evaluating this as constant
#ifndef _MSC_VER
    const int imgWidth = 64;
    const int imgHeight = 64;
#else
    #define imgWidth 64
    #define imgHeight 64
#endif
const int columnWidth = imgWidth / 4;

ssTEST("LoadRawFromMemoryTest")
{
    uint8_t dummyImg[imgWidth * imgHeight * 4] = {0};
    
    for(int y = 0; y < imgHeight; y++)
    {
        for(int x = 0; x < imgWidth; x++)
        {
            dummyImg[(y * imgWidth + x) * 4 + 0] = x < columnWidth ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 1] = x >= columnWidth && x < columnWidth * 2 ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 2] = x >= columnWidth * 2 && x < columnWidth * 3 ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 3] = x >= columnWidth * 3 ? 127 : 255;
        }
    }
    
    TestImage->LoadRawFromMemory(dummyImg, ssGUI::ImageFormat(), glm::ivec2(64, 64));
    
    ssTEST_OUTPUT_ASSERT(TestImage->IsValid());
}

ssTEST("GetSizeTest")
{
    ssTEST_OUTPUT_ASSERT(TestImage->GetSize().x == imgWidth && TestImage->GetSize().y == imgHeight);
}

ssTEST("GetPixelPtrTest")
{
    ssGUI::ImageFormat format;
    uint8_t* pixelPtr = static_cast<uint8_t*>(TestImage->GetPixelPtr(format));
    
    ssTEST_OUTPUT_ASSERT("Format Valid",
                            format.BitDepthPerChannel == 8 &&
                            format.ImgType == ssGUI::Enums::ImageType::RGB &&
                            format.HasAlpha == true &&
                            format.IndexR == 0 &&
                            format.IndexG == 1 &&
                            format.IndexB == 2 &&
                            format.IndexA == 3);

    ssTEST_OUTPUT_ASSERT("Pixel Valid",
                            pixelPtr != nullptr &&
                            //Red
                            pixelPtr[0 * 4 + 0] == 255 &&
                            pixelPtr[0 * 4 + 1] == 0 &&
                            pixelPtr[0 * 4 + 2] == 0 &&
                            pixelPtr[0 * 4 + 3] == 255 &&
                            
                            //Green
                            pixelPtr[columnWidth * 4 + 0] == 0 &&
                            pixelPtr[columnWidth * 4 + 1] == 255 &&
                            pixelPtr[columnWidth * 4 + 2] == 0 &&
                            pixelPtr[columnWidth * 4 + 3] == 255 &&
                            
                            //Blue
                            pixelPtr[columnWidth*2 * 4 + 0] == 0 &&
                            pixelPtr[columnWidth*2 * 4 + 1] == 0 &&
                            pixelPtr[columnWidth*2 * 4 + 2] == 255 &&
                            pixelPtr[columnWidth*2 * 4 + 3] == 255 &&
                            
                            //Green
                            pixelPtr[columnWidth*3 * 4 + 0] == 0 &&
                            pixelPtr[columnWidth*3 * 4 + 1] == 0 &&
                            pixelPtr[columnWidth*3 * 4 + 2] == 0 &&
                            pixelPtr[columnWidth*3 * 4 + 3] == 127);
}

ssTEST("CloneTest")
{
    ssGUI::Backend::BackendImageInterface* clonedImg = TestImage->Clone();
    
    ssTEST_OUTPUT_ASSERT(   clonedImg != nullptr &&
                            clonedImg->GetSize().x == TestImage->GetSize().x &&
                            clonedImg->GetSize().y == TestImage->GetSize().y);

    ssGUI::Factory::Dispose(clonedImg);
}

ssTEST_END();
