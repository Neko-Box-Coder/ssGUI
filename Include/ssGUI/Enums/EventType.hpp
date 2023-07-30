#ifndef SSGUI_EVENT_TYPE_H
#define SSGUI_EVENT_TYPE_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <cstdint>
#include <string>

//namespace: ssGUI
namespace ssGUI 
{
    //namespace: ssGUI::Enums
    namespace Enums
    {
        /*  enum: EventType
        
        NONE                                - Default event type
        BACKGROUND_COLOR_CHANGED            - Triggered *after* the background color is changed. Container will be the source for triggering this event callback.
        
        BEFORE_FONT_CHANGE                  - Triggered *before* the font of a text is changed. Text(Container) will be the source for triggering this event callback. 
                                                Please note that this is not triggered by any changes to default fonts.

        BEFORE_OBJECT_DESTROY               - Triggered *before* the GUI object is destroyed. Container will be the source for triggering this event callback.
        BEFORE_OBJECT_RENDER                - Triggered *before* this GUI Object is drawing to the MainWindow. MainWindow will be the source for triggering this event callback.
                                                You can call <ssGUI::Renderer::IsRedrawNeeded> to determine if cache is being drawn or not.
        
        BEFORE_RECURSIVE_CHILD_ADD          - Triggered *before* a child is paranted recursively on this GUI object. The child object being added will be the source for triggering this event callback.
        BEFORE_RECURSIVE_CHILD_REMOVE       - Triggered *before* a child is removed recursively on this GUI object. The child object being removed will be the source for triggering this event callback.
        BEFORE_WINDOW_CLOSE                 - Triggered *before* this GUI Object (Window) is closed. Window(Container) will be the source for triggering this event callback.
        BEFORE_OBJECT_UPDATE                - Triggered *before* this GUI object is being updated. Container will be the source for triggering this event callback.
                                                <ssGUI::EventInfo::CustomInfo> will be pointer to <ssGUI::ObjectUpdateInfo> struct.
        
        BUTTON_STATE_CHANGED                - Triggered *after* the button state has changed. Button(Container) will be the source for triggering this event callback.
        CHILD_ADDED                         - Triggered *after* a child is paranted on this GUI object. The child object being added will be the source for triggering this event callback.
        CHILD_POSITION_CHANGED              - Triggered *after* <ssGUI::Hierarchy::ChangeChildOrderToBeforePosition> or <ssGUI::Hierarchy::ChangeChildOrderToAfterPosition> is called.
                                                The child that is passed to <ssGUI::Hierarchy::ChangeChildOrderToBeforePosition> or <ssGUI::Hierarchy::ChangeChildOrderToAfterPosition> will be the source 
                                                for triggering this event callback.
                                                
                                                The event will only be triggered once as a result of the ChangeChildOrderToXXXPosition function
                                                and will not be triggered by the change of position for other children as a result of this function call.
                                                
                                                Meaning only the child GUI Object that is being **actively** changed position will have the event triggered
                                                (i.e. the child that is passed to the function for changing the position).
        CHILD_REMOVED                       - Triggered *after* a child is removed on this GUI object. The child object being removed will be the source for triggering this event callback.
        FOCUSED                             - Triggered *after* the GUI Object gained focus
        FOCUS_LOST                          - Triggered *after* the GUI Object lost focus
        ITEM_SELECTED                       - Triggered *after* an item is selected. Dropdown(Container) will be the source for triggering this event callback.
        MIN_MAX_SIZE_CHANGED                - Triggered *after* min or max size of a GUI object has changed. Container will be the source for triggering this event callback.
        OBJECT_RENDERED                     - Triggered *after* this GUI Object is drawing to the MainWindow. MainWindow will be the source for triggering this event callback.
                                                You can call <ssGUI::Renderer::IsRedrawNeeded> to determine if cache is being drawn or not

        RECURSIVE_CHILD_ADDED               - Triggered *after* a child is paranted recursively on this GUI object. The child object being added will be the source for triggering this event callback.
        RECURSIVE_CHILD_REMOVED             - Triggered *after* a child is removed recursively on this GUI object. The child object being removed will be the source for triggering this event callback.
        SIZE_CHANGED                        - Triggered *after* this GUI object's size has changed. Container will be the source for triggering this event callback.
        SLIDER_VALUE_CHANGED                - Triggered *shortly after* the value of the slider is changed. This is triggered under the <ssGUI::Slider::MainLogic> method therefore the event is not immediate.
        SLIDER_VALUE_CHANGED_VIA_GUI        - Triggered *shortly after* the value of the slider is changed. This is triggered under the <ssGUI::Slider::MainLogic> method therefore the event is not immediate.
        SLIDER_VALUE_FINISHED_CHANGING      - Triggered *shortly after* the value of the slider has *finished* changing.
                                                Meaning this will be triggered if the slider value stayed the same for current update
                                                *and* the previous update it has been changed.
                                                
                                                This is triggered under the <ssGUI::Slider::MainLogic> method therefore the event is not immediate.
        WINDOW_DRAG_STATE_CHANGED           - Triggered *after* this GUI object (window) drag state is changed. Window(Container) will be the source for triggering this event callback.
        OBJECT_UPDATED                      - Triggered *after* this GUI object is being updated. Container will be the source for triggering this event callback.
                                                <ssGUI::EventInfo::CustomInfo> will be pointer to <ssGUI::ObjectUpdateInfo> struct.
        TEXT_CONTENT_CHANGED                - Triggered *shortly after* there's a text content change (Not visual change such as underline or bold). 
                                                The text GUI object that has the text being changed will be the source for triggering this event callback.
        COUNT                               - Count
        
        SCROLLBAR_VALUE_CHANGED             - Same as <SLIDER_VALUE_CHANGED>
        SCROLLBAR_VALUE_CHANGED_VIA_GUI     - Same as <SLIDER_VALUE_CHANGED_VIA_GUI>
        SCROLLBAR_VALUE_FINISHED_CHANGING   - Same as <SLIDER_VALUE_FINISHED_CHANGING>
        */
        enum class EventType : uint16_t
        {
            NONE,
            BACKGROUND_COLOR_CHANGED,
            
