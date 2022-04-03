#include "ssGUI/Backend/SFML/BackendSystemInputSFML.hpp"


#include "ssGUI/GUIObjectClasses/MainWindow.hpp"     //For getting cursor in MainWindow space

namespace ssGUI::Backend
{
    template <class T>
    void BackendSystemInputSFML::AddNonExistElements(std::vector<T>& elementsToAdd, std::vector<T>& vectorAddTo)
    {
        for(int i = 0; i < elementsToAdd.size(); i++)
        {
            if(std::find_if(vectorAddTo.begin(), vectorAddTo.end(), [&elementsToAdd, i](T key){return elementsToAdd[i] == key;}) 
                == vectorAddTo.end())
            {
                vectorAddTo.push_back(elementsToAdd[i]);
            }
        }
    }

    template <class T>
    void BackendSystemInputSFML::RemoveExistElements(std::vector<T>& elementsToRemove, std::vector<T>& vectorRemoveFrom)
    {
        for(int i = 0; i < elementsToRemove.size(); i++)
        {
            typename std::vector<T>::iterator foundElement =
                std::find_if(vectorRemoveFrom.begin(), vectorRemoveFrom.end(), [&elementsToRemove, i](T key){return elementsToRemove[i] == key;});

            if(foundElement != vectorRemoveFrom.end())
                vectorRemoveFrom.erase(foundElement);
        }
    }

    void BackendSystemInputSFML::FetchKeysPressed(ssGUI::KeyPresses keysPressedDown)
    {
        AddNonExistElements<ssGUI::Enums::FunctionKey>(keysPressedDown.FunctionKey, CurrentKeyPresses.FunctionKey);
        AddNonExistElements<ssGUI::Enums::LetterKey>(keysPressedDown.LetterKey, CurrentKeyPresses.LetterKey);
        AddNonExistElements<ssGUI::Enums::NumberKey>(keysPressedDown.NumberKey, CurrentKeyPresses.NumberKey);
        AddNonExistElements<ssGUI::Enums::SymbolKey>(keysPressedDown.SymbolKey, CurrentKeyPresses.SymbolKey);
        AddNonExistElements<ssGUI::Enums::SystemKey>(keysPressedDown.SystemKey, CurrentKeyPresses.SystemKey);
    }

    void BackendSystemInputSFML::FetchKeysReleased(ssGUI::KeyPresses keysReleased)
    {
        RemoveExistElements<ssGUI::Enums::FunctionKey>(keysReleased.FunctionKey, CurrentKeyPresses.FunctionKey);
        RemoveExistElements<ssGUI::Enums::LetterKey>(keysReleased.LetterKey, CurrentKeyPresses.LetterKey);
        RemoveExistElements<ssGUI::Enums::NumberKey>(keysReleased.NumberKey, CurrentKeyPresses.NumberKey);
        RemoveExistElements<ssGUI::Enums::SymbolKey>(keysReleased.SymbolKey, CurrentKeyPresses.SymbolKey);
        RemoveExistElements<ssGUI::Enums::SystemKey>(keysReleased.SystemKey, CurrentKeyPresses.SystemKey);
    }

    BackendSystemInputSFML::BackendSystemInputSFML() : CurrentKeyPresses(), LastKeyPresses(), InputText(), CurrentMousePosition(), LastMousePosition(),
                                            CurrentMouseButtons(), LastMouseButtons(), SFMLCursor(), CurrentCursor(ssGUI::Enums::CursorType::NORMAL),
                                            CursorMappedWindow(), ElapsedTime()
    {
        if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
        {
            DEBUG_LINE("Failed to load cursor!");
            DEBUG_EXIT_PROGRAM();
        }

        #if !USE_SFML_TIME
            ElapsedTime = std::chrono::high_resolution_clock::now();
        #endif

        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }

    BackendSystemInputSFML::~BackendSystemInputSFML()
    {
        ssGUI::Backend::BackendManager::RemoveInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }


    void BackendSystemInputSFML::UpdateInput(/*std::vector<ssGUI::Backend::BackendMainWindowInterface*>& mainWindows*/)
    {
        InputText.clear();
        
        //Set last key presses and mouse buttons
        LastKeyPresses = CurrentKeyPresses;
        LastMouseButtons = std::vector<ssGUI::Enums::MouseButton>(CurrentMouseButtons);
        std::vector<sf::Event> keyPressedEvents;
        std::vector<sf::Event> keyReleasedEvents;
        std::vector<sf::Event> mousePressedEvents;
        std::vector<sf::Event> mouseReleasedEvents;

        //Get text input
        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            sf::RenderWindow* sfWindow = static_cast<sf::RenderWindow*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetRawHandle());

            sf::Event event;
            
