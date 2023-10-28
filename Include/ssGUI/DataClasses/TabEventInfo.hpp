#ifndef SSGUI_TAB_EVENT_INFO_HPP
#define SSGUI_TAB_EVENT_INFO_HPP

//namespace: ssGUI
namespace ssGUI
{
    class GUIObject;

    class Tab;

    //struct: ssGUI::TabEventInfo
    //Custom event info for NEW_TAB_CONTENT_ADDED, TAB_CONTENT_SWITCHED and TAB_CONTENT_UNTABBED
    //events
    struct TabEventInfo
    {
            //var: TabAreaContainer
            //Pointer to the GUI Object that has the tab area extension attached
            ssGUI::GUIObject* TabAreaContainer = nullptr;

            //var: Tab
            //Pointer to the tab that is related to the event.
            //For TAB_CONTENT_UNTABBED event, this will be nullptr.
            //For TAB_CONTENT_SWITCHED event, this can be nullptr if no tab is selected.
            ssGUI::Tab* Tab = nullptr;
    };

}

#endif