
/*
#include <iostream>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ProjectConfig.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/ssGUIManager.hpp"
#include "ssGUI/DataClasses/KeyPresses.hpp"
#include "ssGUI/Enums/Key.hpp"
#include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/ButtonStateChangedEventCallback.hpp"




int main(int argc, char *argv[]) {
    ssGUI::MainWindow mainWindow;

    

    return 0;
}
*/
#include <iostream>
#include "SFML/Graphics.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }


    return 0;
}