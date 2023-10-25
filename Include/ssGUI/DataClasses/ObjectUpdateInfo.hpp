#ifndef SSGUI_OBJECT_UPDATE_INFO_HPP
#define SSGUI_OBJECT_UPDATE_INFO_HPP

//namespace: ssGUI
namespace ssGUI
{
    namespace Backend
    {
        class BackendSystemInputInterface;
    }

    class GUIObject;
    class InputStatus;

    //struct: ssGUI::ObjectUpdateInfo
    //Custom event info for OBJECT_UPDATED and BEFORE_OBJECT_UPDATE events
    struct ObjectUpdateInfo
    {
        public:
            //var: InputInterface
            //Pointer to the input interface
            ssGUI::Backend::BackendSystemInputInterface* InputInterface;

            //var: InputStatus
            //Input status for current frame
            ssGUI::InputStatus& CurrentInputStatus;

            //var: LastInputStatus
            //Input status from last frame
            ssGUI::InputStatus& LastInputStatus;

            //var: MainWindow
            //Pointer to the main window
            ssGUI::GUIObject* MainWindow;
    };

}

#endif