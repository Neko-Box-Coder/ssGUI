#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"

#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"

#include "ssGUI/Backend/BackendManager.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"        //For getting cursor in MainWindow space
#include "ssGUI/DataClasses/RealtimeInputInfo.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/Win32InputConverter.hpp"
#include "ssLogger/ssLog.hpp"

#include "clip.h"
#include <codecvt>
#include <algorithm>

#include <windowsx.h>

#include "ssGUI/HelperClasses/ImageUtil.hpp"

namespace ssGUI
{

namespace Backend
{
    template <class T>
    void BackendSystemInputWin32_OpenGL3_3::AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo)
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
    void BackendSystemInputWin32_OpenGL3_3::RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom)
    {
        ssLOG_FUNC_ENTRY();
        
        typename std::vector<T>::iterator foundElement =
            std::find_if(vectorRemoveFrom.begin(), vectorRemoveFrom.end(), [&elementToRemove](T key){return elementToRemove == key;});

        if(foundElement != vectorRemoveFrom.end())
            vectorRemoveFrom.erase(foundElement);

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputWin32_OpenGL3_3::FetchKeysPressed(ssGUI::Enums::GenericButtonAndKeyInput keysPressedDown, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses)
    {
        ssLOG_FUNC_ENTRY();
        auto it = std::find(destinationKeyPresses.begin(), destinationKeyPresses.end(), keysPressedDown);
        if(it == destinationKeyPresses.end())
            destinationKeyPresses.push_back(keysPressedDown);

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputWin32_OpenGL3_3::FetchKeysReleased(ssGUI::Enums::GenericButtonAndKeyInput keysReleased, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses)
    {
        ssLOG_FUNC_ENTRY();        
        auto it = std::find(destinationKeyPresses.begin(), destinationKeyPresses.end(), keysReleased);
        if(it != destinationKeyPresses.end())
            destinationKeyPresses.erase(it);

        ssLOG_FUNC_EXIT();
    }

    HCURSOR BackendSystemInputWin32_OpenGL3_3::CreateWin32Cursor(glm::ivec2 hotspot, ssGUI::ImageData* imgData)
    {
        //Check if it is BGRA32
        ssGUI::ImageFormat format;
        void* pixelPtr = imgData->GetPixelPtr(format);
        if(pixelPtr == nullptr)
            return nullptr;

        if( format.BitPerPixel != 32 ||
            format.ImgType != ssGUI::Enums::ImageType::RGB ||
            format.IndexB != 0 ||
            format.IndexG != 1 ||
            format.IndexR != 2 ||
            format.IndexA != 3 ||
            format.RowPaddingInBytes != 0)
        {
            return nullptr;
        }

        //Create Win32 Cursor
        // Create the AND mask and XOR mask bitmaps from the BGRA image data
        HBITMAP hANDmask = CreateBitmap(imgData->GetSize().x, imgData->GetSize().y, 1, 1, NULL);
        HBITMAP hXORmask = CreateBitmap(imgData->GetSize().x, imgData->GetSize().y, 1, 32, pixelPtr);

        // Create the ICONINFO structure for the cursor
        ICONINFO iconInfo;
        iconInfo.fIcon = FALSE;
        iconInfo.xHotspot = hotspot.x;
        iconInfo.yHotspot = hotspot.y;
        iconInfo.hbmMask = hANDmask;
        iconInfo.hbmColor = hXORmask;
        HCURSOR hCursor = CreateIconIndirect(&iconInfo);

        return hCursor;
    }

    void BackendSystemInputWin32_OpenGL3_3::CreateEmptyCursorIfNeeded()
    {
        if(HasCustomCursor(SSGUI_EMPTY_CURSOR))
            return;

        //Create empty cursor
        CursorData emptyCursorData;

        BYTE andMask[1] = {1};
        BYTE xorMask[1] = {0};

        HCURSOR hCursor = CreateCursor(NULL, 0, 0, 1, 1, andMask, xorMask);

        emptyCursorData.Win32CursorHandle = hCursor;

        CustomCursors[SSGUI_EMPTY_CURSOR] = emptyCursorData;
    }

    BackendSystemInputWin32_OpenGL3_3::BackendSystemInputWin32_OpenGL3_3() :    CurrentKeyPresses(),
                                                                                LastKeyPresses(),
                                                                                InputText(L""),
                                                                                CurrentMousePosition(),
                                                                                LastMousePosition(),
                                                                                CurrentMouseButtons(),
                                                                                LastMouseButtons(),
                                                                                MouseScrollDelta(),
                                                                                CurrentInputInfos(),
                                                                                LastInputInfos(),
                                                                                CurrentCursor(ssGUI::Enums::CursorType::NORMAL),
                                                                                MainWindowRawHandles(),
                                                                                CustomCursors(),
                                                                                CurrentCustomCursor(""),
                                                                                StartTime()
    {
        StartTime = std::chrono::high_resolution_clock::now();
        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }

    BackendSystemInputWin32_OpenGL3_3::~BackendSystemInputWin32_OpenGL3_3()
    {
        ssGUI::Backend::BackendManager::RemoveInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }

    bool BackendSystemInputWin32_OpenGL3_3::HandleMessage(MSG msg)
    {
        auto mainWindowIt = MainWindowRawHandles.find(msg.hwnd);
        
        //If we don't have the raw window handle associated with a mainWindow, associate it.
        if(mainWindowIt == MainWindowRawHandles.end())
        {
            bool found = false;
            for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
            {
                auto currentMainWindow = ssGUI::Backend::BackendManager::GetMainWindowInterface(i);

                if(static_cast<Win32_OpenGL_Handles*>(currentMainWindow->GetRawHandle())->WindowHandle == msg.hwnd)
                {
                    MainWindowRawHandles[msg.hwnd] = currentMainWindow;
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                ssLOG_LINE("Failed to find main window from handle: "<<msg.hwnd);
                return false;
            }
        }

        ssGUI::RealtimeInputInfo curInfo;

        switch(msg.message)
        {
            //Mouse moved event
            case WM_MOUSEMOVE:
                curInfo.MouseMoved = true;
                curInfo.CurrentMousePosition = glm::ivec2(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
                curInfo.CurrentMousePosition.x += mainWindowIt->second->GetWindowPosition().x +  
                    mainWindowIt->second->GetPositionOffset().x;
                curInfo.CurrentMousePosition.y += mainWindowIt->second->GetWindowPosition().y +  
                    mainWindowIt->second->GetPositionOffset().y;
                CurrentInputInfos.push_back(curInfo);
                return true;

            //Close event
            case WM_CLOSE:
                mainWindowIt->second->Close();
                CurrentInputInfos.push_back(curInfo);
                return true;

            //Focus lost event
            //Focus Gained event
            case WM_ACTIVATE:
                if(msg.wParam == WA_INACTIVE)
                {
                    mainWindowIt->second->SetFocus(false, true);
                    CurrentInputInfos.push_back(curInfo);
                    return true;
                }
                else
                {
                    mainWindowIt->second->SetFocus(true, true);
                    CurrentInputInfos.push_back(curInfo);
                    return true;
                }
            
            //Resize event
            case WM_SIZE:
                //Render reshape (glViewPort) is handled in BackendDrawing
                //MainWindow Resized event is handled in BackendMainWindow
                
                //Therefore don't need to do anything
                return true;
            
            //Text enter event
            case WM_CHAR:
            {
                wchar_t curChar = msg.wParam;
                InputText += curChar;
                curInfo.CharacterEntered = true;
                curInfo.CurrentCharacterEntered += curChar;
                CurrentInputInfos.push_back(curInfo);
                return true;
            }
        
            //Key down event
            case WM_KEYDOWN:
            {
                ssGUI::Enums::GenericButtonAndKeyInput input = Win32InputConverter::ConvertButtonAndKeys(msg);
                curInfo.CurrentButtonAndKeyChanged = input;
                FetchKeysPressed(input, CurrentKeyPresses);
                CurrentInputInfos.push_back(curInfo);
                return true;
            }

            //Key up event
            case WM_KEYUP:
            {
                ssGUI::Enums::GenericButtonAndKeyInput input = Win32InputConverter::ConvertButtonAndKeys(msg);
                curInfo.CurrentButtonAndKeyChanged = input;
                FetchKeysReleased(input, CurrentKeyPresses);
                CurrentInputInfos.push_back(curInfo);
                return true;
            }
        
            //Mouse button down event
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_XBUTTONDOWN:
            {
                ssGUI::Enums::GenericButtonAndKeyInput input = Win32InputConverter::ConvertMouseButtons(msg);
                curInfo.CurrentButtonAndKeyChanged = input;
                FetchKeysPressed(input, CurrentKeyPresses);
                CurrentInputInfos.push_back(curInfo);
                AddNonExistElement(static_cast<ssGUI::Enums::MouseButton>(input), CurrentMouseButtons);            
                return true;
            }

            //Mouse button up event
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            case WM_XBUTTONUP:
            {
                ssGUI::Enums::GenericButtonAndKeyInput input = Win32InputConverter::ConvertMouseButtons(msg);
                curInfo.CurrentButtonAndKeyChanged = input;
                FetchKeysReleased(input, CurrentKeyPresses);
                CurrentInputInfos.push_back(curInfo);
                RemoveExistElement(static_cast<ssGUI::Enums::MouseButton>(input), CurrentMouseButtons);          
                return true;
            }

            //Mousescroll event
            case WM_MOUSEWHEEL:
                MouseScrollDelta.y = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                return true;
            case WM_MOUSEHWHEEL:
                MouseScrollDelta.x = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                return true;
        }

        return false;
    }

    void BackendSystemInputWin32_OpenGL3_3::UpdateInput(/*std::vector<ssGUI::Backend::BackendMainWindowInterface*>& mainWindows*/)
    {    
        MSG msg = { };

        InputText.clear();
        MouseScrollDelta = glm::vec2();
        
        //Set last key presses and mouse buttons
        LastKeyPresses = CurrentKeyPresses;
        LastMouseButtons = CurrentMouseButtons;
        LastInputInfos = CurrentInputInfos;

        //Get mouse position
        LastMousePosition = CurrentMousePosition;
        
        POINT p;
        if(!GetCursorPos(&p))
        {
            ssLOG_LINE("Failed to get cursor position");
        }
        CurrentMousePosition = glm::ivec2(p.x, p.y);

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);             //Translates any keyboard/text related inputs (if any)
            DispatchMessage(&msg);              //Dispatches the message to the right window(s) (i.e. WM_PAINT)
        }
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputWin32_OpenGL3_3::GetLastButtonAndKeyPresses()
    {        
        return LastKeyPresses;
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputWin32_OpenGL3_3::GetCurrentButtonAndKeyPresses()
    {
        return CurrentKeyPresses;
    }

    bool BackendSystemInputWin32_OpenGL3_3::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return std::find(LastKeyPresses.begin(), LastKeyPresses.end(), input) != LastKeyPresses.end();
    }

    bool BackendSystemInputWin32_OpenGL3_3::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return std::find(CurrentKeyPresses.begin(), CurrentKeyPresses.end(), input) != CurrentKeyPresses.end();
    }

    glm::ivec2 BackendSystemInputWin32_OpenGL3_3::GetLastMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        if(mainWindow != nullptr)
            return LastMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return LastMousePosition;
    }

    glm::ivec2 BackendSystemInputWin32_OpenGL3_3::GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        if(mainWindow != nullptr)
            return CurrentMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return CurrentMousePosition;
    }

    void BackendSystemInputWin32_OpenGL3_3::SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow)
    {
        //Screen pos
        if(mainWindow == nullptr)
        {
            if(!SetCursorPos(position.x, position.y))
            {
                ssLOG_LINE("Failed to set mouse position");
                return;
            }

            CurrentMousePosition = position;
        }
        else
        {
            POINT pt = {position.x, position.y};
            HWND hwnd = static_cast<Win32_OpenGL_Handles*>(mainWindow->GetBackendWindowInterface()->GetRawHandle())->WindowHandle;
            ClientToScreen(hwnd, &pt);
            if(!SetCursorPos(pt.x, pt.y))
            {
                ssLOG_LINE("Failed to set mouse position");
                return;
            }

            CurrentMousePosition = glm::ivec2(pt.x, pt.y);
        }
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
        for(int i = 0; i < CurrentMouseButtons.size(); i++)
            if(CurrentMouseButtons[i] == button)
                return true;
        
        return false;
    }

    glm::vec2 BackendSystemInputWin32_OpenGL3_3::GetCurrentMouseScrollDelta() const
    {
        return MouseScrollDelta;
    }
    
    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputWin32_OpenGL3_3::GetLastRealtimeInputs() const
    {
        return LastInputInfos;
    }
            
    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputWin32_OpenGL3_3::GetCurrentRealtimeInputs() const
    {
        return CurrentInputInfos;
    }

    std::wstring BackendSystemInputWin32_OpenGL3_3::GetTextInput() const
    {
        return InputText;
    }

    void BackendSystemInputWin32_OpenGL3_3::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        CurrentCursor = cursorType;
        //std::cout<<"cursor type: "<<(int)CurrentCursor<<"\n";
    }

    ssGUI::Enums::CursorType BackendSystemInputWin32_OpenGL3_3::GetCursorType() const
    {
        return CurrentCursor;
    }

    //TODO: Store the hotspot and also shouldn't be setting the cursor 
    void BackendSystemInputWin32_OpenGL3_3::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        ssLOG_FUNC_ENTRY();

        //Validation
        if(hotspot.x > cursorSize.x || hotspot.y > cursorSize.y)
        {
            ssLOG_LINE("Invalid hotspot position: "<<hotspot.x<<", "<<hotspot.y);
            ssLOG_FUNC_EXIT();
            return;
        }

        ssGUI::ImageFormat customCursorFormat;
        void* customCursorPtr = customCursor->GetPixelPtr(customCursorFormat);
        if(customCursorPtr == nullptr)
        {
            ssLOG_LINE("Invalid custom cursor image");
            ssLOG_FUNC_EXIT();    
            return;
        }

        //If the cursor already exists, we need to clean up the cursor first
        if(CustomCursors.find(cursorName) != CustomCursors.end())
        {
            DestroyCursor(CustomCursors[cursorName].Win32CursorHandle);
            CustomCursors.erase(cursorName);
        }

        //First we need to convert the image data to BGRA32
        uint8_t* convertedPtr = new uint8_t[customCursor->GetSize().x * customCursor->GetSize().y * 4];
        bool result = ssGUI::ImageUtil::ConvertToBGRA32(convertedPtr, customCursorPtr, customCursorFormat, customCursor->GetSize());

        if(!result)
        {
            delete[] convertedPtr;
            ssLOG_FUNC_EXIT();    
            return;
        }

        CursorData cursorData = 
        {
            std::make_shared<ssGUI::ImageData>(),
            hotspot,
            nullptr
        };

        ssGUI::ImageFormat convertedFormat;
        convertedFormat.IndexR = 2;
        convertedFormat.IndexB = 0;

        //Now we create the custom cursor
        //If the image size fits the cursor size, we just need to set it
        if(customCursor->GetSize() == cursorSize)
        {
            cursorData.CursorImage->LoadRawFromMemory(convertedPtr, convertedFormat, cursorSize);
        }
        //Otherwise, we need to resize it first
        else
        {
            uint8_t* resizedPtr = new uint8_t[cursorSize.x * cursorSize.y * 4];
            ssGUI::ImageUtil::ResizeRGBA(   convertedPtr, 
                                        customCursor->GetSize().x, 
                                        customCursor->GetSize().y,
                                        resizedPtr,
                                        cursorSize.x,
                                        cursorSize.y);
                                        
            cursorData.CursorImage->LoadRawFromMemory(resizedPtr, convertedFormat, cursorSize);

            delete[] resizedPtr;
        }

        //Deallocate convertedPtr
        delete[] convertedPtr;

        //Create cursor
        cursorData.Win32CursorHandle = CreateWin32Cursor(hotspot, cursorData.CursorImage.get());
        if(cursorData.Win32CursorHandle == nullptr)
        {
            ssLOG_LINE("Failed to recreate the cursor");
            ssLOG_FUNC_EXIT();
            return;
        }

        //Store cursor
        CustomCursors[cursorName] = cursorData;

        ssLOG_FUNC_EXIT();    
    }

    void BackendSystemInputWin32_OpenGL3_3::SetCurrentCustomCursor(std::string cursorName)
    {
        if(!HasCustomCursor(cursorName))
            return;
        
        if(CurrentCustomCursor == cursorName)
            return;

        CurrentCustomCursor = cursorName;
    }

    void BackendSystemInputWin32_OpenGL3_3::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {        
        if(CurrentCustomCursor.empty())
            return;

        GetCustomCursor(customCursor, CurrentCustomCursor, hotspot);
    }

    std::string BackendSystemInputWin32_OpenGL3_3::GetCurrentCustomCursorName()
    {
        return CurrentCustomCursor;
    }

    void BackendSystemInputWin32_OpenGL3_3::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {
        if(CustomCursors.find(cursorName) == CustomCursors.end())
            return;

        ssGUI::ImageFormat imgFormat;

        if(CustomCursors[cursorName].CursorImage->GetPixelPtr(imgFormat) == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory( CustomCursors[cursorName].CursorImage->GetPixelPtr(imgFormat),
                                            imgFormat,
                                            CustomCursors[cursorName].CursorImage->GetSize()))
        {
            ssLOG_LINE("Failed to load custom cursor image");
            return;
        }

        hotspot = CustomCursors[cursorName].Hotspot;
    }

    bool BackendSystemInputWin32_OpenGL3_3::HasCustomCursor(std::string cursorName)
    {
        return CustomCursors.find(cursorName) != CustomCursors.end();
    }

    void BackendSystemInputWin32_OpenGL3_3::UpdateCursor()
    {
        ssLOG_FUNC_ENTRY();

        //For each main window, we need to update the cursor
        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            if(ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->IsClosed())
                continue;            
            
            HWND mainWindowHandle = static_cast<Win32_OpenGL_Handles*>
                                    (BackendManager::GetMainWindowInterface(i)->GetRawHandle())->WindowHandle;
            
            #define SSGUI_SHOW_WIN32_CURSORS(win32Cursor)\
            {\
                HCURSOR cursor = LoadCursor(NULL, win32Cursor);\
                if(cursor == NULL)\
                {\
                    ssLOG_LINE("Failed to load cursor");\
                    break;\
                }\
                SetClassLongPtr(mainWindowHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(cursor));\
                break;\
            }

            switch (CurrentCursor)
            {
                case ssGUI::Enums::CursorType::NONE:
                {
                    CreateEmptyCursorIfNeeded();
                    SetClassLongPtr(mainWindowHandle, 
                                    GCLP_HCURSOR, 
                                    reinterpret_cast<LONG_PTR>(CustomCursors[SSGUI_EMPTY_CURSOR].Win32CursorHandle));
                    break;
                }
                case ssGUI::Enums::CursorType::NORMAL:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_ARROW)
                case ssGUI::Enums::CursorType::TEXT:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_IBEAM)
                case ssGUI::Enums::CursorType::HAND:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_HAND)
                case ssGUI::Enums::CursorType::RESIZE_LEFT:
                case ssGUI::Enums::CursorType::RESIZE_RIGHT:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_SIZEWE)
                case ssGUI::Enums::CursorType::RESIZE_UP:
                case ssGUI::Enums::CursorType::RESIZE_DOWN:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_SIZENS)
                case ssGUI::Enums::CursorType::RESIZE_TOP_LEFT:
                case ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_SIZENWSE)
                case ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT:
                case ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_SIZENESW)
                case ssGUI::Enums::CursorType::MOVE:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_SIZEALL)
                case ssGUI::Enums::CursorType::HELP:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_HELP)
                case ssGUI::Enums::CursorType::NOT_ALLOWED:
                    SSGUI_SHOW_WIN32_CURSORS(IDC_NO)
                case ssGUI::Enums::CursorType::CUSTOM:
                {
                    ssGUI::ImageFormat customCursorFormat;
                    if( !CurrentCustomCursor.empty() && 
                        CustomCursors[CurrentCustomCursor].CursorImage->GetPixelPtr(customCursorFormat) != nullptr &&
                        CustomCursors[CurrentCustomCursor].Win32CursorHandle != nullptr &&
                        CustomCursors[CurrentCustomCursor].Win32CursorHandle != NULL)
                    {
                        SetClassLongPtr(mainWindowHandle, 
                                        GCLP_HCURSOR, 
                                        reinterpret_cast<LONG_PTR>(CustomCursors[CurrentCustomCursor].Win32CursorHandle));
                        break;
                    }
                    else
                    {
                        ssLOG_LINE("Failed to load cursor");
                        break;
                    }
                }
                default:
                    ssLOG_LINE("Unimplemented Cursor");
                    ssLOG_EXIT_PROGRAM();
            }

            #undef SSGUI_SHOW_WIN32_CURSORS
        }

        ssLOG_FUNC_EXIT();
    }

    bool BackendSystemInputWin32_OpenGL3_3::ClearClipboard()
    {
        return clip::clear();
    }

    bool BackendSystemInputWin32_OpenGL3_3::ClipbaordHasText()
    {
        return clip::has(clip::text_format());
    }
            
    bool BackendSystemInputWin32_OpenGL3_3::ClipbaordHasImage()
    {
        return clip::has(clip::image_format());
    }

    bool BackendSystemInputWin32_OpenGL3_3::SetClipboardImage(const ssGUI::ImageData& imgData)
    {
        if(!imgData.IsValid())
            return false;

        ssGUI::ImageFormat format;
        void* oriImgPtr = imgData.GetBackendImageInterface()->GetPixelPtr(format);

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

        uint8_t* imgPtr = new uint8_t[imgData.GetSize().x * imgData.GetSize().y * 4];
        bool result = false;

        //TODO: Move this to somewhere else
        switch(format.BitDepthPerChannel)
        {
            case 8:
                result = ssGUI::ImageUtil::ConvertToRGBA32<uint8_t>(imgPtr, oriImgPtr, format, imgData.GetSize());
                break;
            case 16:
                result = ssGUI::ImageUtil::ConvertToRGBA32<uint16_t>(imgPtr, oriImgPtr, format, imgData.GetSize());
                break;
            default:
                ssLOG_LINE("Unsupported bitdepth: " << format.BitDepthPerChannel);
                delete[] imgPtr;
                return false;
        }

        if(!result)
            return false;

        clip::image clipImg = clip::image(static_cast<const void*>(imgPtr), spec);
        result = clip::set_image(clipImg);
        delete[] imgPtr;
        return result;
    }
            
    bool BackendSystemInputWin32_OpenGL3_3::SetClipboardText(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        return clip::set_text(converter.to_bytes(str));
    }
            
    bool BackendSystemInputWin32_OpenGL3_3::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        clip::image img;

        if(!clip::get_image(img) || img.spec().bits_per_pixel != 32)
            return false;

        ssGUI::ImageFormat format;

        return imgData.LoadRawFromMemory(static_cast<void*>(img.data()), format, glm::ivec2(img.spec().width, img.spec().height));
    }

    bool BackendSystemInputWin32_OpenGL3_3::GetClipboardText(std::wstring& str)
    {
        std::string temp;
        if(!clip::get_text(temp))
            return false;

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        str = converter.from_bytes(temp);
        return true;
    }

    uint64_t BackendSystemInputWin32_OpenGL3_3::GetElapsedTime() const
    {
        using sysClock = std::chrono::high_resolution_clock;
        return std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::duration(sysClock::now() - StartTime)).count();
    }
}

}
