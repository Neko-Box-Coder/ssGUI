#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"

#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"        //For getting cursor in MainWindow space
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssLogger/ssLog.hpp"

#include "clip.h"   //TODO: Add macro for switching between this and SFML one.
#include <codecvt>
#include <algorithm>

// #include "SFML/Window/Clipboard.hpp"

namespace ssGUI::Backend
{    
    BackendSystemInputWin32_OpenGL3_3::BackendSystemInputWin32_OpenGL3_3()
    {
        /*if(!SFMLCursor.loadFromSystem(sf::Cursor::Arrow))
        {
            ssLOG_LINE("Failed to load cursor!");
            ssLOG_EXIT_PROGRAM();
        }

        #if !SSGUI_USE_SFML_TIME
            ElapsedTime = std::chrono::high_resolution_clock::now();
        #endif

        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
        */
    }

    BackendSystemInputWin32_OpenGL3_3::~BackendSystemInputWin32_OpenGL3_3()
    {
        //ssGUI::Backend::BackendManager::RemoveInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }


    void BackendSystemInputWin32_OpenGL3_3::UpdateInput(/*std::vector<ssGUI::Backend::BackendMainWindowInterface*>& mainWindows*/)
    {
        /*ssLOG_FUNC_ENTRY();
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
                if(CurrentInputInfos.empty())
                    curInfo.CurrentMousePosition = LastMousePosition;

                //Check mouse position as mouseMove event is not reliable
                if(event.type == sf::Event::MouseMoved) 
                { 
                    event.mouseMove.x += ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetPosition().x +  
                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetPositionOffset().x;
 
                    event.mouseMove.y += ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetPosition().y +  
                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetPositionOffset().y;

                    curInfo.MouseMoved = true;
                    curInfo.CurrentMousePosition = glm::ivec2(event.mouseMove.x, event.mouseMove.y);
                    CurrentInputInfos.push_back(curInfo);
                    continue;
                }
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
                    FetchKeysReleased(input, CurrentKeyPresses);
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

        ssLOG_FUNC_EXIT();*/
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputWin32_OpenGL3_3::GetLastButtonAndKeyPresses()
    {
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> t;
        
        return t;
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputWin32_OpenGL3_3::GetCurrentButtonAndKeyPresses()
    {
        std::vector<ssGUI::Enums::GenericButtonAndKeyInput> t;
        //return CurrentKeyPresses;
        return t;
    }

    bool BackendSystemInputWin32_OpenGL3_3::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        //return std::find(LastKeyPresses.begin(), LastKeyPresses.end(), input) != LastKeyPresses.end();
        return true;
    }

    bool BackendSystemInputWin32_OpenGL3_3::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        //return std::find(CurrentKeyPresses.begin(), CurrentKeyPresses.end(), input) != CurrentKeyPresses.end();
        return true;
    }

    glm::ivec2 BackendSystemInputWin32_OpenGL3_3::GetLastMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        /*if(mainWindow != nullptr)
            return LastMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return LastMousePosition;
        */

        return glm::ivec2();
    }

