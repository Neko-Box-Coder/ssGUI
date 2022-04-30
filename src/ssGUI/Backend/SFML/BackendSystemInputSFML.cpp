#include "ssGUI/Backend/SFML/BackendSystemInputSFML.hpp"

#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"     //For getting cursor in MainWindow space

namespace ssGUI::Backend
{
    sf::Image BackendSystemInputSFML::CustomCursorImage = sf::Image();
    glm::ivec2 BackendSystemInputSFML::Hotspot = glm::ivec2();
    
    template <class T>
    void BackendSystemInputSFML::AddNonExistElements(std::vector<T>& elementsToAdd, std::vector<T>& vectorAddTo)
    {
        FUNC_DEBUG_ENTRY();

        for(int i = 0; i < elementsToAdd.size(); i++)
        {
            if(std::find_if(vectorAddTo.begin(), vectorAddTo.end(), [&elementsToAdd, i](T key){return elementsToAdd[i] == key;}) 
                == vectorAddTo.end())
            {
                vectorAddTo.push_back(elementsToAdd[i]);
            }
        }

        FUNC_DEBUG_EXIT();
    }

    template <class T>
    void BackendSystemInputSFML::RemoveExistElements(std::vector<T>& elementsToRemove, std::vector<T>& vectorRemoveFrom)
    {
        FUNC_DEBUG_ENTRY();
        
        for(int i = 0; i < elementsToRemove.size(); i++)
        {
            typename std::vector<T>::iterator foundElement =
                std::find_if(vectorRemoveFrom.begin(), vectorRemoveFrom.end(), [&elementsToRemove, i](T key){return elementsToRemove[i] == key;});

            if(foundElement != vectorRemoveFrom.end())
                vectorRemoveFrom.erase(foundElement);
        }

        FUNC_DEBUG_EXIT();
    }

    void BackendSystemInputSFML::FetchKeysPressed(ssGUI::KeyPresses keysPressedDown)
    {
        FUNC_DEBUG_ENTRY();
        AddNonExistElements<ssGUI::Enums::FunctionKey>(keysPressedDown.FunctionKey, CurrentKeyPresses.FunctionKey);
        AddNonExistElements<ssGUI::Enums::LetterKey>(keysPressedDown.LetterKey, CurrentKeyPresses.LetterKey);
        AddNonExistElements<ssGUI::Enums::NumberKey>(keysPressedDown.NumberKey, CurrentKeyPresses.NumberKey);
        AddNonExistElements<ssGUI::Enums::SymbolKey>(keysPressedDown.SymbolKey, CurrentKeyPresses.SymbolKey);
        AddNonExistElements<ssGUI::Enums::SystemKey>(keysPressedDown.SystemKey, CurrentKeyPresses.SystemKey);
        FUNC_DEBUG_EXIT();
    }

    void BackendSystemInputSFML::FetchKeysReleased(ssGUI::KeyPresses keysReleased)
    {
        FUNC_DEBUG_ENTRY();
        RemoveExistElements<ssGUI::Enums::FunctionKey>(keysReleased.FunctionKey, CurrentKeyPresses.FunctionKey);
        RemoveExistElements<ssGUI::Enums::LetterKey>(keysReleased.LetterKey, CurrentKeyPresses.LetterKey);
        RemoveExistElements<ssGUI::Enums::NumberKey>(keysReleased.NumberKey, CurrentKeyPresses.NumberKey);
        RemoveExistElements<ssGUI::Enums::SymbolKey>(keysReleased.SymbolKey, CurrentKeyPresses.SymbolKey);
        RemoveExistElements<ssGUI::Enums::SystemKey>(keysReleased.SystemKey, CurrentKeyPresses.SystemKey);
        FUNC_DEBUG_EXIT();
    }

