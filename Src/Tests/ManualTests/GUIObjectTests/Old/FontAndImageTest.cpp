
#include "ssGUI/DataClasses/Font.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssLogger/ssLog.hpp"

#include <fstream>

int main()
{

    ssGUI::Font f;
    if(!f.GetFontInterface()->LoadFromPath("./Resources/NotoSans-Regular.ttf"))
        return -1;
    
    
    ssGUI::ImageData imgData;
    if(!f.GetFontInterface()->GetCharacterImage(L'B', 200, imgData))
        return -1;
    
    std::ofstream outputStream("./output.fontImage", std::ios::out | std::ios::binary);
    ssGUI::ImageFormat imgFmt;
    ssLOG_LINE("imgData.GetSize().x: "<<imgData.GetSize().x);
    ssLOG_LINE("imgData.GetSize().y: "<<imgData.GetSize().y);
    outputStream.write(reinterpret_cast<char*>(imgData.GetPixelPtr(imgFmt)), imgData.GetSize().y * imgData.GetSize().x * 4);
    
    if(!outputStream.good())
    {
        std::cout << "Failed to output\n";
        return -1;
    }


    return 0;
}