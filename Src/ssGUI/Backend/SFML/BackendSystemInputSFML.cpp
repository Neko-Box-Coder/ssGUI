#include "ssGUI/Backend/SFML/BackendSystemInputSFML.hpp"

#include "ssGUI/HelperClasses/SFMLImageConversion.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"        //For getting cursor in MainWindow space
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssLogger/ssLog.hpp"

#include "clip.h"   //TODO: Add macro for switching between this and SFML one.
#include <codecvt>
#include <algorithm>

// #include "SFML/Window/Clipboard.hpp"

namespace ssGUI
{

namespace Backend
{    
    template <class T>
    void BackendSystemInputSFML::AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo)
    {
        ssLOG_FUNC_ENTRY();

        if(std::find_if(vectorAddTo.begin(), vectorAddTo.end(), [&elementToAdd](T key){return elementToAdd == key;}) 
            == vectorAddTo.end())
        {
            vectorAddTo.push_back(elementToAdd);
        }

        ssLOG_FUNC_EXIT();
    }

    template <class T>
    void BackendSystemInputSFML::RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom)
    {
        ssLOG_FUNC_ENTRY();
        
        typename std::vector<T>::iterator foundElement =
            std::find_if(vectorRemoveFrom.begin(), vectorRemoveFrom.end(), [&elementToRemove](T key){return elementToRemove == key;});

        if(foundElement != vectorRemoveFrom.end())
            vectorRemoveFrom.erase(foundElement);

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputSFML::FetchKeysPressed(ssGUI::Enums::GenericButtonAndKeyInput keysPressedDown, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses)
    {
        ssLOG_FUNC_ENTRY();
        auto it = std::find(destinationKeyPresses.begin(), destinationKeyPresses.end(), keysPressedDown);
        if(it == destinationKeyPresses.end())
            destinationKeyPresses.push_back(keysPressedDown);

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputSFML::FetchKeysReleased(ssGUI::Enums::GenericButtonAndKeyInput keysReleased, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses)
    {
        ssLOG_FUNC_ENTRY();        
        auto it = std::find(destinationKeyPresses.begin(), destinationKeyPresses.end(), keysReleased);
        if(it != destinationKeyPresses.end())
            destinationKeyPresses.erase(it);

        ssLOG_FUNC_EXIT();
    }

    BackendSystemInputSFML::BackendSystemInputSFML() :  CurrentKeyPresses(),
                                                        LastKeyPresses(),
                                                        InputText(L""),
                                                        CurrentMousePosition(),
                                                        LastMousePosition(),
                                                        CurrentMouseButtons(),
                                                        LastMouseButtons(),
                                                        MouseScrollDelta(),
                                                        CurrentInputInfos(),
                                                        LastInputInfos(),
                                                        SFMLCursor(),
                                                        CurrentCursor(ssGUI::Enums::CursorType::NORMAL),
                                                        CursorMappedWindow(),
                                                        CustomCursors(),
                                                        CurrentCustomCursor(""),
                                                        ElapsedTime()
    {
        if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
        {
            ssLOG_LINE("Failed to load cursor!");
            ssLOG_EXIT_PROGRAM();
        }

        #if !SSGUI_USE_SFML_TIME
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
        ssLOG_FUNC_ENTRY();
        InputText.clear();
        MouseScrollDelta = glm::vec2();
        
        //Set last key presses and mouse buttons
        LastKeyPresses = CurrentKeyPresses;
        LastMouseButtons = CurrentMouseButtons;
        LastInputInfos = std::move(CurrentInputInfos);

        //Get mouse position
        LastMousePosition = CurrentMousePosition;
        CurrentMousePosition = glm::ivec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

        //Get text input
        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            sf::RenderWindow* sfWindow = static_cast<sf::RenderWindow*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetRawHandle());

            sf::Event event;
            
            while (sfWindow->pollEvent(event))
            {
                //Add a new item to realtime input info, that continues from the last input state
                ssGUI::RealtimeInputInfo curInfo;

                //TODO: Remove this? This looks wrong
                if(CurrentInputInfos.empty())
                    curInfo.CurrentMousePosition = LastMousePosition;

                //Check mouse position as mouseMove event is not reliable
                if(event.type == sf::Event::MouseMoved) 
                { 
                    event.mouseMove.x += ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetWindowPosition().x +  
                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetPositionOffset().x;
 
                    event.mouseMove.y += ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetWindowPosition().y +  
                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetPositionOffset().y;

                    curInfo.MouseMoved = true;
                    curInfo.CurrentMousePosition = glm::ivec2(event.mouseMove.x, event.mouseMove.y);
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }
                //TODO: Change this? This looks wrong
                else if(glm::ivec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y) != curInfo.CurrentMousePosition)
                {
                    curInfo.MouseMoved = true;
                    curInfo.CurrentMousePosition = glm::ivec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
                    CurrentInputInfos.push_back(curInfo);
                    curInfo = ssGUI::RealtimeInputInfo();
                }

                if (event.type == sf::Event::Closed)
                {
                    ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->Close();
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }

                if(event.type == sf::Event::GainedFocus)
                {
                    ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->SetFocus(true, true);
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }

                if(event.type == sf::Event::LostFocus)
                {
                    ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->SetFocus(false, true);
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }

                if (event.type == sf::Event::Resized)
                {
                    sfWindow->setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f((float)sfWindow->getSize().x, (float)sfWindow->getSize().y))));
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }
                
                if (event.type == sf::Event::TextEntered)
                {                   
                    InputText += event.text.unicode;
                    curInfo.CharacterEntered = true;
                    curInfo.CurrentCharacterEntered += event.text.unicode;
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }

                if(event.type == sf::Event::KeyPressed)
                {
                    ssGUI::Enums::GenericButtonAndKeyInput input = SFMLInputConverter::ConvertButtonAndKeys(event);
                    curInfo.CurrentButtonAndKeyChanged = input;
                    FetchKeysPressed(input, CurrentKeyPresses);
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }
                else if(event.type == sf::Event::KeyReleased)
                {
                    ssGUI::Enums::GenericButtonAndKeyInput input = SFMLInputConverter::ConvertButtonAndKeys(event);
                    curInfo.CurrentButtonAndKeyChanged = input;
                    FetchKeysReleased(input, CurrentKeyPresses);
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }

                if(event.type == sf::Event::MouseButtonPressed)
                {
                    ssGUI::Enums::GenericButtonAndKeyInput input = SFMLInputConverter::ConvertMouseButtons(event);
                    curInfo.CurrentButtonAndKeyChanged = input;
                    FetchKeysPressed(input, CurrentKeyPresses);
                    CurrentInputInfos.push_back(curInfo);
                    AddNonExistElement(static_cast<ssGUI::Enums::MouseButton>(input), CurrentMouseButtons);
                    continue;
                }
                else if(event.type == sf::Event::MouseButtonReleased)
                {
                    ssGUI::Enums::GenericButtonAndKeyInput input = SFMLInputConverter::ConvertMouseButtons(event);
                    curInfo.CurrentButtonAndKeyChanged = input;
                    FetchKeysReleased(input, CurrentKeyPresses);
                    CurrentInputInfos.push_back(curInfo);
                    RemoveExistElement(static_cast<ssGUI::Enums::MouseButton>(input), CurrentMouseButtons);
                    continue;
                }

                if(event.type == sf::Event::MouseWheelScrolled)
                {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                        MouseScrollDelta.y = event.mouseWheelScroll.delta;
                    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                        MouseScrollDelta.x = event.mouseWheelScroll.delta * -1;
                    continue;
                }
            }
        }

        ssLOG_FUNC_EXIT();
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputSFML::GetLastButtonAndKeyPresses()
    {
        return LastKeyPresses;
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputSFML::GetCurrentButtonAndKeyPresses()
    {
        return CurrentKeyPresses;
    }

    bool BackendSystemInputSFML::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return std::find(LastKeyPresses.begin(), LastKeyPresses.end(), input) != LastKeyPresses.end();
    }

    bool BackendSystemInputSFML::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return std::find(CurrentKeyPresses.begin(), CurrentKeyPresses.end(), input) != CurrentKeyPresses.end();
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

    void BackendSystemInputSFML::SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow)
    {
        //TODO: This might not work for multi-monitor, change this to use window version of setPosition instead
        if(mainWindow != nullptr)
            position += mainWindow->GetDisplayPosition() + mainWindow->GetPositionOffset();
        
        CurrentMousePosition = position;
        sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
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

    glm::vec2 BackendSystemInputSFML::GetCurrentMouseScrollDelta() const
    {
        return MouseScrollDelta;
    }
    
    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputSFML::GetLastRealtimeInputs() const
    {
        return LastInputInfos;
    }
            
    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputSFML::GetCurrentRealtimeInputs() const
    {
        return CurrentInputInfos;
    }

    std::wstring BackendSystemInputSFML::GetTextInput() const
    {
        return InputText;
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

    //TODO: refactor this function, see Win32 version
    void BackendSystemInputSFML::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        ssLOG_FUNC_ENTRY();
        
        //Validation
        if(hotspot.x > cursorSize.x || hotspot.y > cursorSize.y)
        {
            ssLOG_LINE("Invalid hotspot position: "<<hotspot.x<<", "<<hotspot.y);
            return;
        }

        //Check if we already have the cursor
        if(CustomCursors.find(cursorName) != CustomCursors.end() && CustomCursors[cursorName].first.getSize().x == cursorSize.x && 
            CustomCursors[cursorName].first.getSize().y == cursorSize.y)
        {
            CustomCursors[cursorName].second = hotspot;
            ssLOG_FUNC_EXIT();
            return;
        }
        //Otherwise we can add the cursor to our cache
        else
        {
            #ifdef SSGUI_IMAGE_BACKEND_SFML
                sf::Image cursorImg = static_cast<sf::Texture*>(customCursor->GetBackendImageInterface()->GetRawHandle())->copyToImage();
            #else
                //Convert everything to sf::Image
                sf::Image cursorImg;
                ssGUI::ImageFormat cursorFormat;
                void* imgPtr = customCursor->GetPixelPtr(cursorFormat);
                
                if(imgPtr == nullptr)
                {
                    ssLOG_LINE("Failed to create custom cursor");
                    return;
                }
                
                bool result = false;

                //TODO: Move this to somewhere else
                if(cursorFormat.BitDepthPerChannel == 8)
                    result = ssGUI::SFMLImageConversion::ConvertToRGBA32<uint8_t>(cursorImg, imgPtr, cursorFormat, customCursor->GetSize());
                else if(cursorFormat.BitDepthPerChannel == 16)
                    result = ssGUI::SFMLImageConversion::ConvertToRGBA32<uint16_t>(cursorImg, imgPtr, cursorFormat, customCursor->GetSize());
                else
                {
                    ssLOG_LINE("Unsupported bit depth: "<<cursorFormat.BitDepthPerChannel);
                    return;
                }
                
                if(!result)
                {
                    ssLOG_LINE("Failed to convert image");
                    return;
                }
            #endif
        
            if(customCursor->GetSize() == cursorSize)
            {
                #ifdef SSGUI_IMAGE_BACKEND_SFML
                    CustomCursors[cursorName].first = static_cast<sf::Texture*>(customCursor->GetBackendImageInterface()->GetRawHandle())->copyToImage();
                #else
                    CustomCursors[cursorName].first = cursorImg;
                #endif
            }
            else
            {
                //Original cursor image
                auto& oriCursorImg = cursorImg;

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
                while ((currentCursorSize.x < cursorSize.x && currentCursorSize.x * 2 < cursorSize.x) ||
                        (currentCursorSize.x > cursorSize.x && (int)(currentCursorSize.x * 0.5) > cursorSize.x))
                {
                    delete[] cursorPtrArr[(populatedImg + 1) % 2];
                    
                    //Enlarging
                    if(currentCursorSize.x < cursorSize.x)
                    {
                        cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentCursorSize.x * 2 * currentCursorSize.y * 4];

                        ssGUI::ImageUtil::ResizeBilinear
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

                        ssGUI::ImageUtil::ResizeBilinear
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
                while ((currentCursorSize.y < cursorSize.y && currentCursorSize.y * 2 < cursorSize.y) ||
                        (currentCursorSize.y > cursorSize.y && (int)(currentCursorSize.y * 0.5) > cursorSize.y))
                {
                    delete[] cursorPtrArr[(populatedImg + 1) % 2];
                    
                    //Enlarging
                    if(currentCursorSize.y < cursorSize.y)
                    {
                        cursorPtrArr[(populatedImg + 1) % 2] = new uint8_t[currentCursorSize.x * currentCursorSize.y * 2 * 4];

                        ssGUI::ImageUtil::ResizeBilinear
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

                        ssGUI::ImageUtil::ResizeBilinear
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
                ssGUI::ImageUtil::ResizeBilinear(cursorPtrArr[populatedImg], currentCursorSize.x, currentCursorSize.y, cursorPtrArr[(populatedImg + 1) % 2], cursorSize.x, cursorSize.y);
                CustomCursors[cursorName].first.create(sf::Vector2u(cursorSize.x, cursorSize.y), cursorPtrArr[(populatedImg + 1) % 2]);

                delete[] cursorPtr;
                delete[] cursorPtr1;
            }
        }
        
        //Setting hotspot
        CustomCursors[cursorName].second = hotspot;

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputSFML::SetCurrentCustomCursor(std::string cursorName)
    {
        if(!HasCustomCursor(cursorName))
            return;
        
        if(CurrentCustomCursor == cursorName)
            return;

        CurrentCustomCursor = cursorName;
    }

    void BackendSystemInputSFML::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {        
        if(CurrentCustomCursor.empty())
            return;

        if(CustomCursors[CurrentCustomCursor].first.getPixelsPtr() == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory( CustomCursors[CurrentCustomCursor].first.getPixelsPtr(),
                                            ssGUI::ImageFormat(),
                                            glm::ivec2( CustomCursors[CurrentCustomCursor].first.getSize().x, 
                                                        CustomCursors[CurrentCustomCursor].first.getSize().y)))
        {
            ssLOG_LINE("Failed to load custom cursor image");   
        }

        hotspot = CustomCursors[CurrentCustomCursor].second;
    }

    std::string BackendSystemInputSFML::GetCurrentCustomCursorName()
    {
        return CurrentCustomCursor;
    }

    void BackendSystemInputSFML::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {
        if(CustomCursors.find(cursorName) == CustomCursors.end())
            return;
        
        if(CustomCursors[cursorName].first.getPixelsPtr() == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory( CustomCursors[cursorName].first.getPixelsPtr(), 
                                            ssGUI::ImageFormat(),
                                            glm::ivec2( CustomCursors[cursorName].first.getSize().x, 
                                                        CustomCursors[cursorName].first.getSize().y)))
        {
            ssLOG_LINE("Failed to load custom cursor image");
        }

        hotspot = CustomCursors[cursorName].second;
    }

    bool BackendSystemInputSFML::HasCustomCursor(std::string cursorName)
    {
        return CustomCursors.find(cursorName) != CustomCursors.end();
    }

    void BackendSystemInputSFML::UpdateCursor()
    {
        ssLOG_FUNC_ENTRY();
        switch (CurrentCursor)
        {
            case ssGUI::Enums::CursorType::NONE:
                break;
            case ssGUI::Enums::CursorType::NORMAL:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::TEXT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Text))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::HAND:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Hand))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_LEFT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeLeft))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_RIGHT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeRight))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_UP:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeTop))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_DOWN:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeBottom))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_TOP_LEFT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeTopLeft))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeTopRight))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeBottomRight))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::SizeBottomLeft))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::MOVE:
                if (!SFMLCursor.loadFromSystem(sf::Cursor::SizeAll)) //Not supported natively for mac os
                    if(!SFMLCursor.loadFromSystem(sf::Cursor::Cross))
                        ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::HELP:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::Help))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::NOT_ALLOWED:
                if(!SFMLCursor.loadFromSystem(sf::Cursor::NotAllowed))
                    ssLOG_LINE("Failed to load cursor");
                break;
            case ssGUI::Enums::CursorType::CUSTOM:
                if(!CurrentCustomCursor.empty() && CustomCursors[CurrentCustomCursor].first.getPixelsPtr() != nullptr)
                {
                    if(!SFMLCursor.loadFromPixels(CustomCursors[CurrentCustomCursor].first.getPixelsPtr(), CustomCursors[CurrentCustomCursor].first.getSize(), 
                        sf::Vector2u(CustomCursors[CurrentCustomCursor].second.x, CustomCursors[CurrentCustomCursor].second.y)))
                    {
                        ssLOG_LINE("Failed to load cursor");
                    }
                }
                else
                    ssLOG_LINE("Failed to load cursor");
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
        ssLOG_FUNC_EXIT();
    }

    bool BackendSystemInputSFML::ClearClipboard()
    {
        return clip::clear();
    }

    bool BackendSystemInputSFML::ClipbaordHasText()
    {
        return clip::has(clip::text_format());
    }
            
    bool BackendSystemInputSFML::ClipbaordHasImage()
    {
        return clip::has(clip::image_format());
    }

    bool BackendSystemInputSFML::SetClipboardImage(const ssGUI::ImageData& imgData)
    {
        if(!imgData.IsValid())
            return false;

        auto sfTexture = static_cast<sf::Texture*>(imgData.GetBackendImageInterface()->GetRawHandle());

        sf::Image img = sfTexture->copyToImage();

        if(img.getPixelsPtr() == nullptr)
            return false;

        clip::image_spec spec;
        spec.width = imgData.GetSize().x;
        spec.height = imgData.GetSize().y;
        spec.bits_per_pixel = 32;
        spec.bytes_per_row = spec.width*4;

        uint32_t endianness = 1;

        //Little endian
        if(*reinterpret_cast<uint8_t*>(&endianness) == 1)
        {
            spec.red_mask = 0xff;
            spec.green_mask = 0xff00;
            spec.blue_mask = 0xff0000;
            spec.alpha_mask = 0xff000000;
            spec.red_shift = 0;
            spec.green_shift = 8;
            spec.blue_shift = 16;
            spec.alpha_shift = 24;
        }
        //Big endian
        else
        {
            spec.red_mask = 0xff000000;
            spec.green_mask = 0xff0000;
            spec.blue_mask = 0xff00;
            spec.alpha_mask = 0xff;
            spec.red_shift = 24;
            spec.green_shift = 16;
            spec.blue_shift = 8;
            spec.alpha_shift = 0;
        }

        clip::image clipImg = clip::image(static_cast<const void*>(img.getPixelsPtr()), spec);
        return clip::set_image(clipImg);
    }
            
    bool BackendSystemInputSFML::SetClipboardText(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        return clip::set_text(converter.to_bytes(str));
    }
            
    bool BackendSystemInputSFML::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        clip::image img;

        if(!clip::get_image(img))
            return false;
        
        clip::image_spec spec = img.spec();
            
        ssGUI::ImageFormat format;
        format.HasAlpha = spec.alpha_mask > 0;
        format.IndexR = spec.red_shift / 8;
        format.IndexG = spec.green_shift / 8;
        format.IndexB = spec.blue_shift / 8;
        format.IndexA = spec.alpha_shift / 8;

        return imgData.LoadRawFromMemory(   static_cast<void*>(img.data()), 
                                            format,
                                            glm::ivec2(img.spec().width, img.spec().height));
    }

    bool BackendSystemInputSFML::GetClipboardText(std::wstring& str)
    {
        // auto sfstr = sf::Clipboard::getString();

        // if(sfstr.isEmpty())
        //     return false;
        // else
        // {
        //     str = sfstr.toAnsiString();
        //     return true;
        // }

        std::string temp;
        if(!clip::get_text(temp))
            return false;

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        str = converter.from_bytes(temp);

        return true;
    }

    uint64_t BackendSystemInputSFML::GetElapsedTime() const
    {
        #if SSGUI_USE_SFML_TIME
            return ElapsedTime.getElapsedTime().asMilliseconds();
        #else
            //ElapsedTime is actually the start time but just keeping the name for compatibility
            using sysClock = std::chrono::high_resolution_clock;
            return std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::duration(sysClock::now() - ElapsedTime)).count();
        #endif
    }
}

}