    glm::ivec2 BackendSystemInputWin32_OpenGL3_3::GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        /*if(mainWindow != nullptr)
            return CurrentMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return CurrentMousePosition;
        */
        return glm::ivec2();
    }

    void BackendSystemInputWin32_OpenGL3_3::SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow)
    {
        /*
        if(mainWindow != nullptr)
            position += mainWindow->GetDisplayPosition() + mainWindow->GetPositionOffset();
        
        CurrentMousePosition = position;
        sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
        */
    }

    bool BackendSystemInputWin32_OpenGL3_3::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        for(int i = 0; i < LastMouseButtons.size(); i++)
            if(LastMouseButtons[i] == button)
                return true;
        
        return false;
    }

    bool BackendSystemInputWin32_OpenGL3_3::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        /*
        for(int i = 0; i < CurrentMouseButtons.size(); i++)
            if(CurrentMouseButtons[i] == button)
                return true;
        
        return false;
        */
    }

    glm::vec2 BackendSystemInputWin32_OpenGL3_3::GetCurrentMouseScrollDelta() const
    {
        //return MouseScrollDelta;
        return glm::vec2();
    }
    
    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputWin32_OpenGL3_3::GetLastRealtimeInputs() const
    {
        std::vector<ssGUI::RealtimeInputInfo> t;
        //return LastInputInfos;
        return t;
    }
            
    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputWin32_OpenGL3_3::GetCurrentRealtimeInputs() const
    {
        std::vector<ssGUI::RealtimeInputInfo> t;
        //return CurrentInputInfos;
        return t;
    }

    std::wstring BackendSystemInputWin32_OpenGL3_3::GetTextInput() const
    {
        //return InputText;
        return L"";
    }

    //Supported cursor type natively: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Cursor.php#ad41999c8633c2fbaa2364e379c1ab25b
    void BackendSystemInputWin32_OpenGL3_3::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        //CurrentCursor = cursorType;
        //std::cout<<"cursor type: "<<(int)CurrentCursor<<"\n";
    }

    ssGUI::Enums::CursorType BackendSystemInputWin32_OpenGL3_3::GetCursorType() const
    {
        //return CurrentCursor;
        return ssGUI::Enums::CursorType::NORMAL;
    }

    //TODO: Store the hotspot and also shouldn't be setting the cursor 
    void BackendSystemInputWin32_OpenGL3_3::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        /*
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
        else
        {
            if(customCursor->GetSize() == cursorSize)
                CustomCursors[cursorName].first = static_cast<sf::Texture*>(customCursor->GetBackendImageInterface()->GetRawHandle())->copyToImage();
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
                CustomCursors[cursorName].first.create(sf::Vector2u(cursorSize.x, cursorSize.y), cursorPtrArr[(populatedImg + 1) % 2]);

                delete[] cursorPtr;
                delete[] cursorPtr1;
            }
        }
        
        //Setting hotspot
        CustomCursors[cursorName].second = hotspot;

        ssLOG_FUNC_EXIT();
        */
    }

    void BackendSystemInputWin32_OpenGL3_3::SetCurrentCustomCursor(std::string cursorName)
    {
        /*   
        if(!HasCustomCursor(cursorName))
            return;
        
        if(CurrentCustomCursor == cursorName)
            return;

        CurrentCustomCursor = cursorName;
        */
    }

    void BackendSystemInputWin32_OpenGL3_3::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {        
        /*
        if(CurrentCustomCursor.empty())
            return;

        if(CustomCursors[CurrentCustomCursor].first.getPixelsPtr() == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory(CustomCursors[CurrentCustomCursor].first.getPixelsPtr(), CustomCursors[CurrentCustomCursor].first.getSize().x, 
            CustomCursors[CurrentCustomCursor].first.getSize().y))
        {
            ssLOG_LINE("Failed to load custom cursor image");   
        }

        hotspot = CustomCursors[CurrentCustomCursor].second;
        */
    }

    std::string BackendSystemInputWin32_OpenGL3_3::GetCurrentCustomCursorName()
    {
        //return CurrentCustomCursor;
        return "";
    }

    void BackendSystemInputWin32_OpenGL3_3::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {
        /*

        if(CustomCursors.find(cursorName) == CustomCursors.end())
            return;
        
        if(CustomCursors[cursorName].first.getPixelsPtr() == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory(CustomCursors[cursorName].first.getPixelsPtr(), CustomCursors[cursorName].first.getSize().x, 
            CustomCursors[cursorName].first.getSize().y))
        {
            ssLOG_LINE("Failed to load custom cursor image");
        }

        hotspot = CustomCursors[cursorName].second;
        */
    }

    bool BackendSystemInputWin32_OpenGL3_3::HasCustomCursor(std::string cursorName)
    {
        //return CustomCursors.find(cursorName) != CustomCursors.end();
        return true;
    }

    void BackendSystemInputWin32_OpenGL3_3::UpdateCursor()
    {
        /*
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
        */
    }

    bool BackendSystemInputWin32_OpenGL3_3::ClearClipboard()
    {
        //return clip::clear();
        return true;
    }

    bool BackendSystemInputWin32_OpenGL3_3::ClipbaordHasText()
    {
        //return clip::has(clip::text_format());
        return true;
    }
            
    bool BackendSystemInputWin32_OpenGL3_3::ClipbaordHasImage()
    {
        //return clip::has(clip::image_format());
        return true;
    }

    bool BackendSystemInputWin32_OpenGL3_3::SetClipboardImage(const ssGUI::ImageData& imgData)
    {
        /*
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
        */
        return true;
    }
            
    bool BackendSystemInputWin32_OpenGL3_3::SetClipboardText(const std::wstring& str)
    {
        //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        //return clip::set_text(converter.to_bytes(str));
        return true;
    }
            
    bool BackendSystemInputWin32_OpenGL3_3::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        /*clip::image img;

        if(!clip::get_image(img) || img.spec().bits_per_pixel != 32)
            return false;

        return imgData.LoadRawFromMemory(static_cast<void*>(img.data()), img.spec().width, img.spec().height);
        */
        return true;
    }

    bool BackendSystemInputWin32_OpenGL3_3::GetClipboardText(std::wstring& str)
    {
        // auto sfstr = sf::Clipboard::getString();

        // if(sfstr.isEmpty())
        //     return false;
        // else
        // {
        //     str = sfstr.toAnsiString();
        //     return true;
        // }

        /*
        std::string temp;
        if(!clip::get_text(temp))
            return false;

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        str = converter.from_bytes(temp);
        */
        return true;
    }

    uint64_t BackendSystemInputWin32_OpenGL3_3::GetElapsedTime() const
    {
        /*#if SSGUI_USE_SFML_TIME
            return ElapsedTime.getElapsedTime().asMilliseconds();
        #else
            //ElapsedTime is actually the start time but just keeping the name for compatibility
            using sysClock = std::chrono::high_resolution_clock;
            return std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::duration(sysClock::now() - ElapsedTime)).count();
        #endif
        */
        return 0;
    }
}


