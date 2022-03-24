#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"


//Rounded corners test 2
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    mainWindow.GetBackendWindowInterface()->SetMSAA(8);
    DEBUG_LINE(mainWindow.GetBackendWindowInterface()->GetMSAA());

    //Creating an image widget
    ssGUI::Image image;
    image.SetPosition(glm::ivec2(100, 100));
    image.SetSize(glm::ivec2(200, 300));
    image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);

    ssGUI::ImageData data;
    data.LoadFromPath("sd.png");
    image.SetImageData(&data);
    image.AddExtension(new ssGUI::Extensions::RoundedCorners());
    static_cast<ssGUI::Extensions::RoundedCorners*>
        (image.GetExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME))->ClearTargetShapes();
    
    static_cast<ssGUI::Extensions::RoundedCorners*>
        (image.GetExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME))->SetRoundedCornersRadius(40);

    // static_cast<ssGUI::Extensions::RoundedCorners*>
    //     (image.GetExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME))->AddTargetShape(1);

    static_cast<ssGUI::Extensions::RoundedCorners*>
        (image.GetExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME))->AddTargetVertex(4);

    static_cast<ssGUI::Extensions::RoundedCorners*>
        (image.GetExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME))->AddTargetVertex(6);

    image.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
