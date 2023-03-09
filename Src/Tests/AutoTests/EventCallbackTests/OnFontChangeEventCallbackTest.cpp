#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/EventCallbacks/OnFontChangeEventCallback.hpp"

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

ssGUI::EventCallbacks::OnFontChangeEventCallback* callback = nullptr;
ssGUI::Text* textObj = nullptr;
ssGUI::Font* customFont = nullptr;
int listerNum = 0;

ssTEST_INIT();

ssTEST_SET_UP
{
    callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::OnFontChangeEventCallback>();
    textObj = ssGUI::Factory::Create<ssGUI::Text>();
    customFont = ssGUI::Factory::Create<ssGUI::Font>();
    customFont->GetBackendFontInterface()->LoadFromPath(ResourcesFolderPath+"arial.ttf");
    
    
    //Timing is making sure the listener is triggered **before** the event
    callback->AddEventListener( "key", [&](ssGUI::EventInfo info)
                                {
                                    if(listerNum < 1)
                                    {
                                        ssTEST_OUTPUT_ASSERT("Timing", textObj->GetFontsCount() == 0);
                                    }
                                    
                                    listerNum += 1;
                                });

    callback->Clone(textObj, true);
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(callback);
    ssGUI::Factory::Dispose(textObj);
    ssGUI::Factory::Dispose(customFont);
    ssGUI::CleanUpDefaultResources();
}

ssTEST("GetEventCallbackNameTest")
{
    ssTEST_OUTPUT_ASSERT(callback->GetEventCallbackName() == "OnFontChangeEvent");
}

ssTEST("EventTest")
{
    //Need to make sure by default it has no font and 1 default font
    ssTEST_OUTPUT_ASSERT("Default count for font", textObj->GetFontsCount() == 0);   
    ssTEST_OUTPUT_ASSERT("Default count for default font", textObj->GetDefaultFontsCount() == 1);   

    textObj->AddFont(customFont);
    ssTEST_OUTPUT_ASSERT("Text font", textObj->GetFontsCount() == 1 && listerNum == 1);
    
    ssGUI::Text::AddDefaultFont()->GetBackendFontInterface()->LoadFromPath(ResourcesFolderPath+"arial.ttf");
    ssTEST_OUTPUT_ASSERT("Default font not triggering event", textObj->GetDefaultFontsCount() == 2 && listerNum == 1);
}

ssTEST_END();