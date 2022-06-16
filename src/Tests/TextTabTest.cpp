#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Text font test
int main()
{
    ssGUI::MainWindow mainWindow;


    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(300, 150));

    text.SetFontSize(20);
    text.SetWrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING);

    text.SetText("i\tiiiii\tiii\tiii\ni\ti\ti\ti\ti\ti\ti");

    text.SetParent(&mainWindow);

    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
