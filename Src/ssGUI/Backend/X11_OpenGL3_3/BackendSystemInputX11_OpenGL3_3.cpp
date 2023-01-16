#include "ssGUI/Backend/X11_OpenGL3_3/BackendSystemInputX11_OpenGL3_3.hpp"

#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/Xfixes.h>
#include <X11/Xlib.h>

#include "clip.h"
#include "ssGUI/Backend/X11_OpenGL3_3/BackendMainWindowX11_OpenGL3_3.hpp"

#include "ssGUI/Backend/X11_OpenGL3_3/X11InputConverter.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"        //For getting cursor in MainWindow space
#include "ssGUI/HelperClasses/ImageUtil.hpp"
#include "ssLogger/ssLog.hpp"


#include <algorithm>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    template <class T>
    void BackendSystemInputX11_OpenGL3_3::AddNonExistElement(T elementToAdd, std::vector<T>& vectorAddTo)
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
    void BackendSystemInputX11_OpenGL3_3::RemoveExistElement(T elementToRemove, std::vector<T>& vectorRemoveFrom)
    {
        ssLOG_FUNC_ENTRY();
        
        typename std::vector<T>::iterator foundElement =
            std::find_if(vectorRemoveFrom.begin(), vectorRemoveFrom.end(), [&elementToRemove](T key){return elementToRemove == key;});

        if(foundElement != vectorRemoveFrom.end())
            vectorRemoveFrom.erase(foundElement);

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputX11_OpenGL3_3::FetchKeysPressed(ssGUI::Enums::GenericButtonAndKeyInput keysPressedDown, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses)
    {
        ssLOG_FUNC_ENTRY();
        auto it = std::find(destinationKeyPresses.begin(), destinationKeyPresses.end(), keysPressedDown);
        if(it == destinationKeyPresses.end())
            destinationKeyPresses.push_back(keysPressedDown);

        ssLOG_FUNC_EXIT();
    }

    void BackendSystemInputX11_OpenGL3_3::FetchKeysReleased(ssGUI::Enums::GenericButtonAndKeyInput keysReleased, std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& destinationKeyPresses)
    {
        ssLOG_FUNC_ENTRY();        
        auto it = std::find(destinationKeyPresses.begin(), destinationKeyPresses.end(), keysReleased);
        if(it != destinationKeyPresses.end())
            destinationKeyPresses.erase(it);

        ssLOG_FUNC_EXIT();
    }
    
    bool BackendSystemInputX11_OpenGL3_3::PopulateCursorDataHandles(CursorData& cursorData)
    {
        ssGUI::ImageFormat format;
        uint8_t* imgPtr = static_cast<uint8_t*>(cursorData.CursorImage->GetPixelPtr(format));

        XcursorImage* cursorImg = XcursorImageCreate(cursorData.CursorImage->GetSize().x, cursorData.CursorImage->GetSize().y);
        if(cursorImg == nullptr)
        {
            ssLOG_LINE("Failed to create cursor image");
            XcursorImageDestroy(cursorImg);
            return false;
        }

        cursorImg->xhot = cursorData.Hotspot.x;
        cursorImg->yhot = cursorData.Hotspot.y;
        cursorImg->pixels = reinterpret_cast<XcursorPixel*>(imgPtr);

        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            ssGUI::Backend::BackendMainWindowInterface* win = ssGUI::Backend::BackendManager::GetMainWindowInterface(i);
            if(win->IsClosed())
                continue;

            ssGUI::Backend::X11RawHandle* curRawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(win->GetRawHandle());
            if(cursorData.X11CursorHandles.find(curRawHandle->WindowDisplay) != cursorData.X11CursorHandles.end())
                continue;
            
            Cursor currentCursor = XcursorImageLoadCursor(curRawHandle->WindowDisplay, cursorImg);
            
            if(currentCursor == None)
            {
                ssLOG_LINE("Failed to load cursor from image");
                XcursorImageDestroy(cursorImg);
                return false;
            }
            
            cursorData.X11CursorHandles[curRawHandle->WindowDisplay] = currentCursor;
        }
        
        XcursorImageDestroy(cursorImg);   
        return true;
    }

    BackendSystemInputX11_OpenGL3_3::BackendSystemInputX11_OpenGL3_3(BackendSystemInputX11_OpenGL3_3 const& other)
    {
        ssLOG_LINE("Not implemented");
        ssLOG_EXIT_PROGRAM();
    }

    BackendSystemInputX11_OpenGL3_3::BackendSystemInputX11_OpenGL3_3() :    CurrentEvents(),
                                                                            CurrentKeyPresses(),
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
                                                                            CustomCursors(),
                                                                            CurrentCustomCursor(),
                                                                            StartTime()
    {
        StartTime = std::chrono::high_resolution_clock::now();
        ssGUI::Backend::BackendManager::AddInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }

    BackendSystemInputX11_OpenGL3_3::~BackendSystemInputX11_OpenGL3_3()
    {
        //TODO: Free cursors
        ssGUI::Backend::BackendManager::RemoveInputInterface(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(this));
    }
    
    void BackendSystemInputX11_OpenGL3_3::FetchEvents()
    {
        //Get text input
        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            X11RawHandle* rawHandle = static_cast<X11RawHandle*>(
                                        ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->GetRawHandle());
        
            while (XPending(rawHandle->WindowDisplay))
            {
                XEvent xev;
                XNextEvent(rawHandle->WindowDisplay, &xev);
                
                CurrentEvents.push_back(xev);
            }
        }
    }
    
    void BackendSystemInputX11_OpenGL3_3::UpdateInput()
    {
        FetchEvents();

        ssGUI::Backend::X11RawHandle* rawHandle = nullptr;
        bool customRawHandle = false;
        
        if(ssGUI::Backend::BackendManager::GetMainWindowCount() > 0)
        {
            rawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(0)->GetRawHandle()); 
        }
        else
        {
            static bool reminder = false;
            if(!reminder)
            {
                ssLOG_LINE("BackendSystemInputX11_OpenGL3_3 is not designed to use without any main window, this could fail");
                reminder = true;
            }
                 
            rawHandle = new ssGUI::Backend::X11RawHandle();
            rawHandle->WindowDisplay = XOpenDisplay(NULL);
            rawHandle->WindowColormap = 0;
            rawHandle->WindowId = RootWindow(rawHandle->WindowDisplay, DefaultScreen(rawHandle->WindowDisplay));
            rawHandle->WindowContext = nullptr;
            rawHandle->WindowCloseEventId = 0;
            rawHandle->XInputContext = nullptr;
        }
        
        InputText.clear();
        MouseScrollDelta = glm::vec2();
        
        //Set last key presses and mouse buttons
        LastKeyPresses = CurrentKeyPresses;
        LastMouseButtons = CurrentMouseButtons;
        LastInputInfos = CurrentInputInfos;
        
        ::Window rootReturned;
        ::Window windowReturned;
        int rootX, rootY, windowX, windowY;
        unsigned int maskReturn;
        
        //Get mouse position
        //https://stackoverflow.com/questions/3585871/how-can-i-get-the-current-mouse-pointer-position-co-ordinates-in-x
        int result = XQueryPointer( rawHandle->WindowDisplay, 
                                    RootWindow(rawHandle->WindowDisplay, DefaultScreen(rawHandle->WindowDisplay)), 
                                    &rootReturned,
                                    &windowReturned, 
                                    &rootX, 
                                    &rootY, 
                                    &windowX, 
                                    &windowY,
                                    &maskReturn);

        if(result != True)
        {
            ssLOG_LINE("Failed to get mouse cursor");
        }

        //Deallocating customRawHandle if there's no main window
        if(customRawHandle)
        {
            delete rawHandle;
        }
        
        for(int i = 0; i < CurrentEvents.size(); i++)
        {
            ssGUI::RealtimeInputInfo curInfo;
            
            if(XFilterEvent(&CurrentEvents[i], None) == True)
                continue;

            switch(CurrentEvents[i].type)
            {
                //Mouse moved event
                case MotionNotify:
                {
                    curInfo.MouseMoved = true;
                    curInfo.CurrentMousePosition = glm::ivec2(CurrentEvents[i].xmotion.x_root, CurrentEvents[i].xmotion.y_root);
                    CurrentInputInfos.push_back(curInfo);
                    break;  
                }
                //Close event
                case ClientMessage:
                {
                    for(int j = 0; j < ssGUI::Backend::BackendManager::GetMainWindowCount(); j++)
                    {
                        X11RawHandle* curHandle = static_cast<X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->GetRawHandle());
                        
                        if((Atom)CurrentEvents[i].xclient.data.l[0] == curHandle->WindowCloseEventId)
                        {    
                            //if(curHandle->WindowId == CurrentEvents[i].xclient.window)
                            //{
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->Close();
                            break;
                            //}
                        }
                    }
                    break;
                }
                //Focus lost event
                case FocusOut:
                {
                    for(int j = 0; j < ssGUI::Backend::BackendManager::GetMainWindowCount(); j++)
                    {
                        X11RawHandle* curHandle = static_cast<X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->GetRawHandle());
                        
                        if((Atom)CurrentEvents[i].xfocus.window == curHandle->WindowId)
                        {
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->SetFocus(false, true);
                            break;
                        }
                    }
                    break;
                }
                
                //Focus Gained event
                case FocusIn:
                {
                    for(int j = 0; j < ssGUI::Backend::BackendManager::GetMainWindowCount(); j++)
                    {
                        X11RawHandle* curHandle = static_cast<X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->GetRawHandle());
                        
                        if((Atom)CurrentEvents[i].xfocus.window == curHandle->WindowId)
                        {
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->SetFocus(true, true);
                            break;
                        }
                    }
                    break;
                }
                
                //Resize event
                    //TODO: Maybe handle this in backend draw?
                
                
                //Text enter event
                //Key down event
                case KeyPress:
                {
                    X11RawHandle* curHandle = nullptr;
                    for(int j = 0; j < ssGUI::Backend::BackendManager::GetMainWindowCount(); j++)
                    {
                        curHandle = static_cast<X11RawHandle*>(
                            ssGUI::Backend::BackendManager::GetMainWindowInterface(j)->GetRawHandle());
                        
                        if((Atom)CurrentEvents[i].xkey.window == curHandle->WindowId)   
                            break;
                    }
                
                    int bufferLength = 0;
                    char* buffer = nullptr;
                    KeySym keysym;
                    Status status;
                    int len = 0; 
                    
                    do
                    {
                        if(bufferLength > 0)
                            delete[] buffer;
                            
                        bufferLength += 32;
                        buffer = new char[bufferLength];
                        len = Xutf8LookupString(curHandle->XInputContext, &CurrentEvents[i].xkey, buffer, bufferLength - 1, &keysym, &status);
                    }
                    while(status == XBufferOverflow);

                    if (len > 0 && (status == XLookupBoth || status == XLookupChars))
                    {
                        // Character input
                        //ssLOG_LINE("Character input: "<< buffer);
                        
                        //TODO: Move to set IME position function
                        //XVaNestedList preedit_attr;
                        //XPoint nspot;
                        //nspot.x = 50;
                        //nspot.y = 50;
                        //preedit_attr = XVaCreateNestedList(0, XNSpotLocation, &nspot, NULL);
                        //XSetICValues(curHandle->XInputContext, XNPreeditAttributes, preedit_attr, NULL);
                        //XFree(preedit_attr);
                        
                        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                        std::wstring charactersEntered = converter.from_bytes(buffer);
                        InputText += charactersEntered;
                        
                        //Push each character to Realtime infos
                        for(int j = 0; j < charactersEntered.size(); j++)
                        {
                            curInfo.CharacterEntered = true;
                            curInfo.CurrentCharacterEntered += charactersEntered[j];
                            CurrentInputInfos.push_back(curInfo);
                            curInfo = ssGUI::RealtimeInputInfo();
                        }
                    }
                    
                    if(status == XLookupBoth || status == XLookupKeySym)
                    {
                        //ssLOG_LINE("Key input: "<<keysym);
                        ssGUI::Enums::GenericButtonAndKeyInput input = X11InputConverter::ConvertButtonAndKeys(XLookupKeysym(&CurrentEvents[i].xkey, 0));
                        curInfo.CurrentButtonAndKeyChanged = input;
                        FetchKeysPressed(input, CurrentKeyPresses);
                        CurrentInputInfos.push_back(curInfo);
                    }
                    
                    break;
                }
                
                //Key up event
                case KeyRelease:
                {
                    bool repeatKey = false;
                    if(i < CurrentEvents.size() - 1)
                    {
                        XEvent nev = CurrentEvents[i + 1];
                        if (nev.type == KeyPress || nev.xkey.time == CurrentEvents[i].xkey.time ||
                            nev.xkey.keycode == CurrentEvents[i].xkey.keycode)
                        {
                            repeatKey = true;
                        }
                    }
                
                    if(!repeatKey)
                    {
                        ssGUI::Enums::GenericButtonAndKeyInput input = X11InputConverter::ConvertButtonAndKeys(XLookupKeysym(&CurrentEvents[i].xkey, 0));
                        curInfo.CurrentButtonAndKeyChanged = input;
                        FetchKeysReleased(input, CurrentKeyPresses);
                        CurrentInputInfos.push_back(curInfo);
                    }
                    break;
                }
                
                //Mouse button down event
                case ButtonPress:
                    //Mousescroll event
                    //Scroll up
                    if(CurrentEvents[i].xbutton.button == Button4)
                        MouseScrollDelta.y = 1;
                    //Scroll down
                    else if(CurrentEvents[i].xbutton.button == Button5)
                        MouseScrollDelta.y = -1;
                    //Scroll left
                    else if(CurrentEvents[i].xbutton.button == 6)
                        MouseScrollDelta.x = -1;
                    //Scroll right
                    else if(CurrentEvents[i].xbutton.button == 7)
                        MouseScrollDelta.x = 1;
                    else
                    {
                        ssGUI::Enums::GenericButtonAndKeyInput input = X11InputConverter::ConvertMouseButtons(CurrentEvents[i]);
                        curInfo.CurrentButtonAndKeyChanged = input;
                        FetchKeysPressed(input, CurrentKeyPresses);
                        CurrentInputInfos.push_back(curInfo);
                        AddNonExistElement(static_cast<ssGUI::Enums::MouseButton>(input), CurrentMouseButtons);
                    }
                    break;
                //Mouse button up event
                case ButtonRelease:
                {
                    ssGUI::Enums::GenericButtonAndKeyInput input = X11InputConverter::ConvertMouseButtons(CurrentEvents[i]);
                    curInfo.CurrentButtonAndKeyChanged = input;
                    FetchKeysReleased(input, CurrentKeyPresses);
                    CurrentInputInfos.push_back(curInfo);
                    RemoveExistElement(static_cast<ssGUI::Enums::MouseButton>(input), CurrentMouseButtons);      
                    break;
                }
                
                //Xlib: Refresh Keyboard mapping
                case MappingNotify:
                    // Handle mapping notify event
                    XRefreshKeyboardMapping(&CurrentEvents[i].xmapping);
                    break;
            }
            
            CurrentEvents.clear();
        }
        
        
        //while (XPending(rawHandle->WindowDisplay)) 
        //{
        //    XEvent xev;
        //    XNextEvent(rawHandle->WindowDisplay, &xev);

        //    if (xev.type == KeyPress) {
        //        ssLOG_LINE("Key pressed");
        //    }
            
        //    else if(xev.type == ClientMessage && (Atom)xev.xclient.data.l[0] == rawHandle->WindowCloseEventId)
        //    {
        //        closeWindow = true;
        //        ssLOG_LINE("Window closing");
        //        break;
        //    }
        //    //if(xev.type == )
        //}
        
        //if(closeWindow)
        //    ssGUI::Backend::BackendManager::GetMainWindowInterface(0)->Close();
    }

    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputX11_OpenGL3_3::GetLastButtonAndKeyPresses()
    {
        return LastKeyPresses;
    }
    
    const std::vector<ssGUI::Enums::GenericButtonAndKeyInput>& BackendSystemInputX11_OpenGL3_3::GetCurrentButtonAndKeyPresses()
    {
        return CurrentKeyPresses;
    }

    bool BackendSystemInputX11_OpenGL3_3::IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return std::find(LastKeyPresses.begin(), LastKeyPresses.end(), input) != LastKeyPresses.end();
    }

    bool BackendSystemInputX11_OpenGL3_3::IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return std::find(CurrentKeyPresses.begin(), CurrentKeyPresses.end(), input) != CurrentKeyPresses.end();
    }

    glm::ivec2 BackendSystemInputX11_OpenGL3_3::GetLastMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        if(mainWindow != nullptr)
            return LastMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return LastMousePosition;
    }
    
    glm::ivec2 BackendSystemInputX11_OpenGL3_3::GetCurrentMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        if(mainWindow != nullptr)
            return CurrentMousePosition - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
        else
            return CurrentMousePosition;
    }
    
    void BackendSystemInputX11_OpenGL3_3::SetMousePosition(glm::ivec2 position, ssGUI::MainWindow* mainWindow)
    {
        ssGUI::Backend::BackendMainWindowInterface* mainWindowInterface = nullptr;
    
        if(mainWindow == nullptr)
        {
            if(ssGUI::Backend::BackendManager::GetMainWindowCount() == 0)
            {
                ssLOG_LINE("Unable to set mouse position without any window.");
                return;
            }
            else
                mainWindowInterface = ssGUI::Backend::BackendManager::GetMainWindowInterface(0);
        }

        ssGUI::Backend::X11RawHandle* rawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(mainWindowInterface->GetRawHandle());
    
        if(!XWarpPointer(   rawHandle->WindowDisplay, 
                            None, 
                            mainWindow == nullptr ? RootWindow(rawHandle->WindowDisplay, rawHandle->WindowId) : rawHandle->WindowId, 
                            0, 
                            0, 
                            0, 
                            0, 
                            position.x, 
                            position.y))
        {
            ssLOG_LINE("XWarpPointer failed");
        }
    }

    bool BackendSystemInputX11_OpenGL3_3::GetLastMouseButton(ssGUI::Enums::MouseButton button) const
    {
        for(int i = 0; i < LastMouseButtons.size(); i++)
            if(LastMouseButtons[i] == button)
                return true;
        
        return false;
    }
    
    bool BackendSystemInputX11_OpenGL3_3::GetCurrentMouseButton(ssGUI::Enums::MouseButton button) const
    {
        for(int i = 0; i < CurrentMouseButtons.size(); i++)
            if(CurrentMouseButtons[i] == button)
                return true;
        
        return false;
    }

    glm::vec2 BackendSystemInputX11_OpenGL3_3::GetCurrentMouseScrollDelta() const
    {
        return MouseScrollDelta;
    }

    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputX11_OpenGL3_3::GetLastRealtimeInputs() const
    {
        return LastInputInfos;
    }

    std::vector<ssGUI::RealtimeInputInfo> const & BackendSystemInputX11_OpenGL3_3::GetCurrentRealtimeInputs() const
    {
        return CurrentInputInfos;
    }

    std::wstring BackendSystemInputX11_OpenGL3_3::GetTextInput() const
    {
        return InputText;
    }
    
    void BackendSystemInputX11_OpenGL3_3::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        CurrentCursor = cursorType;
    }

    ssGUI::Enums::CursorType BackendSystemInputX11_OpenGL3_3::GetCursorType() const
    {
        return CurrentCursor;
    }

    void BackendSystemInputX11_OpenGL3_3::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        //Validation
        if(hotspot.x > cursorSize.x || hotspot.y > cursorSize.y)
        {
            ssLOG_LINE("Invalid hotspot position: "<<hotspot.x<<", "<<hotspot.y);
            return;
        }
        
        ssGUI::ImageFormat customCursorFormat;
        void* customCursorPtr = customCursor->GetPixelPtr(customCursorFormat);
        if(customCursorPtr == nullptr)
        {
            ssLOG_LINE("Invalid custom cursor image");
            return;
        }

        //If the cursor already exists, we need to clean up the cursor first
        if(CustomCursors.find(cursorName) != CustomCursors.end())
        {
            CursorData& currentData = CustomCursors[cursorName];
            for(auto it = currentData.X11CursorHandles.begin(); it != currentData.X11CursorHandles.end(); it++)
                XFreeCursor(it->first, it->second);
            
            CustomCursors.erase(cursorName);
        }

        CursorData& cursorData = CustomCursors[cursorName] = 
        {
            std::make_shared<ssGUI::ImageData>(),
            hotspot,
            std::unordered_map<Display*, Cursor>()
        };

        //If the image size fits the cursor size, we just need to convert it to ARGB and set it
        if(customCursor->GetSize() == cursorSize)
        {
            uint8_t* convertedPtr = new uint8_t[customCursor->GetSize().x * customCursor->GetSize().y * 4];
            bool result = ssGUI::ImageUtil::ConvertToARGB32(convertedPtr, customCursorPtr, customCursorFormat, customCursor->GetSize());
        
            if(!result)
            {
                ssLOG_LINE("Conversion failed");
                delete[] convertedPtr;
                return;
            }
        
            ssGUI::ImageFormat convertedFormat;
            convertedFormat.IndexA = 0;
            convertedFormat.IndexR = 1;
            convertedFormat.IndexR = 2;
            convertedFormat.IndexB = 3;    
            cursorData.CursorImage->LoadRawFromMemory(convertedPtr, convertedFormat, cursorSize);
            cursorData.Hotspot = hotspot;
            delete[] convertedPtr;
            
            result = PopulateCursorDataHandles(cursorData);
            
            if(!result)
                return;
        }
        //Otherwise we need to convert it to RGBA first, then resize itm then convert it to ARGB
        else
        {
            uint8_t* convertedPtr = new uint8_t[customCursor->GetSize().x * customCursor->GetSize().y * 4];
            bool result = ssGUI::ImageUtil::ConvertToRGBA32(convertedPtr, customCursorPtr, customCursorFormat, customCursor->GetSize());
         
            if(!result)
            {
                ssLOG_LINE("Conversion failed");
                delete[] convertedPtr;
                return;
            }
            
            uint8_t* resizedPtr = new uint8_t[cursorSize.x * cursorSize.y * 4];
            ssGUI::ImageUtil::ResizeRGBA(convertedPtr, customCursor->GetSize().x, customCursor->GetSize().y, resizedPtr, cursorSize.x, cursorSize.y);
            
            delete[] convertedPtr;
            convertedPtr = new uint8_t[cursorSize.x * cursorSize.y * 4];
            
            result = ssGUI::ImageUtil::ConvertToARGB32(convertedPtr, resizedPtr, ssGUI::ImageFormat(), cursorSize);
            if(!result)
            {
                ssLOG_LINE("Conversion failed");
                delete[] resizedPtr;
                delete[] convertedPtr;
                return;
            }
            
            ssGUI::ImageFormat convertedFormat;
            convertedFormat.IndexA = 0;
            convertedFormat.IndexR = 1;
            convertedFormat.IndexR = 2;
            convertedFormat.IndexB = 3;    
            cursorData.CursorImage->LoadRawFromMemory(convertedPtr, convertedFormat, cursorSize);
            cursorData.Hotspot = hotspot;
            
            delete[] convertedPtr;
            delete[] resizedPtr;
            
            result = PopulateCursorDataHandles(cursorData);
            
            if(!result)
                return;
        }
    }
    
    void BackendSystemInputX11_OpenGL3_3::SetCurrentCustomCursor(std::string cursorName)
    {
        if(!HasCustomCursor(cursorName))
            return;
        
        if(CurrentCustomCursor == cursorName)
            return;

        CurrentCustomCursor = cursorName;
    }

    void BackendSystemInputX11_OpenGL3_3::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {
        if(CurrentCustomCursor.empty())
            return;

        GetCustomCursor(customCursor, CurrentCustomCursor, hotspot);
    }

    std::string BackendSystemInputX11_OpenGL3_3::GetCurrentCustomCursorName()
    {
        return CurrentCustomCursor;
    }
    
    void BackendSystemInputX11_OpenGL3_3::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
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

    bool BackendSystemInputX11_OpenGL3_3::HasCustomCursor(std::string cursorName)
    {
        return CustomCursors.find(cursorName) != CustomCursors.end();
    }

    void BackendSystemInputX11_OpenGL3_3::UpdateCursor()
    {
        for(int i = 0; i < ssGUI::Backend::BackendManager::GetMainWindowCount(); i++)
        {
            if(ssGUI::Backend::BackendManager::GetMainWindowInterface(i)->IsClosed())
                continue;
        
            ssGUI::Backend::BackendMainWindowInterface* curMainWindow = ssGUI::Backend::BackendManager::GetMainWindowInterface(i);
        
            ssGUI::Backend::X11RawHandle* rawHandle = static_cast<ssGUI::Backend::X11RawHandle*>(curMainWindow->GetRawHandle());
        
            Cursor cursor = None;
        
            switch (CurrentCursor)
            {
                case ssGUI::Enums::CursorType::NONE:
                    break;
                case ssGUI::Enums::CursorType::NORMAL:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_left_ptr);
                    break;
                case ssGUI::Enums::CursorType::TEXT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_xterm);
                    break;
                case ssGUI::Enums::CursorType::HAND:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_fleur);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_LEFT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_left_side);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_RIGHT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_right_side);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_UP:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_top_side);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_DOWN:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_bottom_side);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_TOP_LEFT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_top_left_corner);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_BOTTOM_RIGHT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_bottom_right_corner);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_TOP_RIGHT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_top_right_corner);
                    break;
                case ssGUI::Enums::CursorType::RESIZE_BOTTOM_LEFT:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_bottom_left_corner);
                    break;
                case ssGUI::Enums::CursorType::MOVE:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_fleur);
                    break;
                case ssGUI::Enums::CursorType::HELP:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_question_arrow);
                    break;
                case ssGUI::Enums::CursorType::NOT_ALLOWED:
                    cursor = XCreateFontCursor(rawHandle->WindowDisplay, XC_X_cursor);
                    break;
                case ssGUI::Enums::CursorType::CUSTOM:
                {
                    CursorData& currentCursorData = CustomCursors[CurrentCustomCursor];
                    
                    //If this is a new window that the custom cursor didn't add, add it
                    if(currentCursorData.X11CursorHandles.find(rawHandle->WindowDisplay) == currentCursorData.X11CursorHandles.end())
                    {
                        bool result = PopulateCursorDataHandles(currentCursorData);
                        if(!result)
                            continue;
                    }
                    
                    cursor = currentCursorData.X11CursorHandles[rawHandle->WindowDisplay];
                    break;
                }
                default:
                    ssLOG_LINE("Unimplemented Cursor");
                    ssLOG_EXIT_PROGRAM();
            }
            
            if(CurrentCursor == ssGUI::Enums::CursorType::NONE)
            {
                XFixesHideCursor(rawHandle->WindowDisplay, rawHandle->WindowId);
                XFlush(rawHandle->WindowDisplay);
            }
            else
            {
                XFixesShowCursor(rawHandle->WindowDisplay, rawHandle->WindowId);
                XFlush(rawHandle->WindowDisplay);
                XDefineCursor(rawHandle->WindowDisplay, rawHandle->WindowId, cursor);        
            }
        }
    }

    bool BackendSystemInputX11_OpenGL3_3::ClearClipboard()
    {
        return clip::clear();
    }

    bool BackendSystemInputX11_OpenGL3_3::ClipbaordHasText()
    {
        return clip::has(clip::text_format());
    }
    
    bool BackendSystemInputX11_OpenGL3_3::ClipbaordHasImage()
    {
        return clip::has(clip::image_format());
    }

    bool BackendSystemInputX11_OpenGL3_3::SetClipboardImage(const ssGUI::ImageData& imgData)
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

        result = ssGUI::ImageUtil::ConvertToRGBA32(imgPtr, oriImgPtr, format, imgData.GetSize());

        if(!result)
            return false;

        clip::image clipImg = clip::image(static_cast<const void*>(imgPtr), spec);
        result = clip::set_image(clipImg);
        delete[] imgPtr;
        return result;
    }
    
    bool BackendSystemInputX11_OpenGL3_3::SetClipboardText(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        return clip::set_text(converter.to_bytes(str));
    }
    
    bool BackendSystemInputX11_OpenGL3_3::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        clip::image img;

        if(!clip::get_image(img) || img.spec().bits_per_pixel != 32)
            return false;

        ssGUI::ImageFormat format;

        return imgData.LoadRawFromMemory(static_cast<void*>(img.data()), format, glm::ivec2(img.spec().width, img.spec().height));
    }

    bool BackendSystemInputX11_OpenGL3_3::GetClipboardText(std::wstring& str)
    {
        std::string temp;
        if(!clip::get_text(temp))
            return false;

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        str = converter.from_bytes(temp);
        return true;
    }

    uint64_t BackendSystemInputX11_OpenGL3_3::GetElapsedTime() const
    {
        using sysClock = std::chrono::high_resolution_clock;
        return std::chrono::duration_cast<std::chrono::milliseconds>(sysClock::duration(sysClock::now() - StartTime)).count();
    }
}

}