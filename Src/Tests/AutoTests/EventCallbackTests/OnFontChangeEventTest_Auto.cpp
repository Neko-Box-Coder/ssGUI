#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

ssGUI::EventCallback* Callback = nullptr;
ssGUI::Text* TextObj = nullptr;
ssGUI::Font* CustomFont = nullptr;
int ListenerNum = 0;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        Callback = ssGUI::Factory::Create<ssGUI::EventCallback>();
        Callback->SetEventType(ssGUI::Enums::EventType::BEFORE_FONT_CHANGE);
        TextObj = ssGUI::Factory::Create<ssGUI::Text>();
        CustomFont = ssGUI::Factory::Create<ssGUI::Font>();
        CustomFont->GetBackendFontInterface()->LoadFromPath(ResourcesFolderPath+"arial.ttf");
        
        
        //Timing is making sure the listener is triggered **before** the event
        Callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                    {
                                        if(ListenerNum < 1)
                                        {
                                            ssTEST_OUTPUT_ASSERT("Timing", TextObj->GetFontsCount() == 0);
                                        }
                                        
                                        ListenerNum += 1;
                                    });

        TextObj->AddEventCallbackCopy(Callback, true);
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(Callback);
        ssGUI::Factory::Dispose(TextObj);
        ssGUI::Factory::Dispose(CustomFont);
        ssGUI::CleanUpDefaultResources();
    }

    ssTEST("EventTest")
    {
        //Need to make sure by default it has no font and 1 default font
        ssTEST_OUTPUT_ASSERT("Default count for font", TextObj->GetFontsCount() == 0);   
        ssTEST_OUTPUT_ASSERT("Default count for default font", TextObj->GetDefaultFontsCount() == 1);   

        TextObj->AddFont(CustomFont);
        ssTEST_OUTPUT_ASSERT("Text font", TextObj->GetFontsCount() == 1 && ListenerNum == 1);
        
        ssGUI::Text::AddDefaultFont()->GetBackendFontInterface()->LoadFromPath(ResourcesFolderPath+"arial.ttf");
        ssTEST_OUTPUT_ASSERT("Default font not triggering event", TextObj->GetDefaultFontsCount() == 2 && ListenerNum == 1);
    }

    ssTEST_END();
}