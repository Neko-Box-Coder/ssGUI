> ⚠️ This library is constantly changing & incomplete until v1.00 . However, feel free to star or bookmark this project.

### 🔌 Status of ssGUI
- #### 🔀 Branches
    - Master: ✅

- #### 🖥️ OS support
    - Linux: Working ✅
    - Windows: Broken ❌
    - MacOS: Unknown ❓

- #### 📈 Current Status
    - ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ *v0.93 (100%)* 
    - ⬛⬛🔲🔲🔲🔲🔲🔲🔲🔲 *v0.94 (20%)*

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
**Cross Platform (Depends on Backend)**

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

//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 110));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetSize(glm::vec2(450, 45));
    text.SetNewCharacterFontSize(17);
    text.SetText("Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);

    //Create a button
    ssGUI::StandardButton button;
    button.SetSize(glm::vec2(40, 30));
    button.SetPosition(glm::vec2(205, 60));

    //Set the parents
    button.SetParent(&mainWindow);
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            if(button.GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                text.SetText(L"(`oωo´)");
        }
    );
    guiManager.StartRunning();
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


