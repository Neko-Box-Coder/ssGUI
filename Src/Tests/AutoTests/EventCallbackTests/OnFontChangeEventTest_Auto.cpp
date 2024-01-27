#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "TestsResources.h"

#include <fstream>

ssGUI::EventCallback* Callback = nullptr;
ssGUI::Text* TextObj = nullptr;
ssGUI::Font* CustomFont = nullptr;
int ListenerNum = 0;

int main()
{
    std::fstream checkExistFs;
    checkExistFs.open("./arial.ttf");
    if(checkExistFs.fail())
    {
        std::ofstream ofs("./arial.ttf", std::ofstream::binary | std::ofstream::out);
        ofs.write((char*)ssGUI_Test_arial, ssGUI_Test_arial_size);
        ofs.close();
    }
    else
        checkExistFs.close();

    ssTEST_INIT();

    ssTEST_SET_UP
    {
        Callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        Callback->SetEventType(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE);
        TextObj = ssGUI::Factory::Create<ssGUI::Text>();
        CustomFont = ssGUI::Factory::Create<ssGUI::Font>();
        CustomFont->GetFontInterface()->LoadFromPath("./arial.ttf");
        
        
        //Timing is making sure the listener is triggered **before** the event
        Callback->AddEventListener( "key", [&](ssGUI::EventInfo& info)
                                    {
                                        if(ListenerNum < 1)
                                        {
                                            ssTEST_OUTPUT_ASSERT("Timing", TextObj->GetFontsCount() == 0);
                                        }
                                        
                                        ListenerNum += 1;
                                    });

        TextObj->AddEventCallbackCopy(Callback, true);
    };

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(Callback);
        ssGUI::Factory::Dispose(TextObj);
        ssGUI::Factory::Dispose(CustomFont);
    };

    ssTEST("EventTest")
    {
        //Need to make sure by default it has no font and 1 default font
        ssTEST_OUTPUT_ASSERT("Default count for font", TextObj->GetFontsCount() == 0);   
        ssTEST_OUTPUT_ASSERT("Default count for default font", TextObj->GetDefaultFontsCount() == 1);   

        TextObj->AddFont(CustomFont);
        ssTEST_OUTPUT_ASSERT("Text font", TextObj->GetFontsCount() == 1 && ListenerNum == 1);
        
        ssGUI::Text::AddDefaultFont()->GetFontInterface()->LoadFromPath("./arial.ttf");
        ssTEST_OUTPUT_ASSERT("Default font not triggering event", TextObj->GetDefaultFontsCount() == 2 && ListenerNum == 1);
    };

    ssTEST_END();
}