#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssLogger/ssLog.hpp"


//Rounded corners test 2
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    mainWindow.GetBackendWindowInterface()->SetMSAA(8);
    ssLOG_LINE(mainWindow.GetBackendWindowInterface()->GetAntiAliasingLevel());

    //Creating an image widget
    ssGUI::Image image;
    image.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    image.SetPosition(glm::ivec2(100, 100));
    image.SetSize(glm::ivec2(200, 300));
    image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);

    ssGUI::ImageData data;
    data.LoadFromPath("Resources/sd.png");
    image.SetImageData(&data);
    auto roundedCornersEx = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
    roundedCornersEx->ClearTargetShapes();
    
    roundedCornersEx->SetRoundedCornersRadius(40);

    // roundedCornersEx->AddTargetShape(1);

    roundedCornersEx->AddTargetVertex(4);

    roundedCornersEx->AddTargetVertex(6);

    image.AddExtension(roundedCornersEx);
    image.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
