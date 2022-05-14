#ifndef SSGUI_INPUT_STATUS
#define SSGUI_INPUT_STATUS

//namespace: ssGUI
namespace ssGUI
{
    //class: ssGUI::InputStatus
    class InputStatus
    {
        public:
            //var: MouseInputBlocked
            bool MouseInputBlocked = false;
            
            //var: KeyInputBlocked
            bool KeyInputBlocked = false;

            //var: DockingBlocked
            //Blocks docking of a window if there's another GUI Object on top of it.
            bool DockingBlocked = false;
    };
}


#endif