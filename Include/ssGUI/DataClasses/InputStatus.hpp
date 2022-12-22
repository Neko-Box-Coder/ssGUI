#ifndef SSGUI_INPUT_STATUS_H
#define SSGUI_INPUT_STATUS_H

//namespace: ssGUI
namespace ssGUI
{
    class GUIObject;
    //class: ssGUI::InputStatus
    class InputStatus
    {
        public:
            //var: MouseInputBlockedObject
            ssGUI::GUIObject* MouseInputBlockedObject = nullptr;
            
            //var: KeyInputBlockedObject
            ssGUI::GUIObject* KeyInputBlockedObject = nullptr;

            //var: DockingBlockedObject
            //Blocks docking of a window if there's another GUI Object on top of it.
            ssGUI::GUIObject* DockingBlockedObject = nullptr;
    };
}


#endif