            BEFORE_FONT_CHANGE,
            BEFORE_OBJECT_DESTROY,
            BEFORE_OBJECT_RENDER,
            BEFORE_RECURSIVE_CHILD_ADD,
            BEFORE_RECURSIVE_CHILD_REMOVE,
            BEFORE_WINDOW_CLOSE,
            BEFORE_OBJECT_UPDATE,
            
            BUTTON_STATE_CHANGED,
            CHILD_ADDED,
            CHILD_POSITION_CHANGED,
            CHILD_REMOVED,
            FOCUSED,
            FOCUS_LOST,
            ITEM_SELECTED,
            MIN_MAX_SIZE_CHANGED,
            OBJECT_RENDERED,
            TEXT_CONTENT_CHANGED,
            RECURSIVE_CHILD_ADDED,
            RECURSIVE_CHILD_REMOVED,
            SIZE_CHANGED,
            SLIDER_VALUE_CHANGED,
            SLIDER_VALUE_CHANGED_VIA_GUI,
            SLIDER_VALUE_FINISHED_CHANGING,
            WINDOW_DRAG_STATE_CHANGED,
            OBJECT_UPDATED,
            
            COUNT,
            
            SCROLLBAR_VALUE_CHANGED = SLIDER_VALUE_CHANGED,
            SCROLLBAR_VALUE_CHANGED_VIA_GUI = SLIDER_VALUE_CHANGED_VIA_GUI,
            SCROLLBAR_VALUE_FINISHED_CHANGING = SLIDER_VALUE_FINISHED_CHANGING,
        };
        
        //function: EventTypeToString
        inline std::string EventTypeToString(EventType event)
        {
            static_assert((int)EventType::COUNT == 27, "ToString");
            switch(event)
            {
                RETURN_ENUM_STRING(EventType::NONE);
                RETURN_ENUM_STRING(EventType::BACKGROUND_COLOR_CHANGED);

                RETURN_ENUM_STRING(EventType::BEFORE_FONT_CHANGE);
                RETURN_ENUM_STRING(EventType::BEFORE_OBJECT_DESTROY);
                RETURN_ENUM_STRING(EventType::BEFORE_OBJECT_RENDER);
                RETURN_ENUM_STRING(EventType::BEFORE_RECURSIVE_CHILD_ADD);
                RETURN_ENUM_STRING(EventType::BEFORE_RECURSIVE_CHILD_REMOVE);
                RETURN_ENUM_STRING(EventType::BEFORE_WINDOW_CLOSE);
                RETURN_ENUM_STRING(EventType::BEFORE_OBJECT_UPDATE);
                
                RETURN_ENUM_STRING(EventType::BUTTON_STATE_CHANGED);
                RETURN_ENUM_STRING(EventType::CHILD_ADDED);
                RETURN_ENUM_STRING(EventType::CHILD_POSITION_CHANGED);
                RETURN_ENUM_STRING(EventType::CHILD_REMOVED);
                RETURN_ENUM_STRING(EventType::FOCUSED);
                RETURN_ENUM_STRING(EventType::FOCUS_LOST);
                RETURN_ENUM_STRING(EventType::ITEM_SELECTED);
                RETURN_ENUM_STRING(EventType::MIN_MAX_SIZE_CHANGED);
                RETURN_ENUM_STRING(EventType::OBJECT_RENDERED);
                RETURN_ENUM_STRING(EventType::TEXT_CONTENT_CHANGED);
                RETURN_ENUM_STRING(EventType::RECURSIVE_CHILD_ADDED);
                RETURN_ENUM_STRING(EventType::RECURSIVE_CHILD_REMOVED);
                RETURN_ENUM_STRING(EventType::SIZE_CHANGED);
                RETURN_ENUM_STRING(EventType::SLIDER_VALUE_CHANGED);
                RETURN_ENUM_STRING(EventType::SLIDER_VALUE_CHANGED_VIA_GUI);
                RETURN_ENUM_STRING(EventType::SLIDER_VALUE_FINISHED_CHANGING);
                RETURN_ENUM_STRING(EventType::WINDOW_DRAG_STATE_CHANGED);
                RETURN_ENUM_STRING(EventType::OBJECT_UPDATED);
                
                RETURN_ENUM_STRING(EventType::COUNT);
            }
            
            return "";
        }
    }
}

#endif