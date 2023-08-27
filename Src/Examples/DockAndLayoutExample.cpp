
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/Extensions/Dockable.hpp"

#include <tuple>

//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 650));

    ssGUI::Window window;
    // window.SetTitlebarHeight(25);
    window.SetSize(glm::vec2(300, 600));
    window.SetParent(&mainWindow);
    // window.AddExtension(new ssGUI::Extensions::Layout());
    // window.RemoveExtension(ssGUI::Extensions::Dockable::EXTENSION_NAME);
    window.AddExtension<ssGUI::Extensions::Docker>();
    // window.GetExtension<ssGUI::Extensions::Layout>()->SetPadding(5);
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));
    
    ssGUI::StandardWindow window2;
    window2.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    ssGUI::StandardWindow window3;
    window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window3.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window3.AddExtension<ssGUI::Extensions::Layout>();
    window3.AddChild<ssGUI::StandardSlider>();
    window3.AddChild<ssGUI::StandardSlider>();
    
    auto* dummyHolder = window3.AddChild<ssGUI::GUIObject>();
    dummyHolder->AddExtension<ssGUI::Extensions::Layout>()->SetHorizontalLayout(true);
    dummyHolder->AddChild<ssGUI::Text>()->SetText("Test");
    dummyHolder->AddChild<ssGUI::TextField>();
    
    dummyHolder = window3.AddChild<ssGUI::GUIObject>();
    dummyHolder->AddExtension<ssGUI::Extensions::Layout>()->SetHorizontalLayout(true);
    dummyHolder->AddChild<ssGUI::Text>()->SetText("Test2");
    dummyHolder->AddChild<ssGUI::Checkbox>();
    
    
    ssGUI::StandardWindow window4;
    window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
    window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));

    ssGUI::StandardWindow window5;
    // window5.SetTitlebarColor(glm::u8vec4(127, 127, 127, 255));
    window5.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));


    window2.SetParent(&window);
    window3.SetParent(&window);
    window4.SetParent(&window);
    window5.SetParent(&window);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    
    guiManager.AddPostGUIUpdateEventListener
    (
        [&mainWindow, &guiManager]()
        {
            if(guiManager.IsButtonOrKeyDown(ssGUI::Enums::SystemKey::ENTER))
            {
                //TODO(NOW): Move to a function to debug
                using depth = int;
                using lastSpaces = std::string;

                std::list<std::tuple<ssGUI::GUIObject*, depth, lastSpaces>> childrenToPrint;
                childrenToPrint.push_back(std::make_tuple(&mainWindow, 0, ""));
                
                while(!childrenToPrint.empty()) 
                {
                    ssGUI::GUIObject* curObj = std::get<0>(childrenToPrint.front());
                    int curDepth = std::get<1>(childrenToPrint.front());
                    std::string lastSpaces = std::get<2>(childrenToPrint.front());
                    childrenToPrint.pop_front();
                    
                    bool hasNextWithSameDepth = childrenToPrint.empty() ? false : (std::get<1>(childrenToPrint.front()) == curDepth);
                    
                    ssLOG_LINE(lastSpaces << "|   ");
                    ssLOG_LINE(lastSpaces << "|---" << "GUI Object: " << curObj);
                    ssLOG_LINE(lastSpaces << (hasNextWithSameDepth ? "|   " : "    " ) << "Type: " << ssGUI::Enums::GUIObjectTypeToString(curObj->GetType())); 
                    
                    //Add the children to the list
                    std::vector<ssGUI::GUIObject*> curChildren = curObj->GetListOfChildren();
                    
                    std::string nextSpaces = lastSpaces;
                    if(!childrenToPrint.empty() && hasNextWithSameDepth)
                        nextSpaces += "|   ";
                    else
                        nextSpaces += "    ";
                    
                    for(int i = curChildren.size() - 1; i >= 0; --i)
                        childrenToPrint.push_front(std::make_tuple(curChildren.at(i), curDepth + 1, nextSpaces)); 
                }
            }
        }
    );
    
    guiManager.StartRunning();
    return 0;
}
