#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/ImageCanvas.hpp"
#include "ssGUI/Extensions/Border.hpp"

#include "ssLogger/ssLog.hpp"


//Image Test
int main()
{
    ssGUI::MainWindow mainWindow;
    
    //Creating an image widget
    ssGUI::ImageCanvas imageCanvas;
    imageCanvas.SetSize(glm::vec2(400, 200));
    imageCanvas.SetPosition(glm::vec2(200, 200));
    imageCanvas.SetBackgroundColor(glm::u8vec4(128, 128, 128, 255));
    imageCanvas.SetViewportRotation(20);
    imageCanvas.SetViewportZoom(0.5);
    imageCanvas.SetViewportCenterPosition(glm::vec2(0, 1));

    // imageCanvas.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
    // image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);
    // image.SetImageTint(glm::u8vec4(255, 148, 148, 255));

    ssGUI::ImageData data;
    data.LoadFromPath("Resources/sd.png");
    imageCanvas.SetImageData(&data);
    imageCanvas.SetParent(&mainWindow);
    imageCanvas.SetImageTint(glm::u8vec4(255, 255, 255, 255));
    // imageCanvas.GetHorizontalScrollbar()->SetReverse(!imageCanvas.GetHorizontalScrollbar()->IsReverse());
    // imageCanvas.GetVerticalScrollbar()->SetReverse(!imageCanvas.GetVerticalScrollbar()->IsReverse());
    
    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            // if(imageCanvas.GetHorizontalScrollbar()->IsFocused())
            //     imageCanvas.GetHorizontalScrollbar()->SetBackgroundColor(glm::u8vec4(0, 255, 0, 255));
            // else
            //     imageCanvas.GetHorizontalScrollbar()->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));

            // if(imageCanvas.GetVerticalScrollbar()->IsFocused())
            //     imageCanvas.GetVerticalScrollbar()->SetBackgroundColor(glm::u8vec4(0, 255, 0, 255));
            // else
            //     imageCanvas.GetVerticalScrollbar()->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));

            // if(imageCanvas.IsFocused())
            //     imageCanvas.SetBackgroundColor(glm::u8vec4(0, 255, 0, 255));
            // else
            //     imageCanvas.SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));

            if(guiManager.GetBackendInputInterface()->IsInputExistCurrentFrame(ssGUI::Enums::LetterKey::U))
            {
                auto uv = imageCanvas.GetUVFromGlobalPosition(guiManager.GetBackendInputInterface()->GetCurrentMousePosition(&mainWindow));
                ssLOG_LINE("uv: "<<uv.x<<", "<<uv.y);
            }

            if(guiManager.GetBackendInputInterface()->IsInputExistCurrentFrame(ssGUI::Enums::LetterKey::G))
            {
                auto g = imageCanvas.GetGlobalPositionFromUV(glm::vec2(20, 20)) - imageCanvas.GetGlobalPosition();
                auto g2 = imageCanvas.GetGlobalPositionFromUV(glm::vec2(270, 20)) - imageCanvas.GetGlobalPosition();
                auto g3 = imageCanvas.GetGlobalPositionFromUV(glm::vec2(265, 270)) - imageCanvas.GetGlobalPosition();
                auto g4 = imageCanvas.GetGlobalPositionFromUV(glm::vec2(20, 280)) - imageCanvas.GetGlobalPosition();
                ssLOG_LINE("g: "<<g.x<<", "<<g.y);
                ssLOG_LINE("g2: "<<g2.x<<", "<<g2.y);
                ssLOG_LINE("g3: "<<g3.x<<", "<<g3.y);
                ssLOG_LINE("g4: "<<g4.x<<", "<<g4.y);
            }
        }
    );

    guiManager.StartRunning();
    return 0;
}