    void BackendSystemInputSFML::ResizeBilinear(const uint8_t* inputPixels, int w, int h, uint8_t* outputPixels, int w2, int h2)
    {
        FUNC_DEBUG_ENTRY();
        const uint8_t* a;
        const uint8_t* b;
        const uint8_t* c;
        const uint8_t* d; 
        int x, y, index;
        float x_ratio = ((float)(w - 1)) / w2 ;
        float y_ratio = ((float)(h - 1)) / h2 ;
        float x_diff, y_diff, blue, red, green, alpha;
        int offset = 0;
        bool aValid, bValid, cValid, dValid;
        for (int i = 0; i < h2; i++) 
        {
            for (int j = 0; j < w2; j++) 
            {
                x =         (int)(x_ratio * j) ;
                y =         (int)(y_ratio * i) ;
                x_diff =    (x_ratio * j) - x ;
                y_diff =    (y_ratio * i) - y ;
                index =     (y * w + x) ;                
                a =         &inputPixels[index * 4] ;
                b =         &inputPixels[(index + 1) * 4] ;
                c =         &inputPixels[(index + w) * 4] ;
                d =         &inputPixels[(index + w + 1) * 4] ;

                //Make sure pixels with 0 alpha are not affecting any of the sampling
                aValid = *(a + 3) > 0;
                bValid = *(b + 3) > 0;
                cValid = *(c + 3) > 0;
                dValid = *(d + 3) > 0;

                float inverseWidthAndHight =    (1 - x_diff) * (1 - y_diff);
                float widthAndInverseHeight =   (x_diff) * (1 - y_diff);
                float heightAndInverseWidth =   (y_diff) * (1 - x_diff);
                float widthHeight =             (x_diff * y_diff);
                float total =   inverseWidthAndHight * aValid+ 
                                widthAndInverseHeight * bValid+ 
                                heightAndInverseWidth * cValid+ 
                                widthHeight * dValid;

                // red element
                // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
                red =   *(a + 0) * inverseWidthAndHight * aValid +
                        *(b + 0) * widthAndInverseHeight * bValid +
                        *(c + 0) * heightAndInverseWidth * cValid + 
                        *(d + 0) * widthHeight * dValid;
                if(red > 0)
                    red /= total;

                // green element
                // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
                green = *(a + 1) * inverseWidthAndHight * aValid +
                        *(b + 1) * widthAndInverseHeight * bValid +
                        *(c + 1) * heightAndInverseWidth * cValid +
                        *(d + 1) * widthHeight * dValid;
                if(green > 0)
                    green /= total;

                // blue element
                // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
                blue =  *(a + 2) * inverseWidthAndHight * aValid +
                        *(b + 2) * widthAndInverseHeight * bValid +
                        *(c + 2) * heightAndInverseWidth * cValid +
                        *(d + 2) * widthHeight * dValid;
                if(blue > 0)
                    blue /= total;

                // alpha element
                //Using nearest neighbour for alpha otherwise it will show the color for pixels we are sampling that have 0 alpha
                if(inverseWidthAndHight > 0.25)
                    alpha = *(a + 3);
                else if(widthAndInverseHeight > 0.25)
                    alpha = *(b + 3);
                else if(heightAndInverseWidth > 0.25)
                    alpha = *(c + 3);
                else
                    alpha = *(d + 3);

                //Ya = Aa(1-w)(1-h) + Ba(w)(1-h) + Ca(h)(1-w) + Da(wh)
                // alpha = *(a + 3) * inverseWidthAndHight + 
                //         *(b + 3) * widthAndInverseHeight +
                //         *(c + 3) * heightAndInverseWidth + 
                //         *(d + 3) * widthHeight;
                
                //DEBUG_LINE("Pixel["<<i<<"]["<<j<<"]: ("<<red<<", "<<green<<", "<<blue<<", "<<alpha<<")");

                // range is 0 to 255 thus bitwise AND with 0xff
                outputPixels[offset * 4] =      (uint8_t)(((int)red) & 0xff);
                outputPixels[offset * 4 + 1] =  (uint8_t)(((int)green) & 0xff);
                outputPixels[offset * 4 + 2] =  (uint8_t)(((int)blue) & 0xff);
                outputPixels[offset * 4 + 3] =  (uint8_t)(((int)alpha) & 0xff);
                offset++;
            }
        }
        FUNC_DEBUG_EXIT();
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
        FUNC_DEBUG_ENTRY();
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
        FUNC_DEBUG_EXIT();
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
        FUNC_DEBUG_ENTRY();
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
            case ssGUI::Enums::CursorType::CUSTOM:
                if(CustomCursorImage.getPixelsPtr() != nullptr)
                    if(!SFMLCursor.loadFromPixels(CustomCursorImage.getPixelsPtr(), CustomCursorImage.getSize(), sf::Vector2u(Hotspot.x, Hotspot.y)))
                        DEBUG_LINE("Failed to load cursor");
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
        FUNC_DEBUG_EXIT();
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

    void BackendSystemInputSFML::SetCustomCursor(ssGUI::ImageData* customCursor, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        FUNC_DEBUG_ENTRY();
        if(customCursor == nullptr)
        {
            CustomCursorImage = sf::Image();
            FUNC_DEBUG_EXIT();
            return;
        }

        if(customCursor->GetSize() == cursorSize)
            CustomCursorImage = static_cast<sf::Texture*>(customCursor->GetBackendImageInterface()->GetRawHandle())->copyToImage();
        else
        {            
            //Original cursor image
            auto oriCursorImg = static_cast<sf::Texture*>(customCursor->GetBackendImageInterface()->GetRawHandle())->copyToImage();

            //temporary image pointers for resizing
            uint8_t* cursorPtr = new uint8_t[oriCursorImg.getSize().x * oriCursorImg.getSize().y * 4];
            uint8_t* cursorPtr1 = new uint8_t[1];
            uint8_t* cursorPtrArr[] = {cursorPtr, cursorPtr1};

            //Flag for indicating which pointer has just been populated
            int populatedImg = 0;

            //Populate the first temporary image pointer
            for(int i = 0; i < oriCursorImg.getSize().x * oriCursorImg.getSize().y * 4; i++)
                cursorPtr[i] = (*(oriCursorImg.getPixelsPtr() + i));

            //Record the current image size
            glm::ivec2 currentCursorSize = glm::ivec2(oriCursorImg.getSize().x, oriCursorImg.getSize().y);

            //Resize width until the new cursor size is within 2x or 0.5x
            while ((currentCursorSize.x > cursorSize.x && currentCursorSize.x * 2 < cursorSize.x) ||
                    (currentCursorSize.x < cursorSize.x && (int)(currentCursorSize.x * 0.5) > cursorSize.x))
            {
                delete[] cursorPtrArr[(populatedImg + 1) % 2];
                
                //Enlarging
                if(currentCursorSize.x > cursorSize.x)
                {
                    cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentCursorSize.x * 2 * currentCursorSize.y * 4];

                    ResizeBilinear
                    (
                        cursorPtrArr[populatedImg], 
                        currentCursorSize.x, 
                        currentCursorSize.y,
                        cursorPtrArr[(populatedImg + 1) % 2],
                        currentCursorSize.x * 2,
                        currentCursorSize.y
                    );

                    currentCursorSize.x *= 2;
                }
                //Reducing
                else
                {
                    cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[(int)(currentCursorSize.x * 0.5) * currentCursorSize.y * 4];

                    ResizeBilinear
                    (
                        cursorPtrArr[populatedImg], 
                        currentCursorSize.x, 
                        currentCursorSize.y,
                        cursorPtrArr[(populatedImg + 1) % 2],
                        currentCursorSize.x * 0.5,
                        currentCursorSize.y
                    );

                    currentCursorSize.x *= 0.5;
                }

                populatedImg = (populatedImg + 1) % 2;
            }
            
            //Resize height until the new cursor size is within 2x or 0.5x
            while ((currentCursorSize.y > cursorSize.y && currentCursorSize.y * 2 < cursorSize.y) ||
                    (currentCursorSize.y < cursorSize.y && (int)(currentCursorSize.y * 0.5) > cursorSize.y))
            {
                delete[] cursorPtrArr[(populatedImg + 1) % 2];
                
                //Enlarging
                if(currentCursorSize.y > cursorSize.y)
                {
                    cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentCursorSize.x * currentCursorSize.y * 2 * 4];

                    ResizeBilinear
                    (
                        cursorPtrArr[populatedImg], 
                        currentCursorSize.x, 
                        currentCursorSize.y,
                        cursorPtrArr[(populatedImg + 1) % 2],
                        currentCursorSize.x,
                        currentCursorSize.y * 2
                    );

                    currentCursorSize.y *= 2;
                }
                //Reducing
                else
                {
                    cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentCursorSize.x * (int)(currentCursorSize.y * 0.5) * 4];

                    ResizeBilinear
                    (
                        cursorPtrArr[populatedImg],
                        currentCursorSize.x, 
                        currentCursorSize.y,
                        cursorPtrArr[(populatedImg + 1) % 2],
                        currentCursorSize.x,
                        currentCursorSize.y * 0.5
                    );

                    currentCursorSize.y *= 0.5;
                }

                populatedImg = (populatedImg + 1) % 2;
            }

