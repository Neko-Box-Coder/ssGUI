> ⚠️ This library is constantly changing & incomplete until v1.00 . However, feel free to star or bookmark this project.

> ⚠️ Documentation is for the current stable version (v0.93.00.b), not for this version. Some info may not up-to-date.
### 🔌 Status of ssGUI (v0.93.03)
- #### 🔀 Branches
    <!--- ![](https://img.shields.io/badge/Latest_Development-v0.93.03-E5FF3C?style=for-the-badge&logo=GitHub)-->
    - ![](https://img.shields.io/badge/Latest_Development-v0.94.00-brightgreen?style=for-the-badge&logo=GitHub)
        <!--- ![](https://img.shields.io/badge/Status_📋:-Partial_implemented_Win32-2E2E2E?style=flat-square&labelColor=D53434)-->
    - ![](https://img.shields.io/badge/Development_Release-v0.93.03-brightgreen?style=for-the-badge&logo=GitHub)
    - ![](https://img.shields.io/badge/Stable_Release-v0.93.00.b-brightgreen?style=for-the-badge&logo=GitHub)
    - ![](https://img.shields.io/badge/Main-v0.93.03-brightgreen?style=for-the-badge&logo=GitHub)

- #### 🖥️ OS support
    - ![](https://img.shields.io/badge/Linux-Working-brightgreen?style=for-the-badge&logo=linux&logoColor=white)
    - ![](https://img.shields.io/badge/Windows-Working-brightgreen?style=for-the-badge&logo=windows)
    - ![](https://img.shields.io/badge/OSX-Unknown-red?style=for-the-badge&logo=apple)

- #### 📈 Current Progress
    - ![](https://img.shields.io/badge/v0.93-|█████ 100％ █████|-29D236?style=for-the-badge&logo=)
    - ![](https://img.shields.io/badge/v0.94-|█████  70％  ██───|-E5FF3C?style=for-the-badge)
---

### ❓ What is ssGUI?

![](DocsGeneration/ND_Config/Images/Logo.png "Logo")

ssGUI stands for Super Simple GUI. The goal for this library is "Simple to Use, Simple to Customize, Simple to Extend".

This is a verbose library 💬, meaning you have high flexibility on changing different aspects of the GUI objects.

But that doesn't mean you need to go and set every value, most of the default values should work out of the box 📤.

This library can be used for both *graphical intensive* applications 🔥 such as games or 2D/3D applications or *normal* day to day GUI applications 🎹.

Allowing user to build GUI with ease, and the ability to customize it with `ssGUI::Extensions::Extension`.

You can visit the amazing documentation [here](https://neko-box-coder.github.io/ssGUI/)

Currently, ssGUI only supports SFML but it is architectured to also be compatible with other backends. More backend will be added in the future. 

---

### ✨ What makes ssGUI special?

![](DocsGeneration/ND_Config/Images/EasyToUse.png)
**Simple To Use**

![](DocsGeneration/ND_Config/Images/CrossPlatform.png)
**Cross Platform**

![](DocsGeneration/ND_Config/Images/Documentation.png)
**Awesome Documentations**

![](DocsGeneration/ND_Config/Images/Extension.png)
**Customization With Extensions**

![](DocsGeneration/ND_Config/Images/Compatible.png)
**Source Compatible (WIP)**

![](DocsGeneration/ND_Config/Images/Independent.png)
**GUI Code Is Independent From Backend**

![](DocsGeneration/ND_Config/Images/Swap.png)
**Decopuled Backend Architecture**

![](DocsGeneration/ND_Config/Images/Template.png)
**Code Templates For Creating Your Own Widgets, Windows, Extensions And EventCallbacks**

![](DocsGeneration/ND_Config/Images/Builder.png)
**GUI Builder (WIP)**

---

### 📌 Cool Features
#### Window Docking & Auto Layout:
![](DocsGeneration/ND_Config/Images/DockingAndLayout.gif)

#### Mask:
![](DocsGeneration/ND_Config/Images/Mask.gif)

#### Text Alignment:
![](DocsGeneration/ND_Config/Images/TextAlignment.gif)

#### Text Character Level Customization:
![](DocsGeneration/ND_Config/Images/CharAdjust.gif)

#### Extension To Provide Modularity:
![](DocsGeneration/ND_Config/Images/ExtensionExample.gif)

#### And more to come...

---

### GUI Objects List:
- ▶️ Button
- 🔲 Checkbox
- 📷 Image
- 🪟 MainWindow
- 📑 Menu
- 🈶 Text
- ✏️ TextField
- 📦 GUIObject, Widget & Window (Base GUI Objcet)
- 🗄️ Composite GUI Objects
    - 📄 Dropdown
    - 🩻 ImageCanvas
    - 🗃️ MenuItem
    - ↕️ Scrollbar
    - 🛝 Slider
    - ⏩ StandardButton
    - 🖥 StandardWindow

---

### 🧮 Okay, what does it look like in code?
![](DocsGeneration/ND_Config/Images/IntroductionExample.gif) 
```C++
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"

//Readme example
using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;                                               //Create the main window for showing content
    mainWindow.SetRenderSize(glm::vec2(450, 80));
    auto* layout = ssGUI::Factory::Create<ssGUI::Extensions::Layout>();
    mainWindow.AddExtension(layout);                                            //Add layout for auto sizing child GUI objects
    
    ssGUI::Text text;                                                           //Create a text widget and set the respective properties
    text.SetNewCharacterFontSize(17);
    text.SetText("Click on the button to show the message");
    text.SetAlignment(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);  //We center the text right above the button we will be adding later
    text.SetParent(&mainWindow);                                                //Attach text to main window, the layout will control its size.
    
    ssGUI::StandardButton button;                                               //Create a standard button, just a fancier button.
    button.SetSize(glm::vec2(50, 30));
    layout->AddChildWithAlignment(&button,  AlignmentHorizontal::CENTER,        //Attach button to main window with alignment, so that the size
                                            AlignmentVertical::CENTER);         //      stays the same and won't be changed by layout
                                                                                
    ssGUI::ssGUIManager guiManager;                                             //Create the GUIManager, which manages the flow of the program.
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);                    //Add the main window (which has both text and button parented to it)
    guiManager.AddPostGUIUpdateEventListener                                    
    (
        [&]()
        {
            if(button.GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)   //Then we want to check if the button is pressed every frame
                text.SetText(L"(`oωo´)");                                       //If it is, we change the text to a cute little face :)
        }
    );
    guiManager.StartRunning();                                                  //Finally we start running the program
    return 0;
}
```

---

### 🛣️ [Roadmap](https://ssGUI.nekoboxcoder.dev/b/ckQsLZToXDPFzsAzx/ssgui)
![](DocsGeneration/ND_Config/Images/Roadmap.png)

---

### ❤️ Special Thanks

#### SFML With Awesome Documentation:

![](DocsGeneration/ND_Config/Images/SFML.png)

#### GLM With Great Math Structs And Math Functions: 

![](DocsGeneration/ND_Config/Images/Glm.png)

#### Natural Docs With Amazing And Intuative Documentation Generation::

![](DocsGeneration/ND_Config/Images/NaturalDocs.svg) 

---

### 📜 Licenses
This project is under Apache-2.0 License. 
Licenses for resoureces used by testing and examples are stored as "Credits.txt" under the same folder.

GLM:
- The Happy Bunny License or MIT License

SFML:
- zlib/png license
- Components used by ssGUI:
    - freetype is under the FreeType license or the GPL license
    - stb_image and stb_image_write are public domain
- If you want to use other components, please visit https://github.com/SFML/SFML/blob/master/license.md for their licenses