            while (sfWindow->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->Close();

                if (event.type == sf::Event::Resized)
                    sfWindow->setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)sfWindow->getSize().x, (float)sfWindow->getSize().y))));
                
                if (event.type == sf::Event::TextEntered)
                    if (event.text.unicode > 31 && event.text.unicode < 128 && event.text.unicode > 160)
                        InputText += event.text.unicode;

                if(event.type == sf::Event::KeyPressed)
                    keyPressedEvents.push_back(event);
                else if(event.type == sf::Event::KeyReleased)
                    keyReleasedEvents.push_back(event);

                if(event.type == sf::Event::MouseButtonPressed)
                    mousePressedEvents.push_back(event);
                else if(event.type == sf::Event::MouseButtonReleased)
                    mouseReleasedEvents.push_back(event);
            }
        }

        FetchKeysPressed(SFMLInputConverter::ConvertKeys(keyPressedEvents));
        FetchKeysReleased(SFMLInputConverter::ConvertKeys(keyReleasedEvents));

        std::vector<ssGUI::Enums::MouseButton> mousePressedButtons = SFMLInputConverter::ConvertMouseButtons(mousePressedEvents);
        std::vector<ssGUI::Enums::MouseButton> mouseReleasedButtons = SFMLInputConverter::ConvertMouseButtons(mouseReleasedEvents);

        AddNonExistElements<ssGUI::Enums::MouseButton>(mousePressedButtons, CurrentMouseButtons);
        RemoveExistElements<ssGUI::Enums::MouseButton>(mouseReleasedButtons, CurrentMouseButtons);
        
        //Get mouse position
        LastMousePosition = CurrentMousePosition;
        CurrentMousePosition = glm::ivec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

        //TODO: Get Mouse scroll
    }

    const ssGUI::KeyPresses& BackendSystemInputSFML::GetLastKeyPresses()
    {
        return LastKeyPresses;
    }

    const ssGUI::KeyPresses& BackendSystemInputSFML::GetCurrentKeyPresses()
    {
        return CurrentKeyPresses;
    }

    glm::ivec2 BackendSystemInputSFML::GetLastMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        if(mainWindow != nullptr)
            return LastMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return LastMousePosition;
    }

    glm::ivec2 BackendSystemInputSFML::GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        if(mainWindow != nullptr)
            return CurrentMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return CurrentMousePosition;
    }

    bool BackendSystemInputSFML::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        for(int i = 0; i < LastMouseButtons.size(); i++)
            if(LastMouseButtons[i] == button)
                return true;
        
        return false;
    }

    bool BackendSystemInputSFML::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        for(int i = 0; i < CurrentMouseButtons.size(); i++)
            if(CurrentMouseButtons[i] == button)
                return true;
        
        return false;
    }

    void BackendSystemInputSFML::SetMousePosition(glm::ivec2 position)
    {
        CurrentMousePosition = position;
        sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
    }

    std::wstring BackendSystemInputSFML::GetTextInput() const
    {
        return InputText;
    }

    void BackendSystemInputSFML::UpdateCursor()
    {
        switch (CurrentCursor)
        {
            case ssGUI::Enums::CursorType::NONE:
                break;
            case ssGUI::Enums::CursorType::NORMAL:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::TEXT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Text))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::HAND:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Hand))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_LEFT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeLeft))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_RIGHT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeRight))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_UP:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeTop))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_DOWN:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeBottom))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_TOP_LEFT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeTopLeft))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeTopRight))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeBottomRight))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeBottomLeft))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::MOVE:
                if (!SFMLCursor.loadFromSystem(sf::Cursor::SizeAll)) //Not supported natively for mac os
                    if(!SFMLCursor.loadFromSystem(sf::Cursor::Cross))
                        DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::HELP:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Help))
                    DEBUG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::NOT_ALLOWED:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::NotAllowed))
                    DEBUG_LINE("Failed to load cursor");
                break;
        }

        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            if(ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->IsClosed())
                continue;            
            
            if(CurrentCursor == ssGUI::Enums::CursorType::NONE)
            {
                static_cast<sf::RenderWindow*>(ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetRawHandle())->setMouseCursorVisible(false);
            }
            else
            {
                //For whatever reason, when the cursor is changed and set visible without setting mouse cursor, it will crash. 
                //This line is needed for whatever reason lol 
                static_cast<sf::RenderWindow*>(ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetRawHandle())->setMouseCursor(SFMLCursor);
                static_cast<sf::RenderWindow*>(ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetRawHandle())->setMouseCursorVisible(true);
            }
        }
    }

    //Supported cursor type natively: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Cursor.php#ad41999c8633c2fbaa2364e379c1ab25b
    void BackendSystemInputSFML::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        CurrentCursor = cursorType;
        //std::cout<<"cursor type: "<<(int)CurrentCursor<<"\n";
    }

    ssGUI::Enums::CursorType BackendSystemInputSFML::GetCursorType() const
    {
        return CurrentCursor;
    }

    uint64_t BackendSystemInputSFML::GetElapsedTime() const
    {
        #if USE_SFML_TIME
            return ElapsedTime.getElapsedTime().asMilliseconds();
        #else
            //ElapsedTime is actually the start time but just keeping the name for compatibility
            using sysClock = std::chrono::high_resolution_clock;
            return std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::duration(sysClock::now() - ElapsedTime)).count();
        #endif
    }
}