            //Do the final round of resizing
            cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[cursorSize.x * cursorSize.y * 4];
            ResizeBilinear(cursorPtrArr[populatedImg], currentCursorSize.x, currentCursorSize.y, cursorPtrArr[(populatedImg + 1) % 2], cursorSize.x, cursorSize.y);
            CustomCursorImage.create(cursorSize.x, cursorSize.y, cursorPtrArr[(populatedImg + 1) % 2]);

            delete[] cursorPtr;
            delete[] cursorPtr1;
        }
        
        if(CustomCursorImage.getPixelsPtr() != nullptr)
        {            
            if(!SFMLCursor.loadFromPixels(CustomCursorImage.getPixelsPtr(), CustomCursorImage.getSize(), sf::Vector2u(hotspot.x, hotspot.y)))
                DEBUG_LINE("Failed to load cursor");
            
            Hotspot = hotspot;
        }
        else
            DEBUG_LINE("Failed to load cursor");

        FUNC_DEBUG_EXIT();
    }

    void BackendSystemInputSFML::GetCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {        
        if(CustomCursorImage.getPixelsPtr() == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory(CustomCursorImage.getPixelsPtr(), CustomCursorImage.getSize().x, CustomCursorImage.getSize().y))
            DEBUG_LINE("Failed to load custom cursor image");

        hotspot = Hotspot;
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


