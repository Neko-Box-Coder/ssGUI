#include "ssGUI/Backend/SDL2/SystemInputSDL2.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/HelperClasses/GenericInputToString.hpp"
#include "ssGUI/Backend/SDL2/SDL2InputConverter.hpp"
#include "ssGUI/Backend/Factory.hpp"
#include "ssGUI/HelperClasses/ImageUtil.hpp"
#include "ssGUI/Backend/SDL2/MainWindowSDL2.hpp"

#include "clip.h"
#include "SDL.h"
#include "SDL_syswm.h"

//#include "clip.h"

#include <algorithm>
#include <codecvt>

namespace ssGUI
{

namespace Backend
{
    SystemInputSDL2::SystemInputSDL2(SystemInputSDL2 const& other) : 
        CurrentInputs(other.CurrentInputs),
        LastInputs(other.LastInputs),
        CurrentRealtimeInputs(other.CurrentRealtimeInputs),
        LastRealtimeInputs(other.LastRealtimeInputs),
        LastMousePosition(other.LastMousePosition),
        CurrentMousePosition(other.CurrentMousePosition),
        CurrentMouseScrollDelta(other.CurrentMouseScrollDelta),
        CurrentTextInput(other.CurrentTextInput),
        CurrentCursorType(other.CurrentCursorType),
        CurrentSDLCursor(other.CurrentSDLCursor),
        CurrentCustomCursorName(other.CurrentCustomCursorName),
        CustomCursors(other.CustomCursors),
        RawEventHandlers(other.RawEventHandlers),
        StartTime(other.StartTime)
    {
    }

    void SystemInputSDL2::FetchKeysPressed(  Enums::GenericInput keysPressedDown, 
                                                    std::vector<Enums::GenericInput>& destinationKeyPresses)
    {
        auto it = std::find(destinationKeyPresses.begin(), 
                            destinationKeyPresses.end(), 
                            keysPressedDown);

        if(it == destinationKeyPresses.end())
            destinationKeyPresses.push_back(keysPressedDown);
    }

    void SystemInputSDL2::FetchKeysReleased( Enums::GenericInput keysReleased, 
                                                    std::vector<Enums::GenericInput>& destinationKeyPresses)
    {
        auto it = std::find(destinationKeyPresses.begin(), 
                            destinationKeyPresses.end(), 
                            keysReleased);
        
        if(it != destinationKeyPresses.end())
            destinationKeyPresses.erase(it);
    }
    
    SystemInputSDL2::SystemInputSDL2() :  CurrentInputs(),
                                                        LastInputs(),
                                                        CurrentRealtimeInputs(),
                                                        LastRealtimeInputs(),
                                                        LastMousePosition(),
                                                        CurrentMousePosition(),
                                                        CurrentMouseScrollDelta(),
                                                        CurrentTextInput(),
                                                        CurrentCursorType(Enums::CursorType::NORMAL),
                                                        CurrentSDLCursor(nullptr),
                                                        CurrentCustomCursorName(""),
                                                        CustomCursors(),
                                                        RawEventHandlers(),
                                                        StartTime()
    {
    }

    SystemInputSDL2::~SystemInputSDL2()
    {
    }
    
    bool SystemInputSDL2::Initialize()
    {
        StartTime = std::chrono::high_resolution_clock::now();
        SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
        return true;
    }
    
    void SystemInputSDL2::UpdateInput(MainWindowInterface** mainWindows, int count)
    {
        LastInputs = CurrentInputs;
        LastRealtimeInputs = CurrentRealtimeInputs;
        LastMousePosition = CurrentMousePosition;
        
        CurrentRealtimeInputs.clear();
        CurrentMouseScrollDelta = glm::vec2();
        CurrentTextInput.clear();
        
        SDL_GetGlobalMouseState(&CurrentMousePosition.x, &CurrentMousePosition.y);
        
        CurrentMouseScrollDelta = glm::vec2();
        
        SDL_Event currentEvent;
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        
        bool hasCloseEvent = false;
        
        //Check if there's any SDL_WINDOWEVENT_CLOSE
        {
            SDL_PumpEvents();
                        
            std::vector<SDL_Event> currentEvents;
            currentEvents.resize(999);
            
            //Get all the window related events
            int numEvents = SDL_PeepEvents( currentEvents.data(), 
                                            999, 
                                            SDL_PEEKEVENT, 
                                            SDL_WINDOWEVENT, 
                                            SDL_WINDOWEVENT);
            
            if(numEvents < 0)
            {
                ssGUI_WARNING(  ssGUI_BACKEND_TAG, 
                                "SDL2 error while fetching window events: " << numEvents);
            }
            else
            {
                currentEvents.resize(numEvents);
                
                //Check if there's any window close event
                for(int i = 0; i < numEvents; i++)
                {
                    if(currentEvents[i].window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        hasCloseEvent = true;
                        break;
                    }
                }
            }
        }
        
        while(SDL_PollEvent(&currentEvent))
        {
            bool consumedByCustomHandler = false;
            
            //Check if any of the RawEventHandlers consumed this event
            for(int i = 0; i < RawEventHandlers.size(); ++i)
            {
                if(RawEventHandlers.at(i) == nullptr)
                    continue;
                
                if(RawEventHandlers.at(i)(count > 0 ?  mainWindows[0] : nullptr, &currentEvent))
                {
                    consumedByCustomHandler = true;
                    break;
                }
            }
            
            if(consumedByCustomHandler)
                continue;
            
            
            //Handle the event
            switch(currentEvent.type)
            {
                case SDL_QUIT:
                {
                    if(!hasCloseEvent)
                    {
                        for(int i = 0; i < count; ++i)
                            mainWindows[i]->Close();
                    }
                
                    break;
                }
                
                case SDL_WINDOWEVENT:
                {
                    for(int i = 0; i < count; ++i)
                    {
                        auto* currentSDL_Window = static_cast<MainWindowSDL2*>(mainWindows[i]);
                        
                        using RawHandle = MainWindowSDL2::SDL_RawHandle;
                        auto* sdlRawHandle = static_cast<RawHandle*>(currentSDL_Window->GetRawHandle());
                        
                        if(currentEvent.window.windowID == SDL_GetWindowID(sdlRawHandle->Window))
                        {
                            switch(currentEvent.window.event)
                            {
                                case SDL_WINDOWEVENT_FOCUS_GAINED:
                                    mainWindows[i]->SetFocus(true, true);
                                    break;
                                case SDL_WINDOWEVENT_FOCUS_LOST:
                                    mainWindows[i]->SetFocus(false, true);
                                    break;
                                case SDL_WINDOWEVENT_CLOSE:
                                    mainWindows[i]->Close();
                                    break;
                            }
                            
                            //Break out of the windows loop
                            break;
                        }
                    }
                    break;
                }
                case SDL_KEYDOWN:
                {
                    Enums::GenericInput input = 
                        SDL2InputConverter::ConvertKeys(currentEvent.key.keysym.sym);
                    
                    if(input != Enums::NO_INPUT)
                    {
                        RealtimeInputInfo realtimeInfo;
                        realtimeInfo.InputType = Enums::RealtimeInputType::KEY_BUTTON_INPUT;
                        realtimeInfo.Data.KeyButton.Input = input;
                        realtimeInfo.Data.KeyButton.IsDown = true;
                        CurrentRealtimeInputs.push_back(realtimeInfo);
                        FetchKeysPressed(input, CurrentInputs);
                    }

                    break;
                }
                case SDL_KEYUP:
                {
                    Enums::GenericInput input = 
                        SDL2InputConverter::ConvertKeys(currentEvent.key.keysym.sym);
                    
                    if(input != Enums::NO_INPUT)
                    {
                        RealtimeInputInfo realtimeInfo;
                        realtimeInfo.InputType = Enums::RealtimeInputType::KEY_BUTTON_INPUT;
                        realtimeInfo.Data.KeyButton.Input = input;
                        realtimeInfo.Data.KeyButton.IsDown = false;
                        CurrentRealtimeInputs.push_back(realtimeInfo);
                        FetchKeysReleased(input, CurrentInputs);
                    }

                    break;
                }
                case SDL_TEXTINPUT:
                {
                    size_t strLen = strnlen(currentEvent.text.text, 32);
                    std::string textInputString = std::string(currentEvent.text.text, strLen);
                    std::u32string textInputU32 = converter.from_bytes(textInputString);
                    
                    for(int i = 0; i < textInputU32.size(); ++i)
                    {
                        CurrentTextInput += textInputU32.at(i);
                        
                        RealtimeInputInfo realtimeInfo;
                        realtimeInfo.InputType = Enums::RealtimeInputType::TEXT_INPUT;
                        realtimeInfo.Data.CharacterEntered = textInputU32.at(i);
                        CurrentRealtimeInputs.push_back(realtimeInfo);
                    }
                    
                    break;
                }
                case SDL_MOUSEMOTION:
                {
                    RealtimeInputInfo realtimeInfo;
                    realtimeInfo.InputType = Enums::RealtimeInputType::MOUSE_POSITION;
                    realtimeInfo.Data.MousePosition = glm::ivec2(   currentEvent.motion.x, 
                                                                    currentEvent.motion.y);

                    CurrentRealtimeInputs.push_back(realtimeInfo);
                    break;
                }
                
                case SDL_MOUSEBUTTONDOWN:
                {
                    //(void)currentEvent.button.button;
                    Enums::GenericInput input = 
                        SDL2InputConverter::ConvertMouseButton(currentEvent.button.button);
                    
                    if(input != Enums::NO_INPUT)
                    {
                        RealtimeInputInfo realtimeInfo;
                        realtimeInfo.InputType = Enums::RealtimeInputType::KEY_BUTTON_INPUT;
                        realtimeInfo.Data.KeyButton.Input = input;
                        realtimeInfo.Data.KeyButton.IsDown = true;
                        CurrentRealtimeInputs.push_back(realtimeInfo);
                        FetchKeysPressed(input, CurrentInputs);
                    }
                    
                    break;
                }
                
                case SDL_MOUSEBUTTONUP:
                {
                    //(void)currentEvent.button.button;
                    Enums::GenericInput input = 
                        SDL2InputConverter::ConvertMouseButton(currentEvent.button.button);
                    
                    if(input != Enums::NO_INPUT)
                    {
                        RealtimeInputInfo realtimeInfo;
                        realtimeInfo.InputType = Enums::RealtimeInputType::KEY_BUTTON_INPUT;
                        realtimeInfo.Data.KeyButton.Input = input;
                        realtimeInfo.Data.KeyButton.IsDown = false;
                        CurrentRealtimeInputs.push_back(realtimeInfo);
                        FetchKeysReleased(input, CurrentInputs);
                    }
                    
                    break;
                }
                
                case SDL_MOUSEWHEEL:
                    CurrentMouseScrollDelta = glm::vec2(currentEvent.wheel.x > 0 ? 1 : -1, 
                                                        currentEvent.wheel.y > 0 ? 1 : -1);
                    
                    CurrentMouseScrollDelta *= 
                        (currentEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1);
                    
                    break;
                
                case SDL_SYSWMEVENT:
                    //NOTE: Don't need to do anything here
                    break;
                
                default:
                    //Log events that are not captured
                    ssGUI_WARNING(ssGUI_BACKEND_TAG,    "SDL2 event not captured: " << 
                                                        currentEvent.type);
                    break;
            }
        }
    }

    const std::vector<Enums::GenericInput>& SystemInputSDL2::GetLastInputs() const
    {
        return LastInputs;
    }
    
    const std::vector<Enums::GenericInput>& SystemInputSDL2::GetCurrentInputs() const
    {
        return CurrentInputs;
    }

    bool SystemInputSDL2::IsInputExistLastFrame(Enums::GenericInput input) const
    {
        return !LastInputs.empty();
    }

    bool SystemInputSDL2::IsInputExistCurrentFrame(Enums::GenericInput input) const
    {
        return CurrentInputs.empty();
    }

    glm::ivec2 
    SystemInputSDL2::GetLastMousePosition(MainWindowInterface* mainWindow) const
    {
        return LastMousePosition;
    }
    
    glm::ivec2 
    SystemInputSDL2::GetCurrentMousePosition(MainWindowInterface* mainWindow) const
    {
        return CurrentMousePosition;
    }
    
    void SystemInputSDL2::SetMousePosition(  glm::ivec2 position, 
                                                    MainWindowInterface* mainWindow)
    {
        SDL_WarpMouseInWindow((SDL_Window*)mainWindow->GetRawHandle(), position.x, position.y);
    }

    glm::vec2 SystemInputSDL2::GetCurrentMouseScrollDelta() const
    {
        return CurrentMouseScrollDelta;
    }

    const std::vector<RealtimeInputInfo>& SystemInputSDL2::GetLastRealtimeInputs() const
    {
        return LastRealtimeInputs;
    }

    const std::vector<RealtimeInputInfo>& SystemInputSDL2::GetCurrentRealtimeInputs() const
    {
        return CurrentRealtimeInputs;
    }

    void SystemInputSDL2::StartTextInput(glm::ivec2 inputPos, glm::ivec2 inputSize)
    {
        SDL_Rect inputRect;
        inputRect.x = inputPos.x;
        inputRect.y = inputPos.y;
        inputRect.w = inputSize.x;
        inputRect.h = inputSize.y;
        
        SDL_SetTextInputRect(&inputRect);
        
        SDL_StartTextInput();
    }
    
    void SystemInputSDL2::FinishTextInput()
    {
        SDL_StopTextInput();
    }

    void SystemInputSDL2::GetTextInput(std::u32string& outText) const
    {
        outText = CurrentTextInput;
    }
    
    void SystemInputSDL2::GetTextInput(std::string& outText) const
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        outText = converter.to_bytes(CurrentTextInput);
    }
    
    void SystemInputSDL2::SetCursorType(Enums::CursorType cursorType)
    {
        CurrentCursorType = cursorType;
    }

    Enums::CursorType SystemInputSDL2::GetCursorType() const
    {
        return CurrentCursorType;
    }

    void SystemInputSDL2::CreateCustomCursor(ImageInterface* customCursor, 
                                                    std::string cursorName, 
                                                    glm::ivec2 cursorSize, 
                                                    glm::ivec2 hotspot)
    {
        //Validation
        if(hotspot.x > cursorSize.x || hotspot.y > cursorSize.y)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG,    "Invalid hotspot position: " << 
                                                hotspot.x << ", " << hotspot.y);
            return;
        }
        
        ssGUI::ImageFormat customCursorFormat;
        void* customCursorPtr = customCursor->GetPixelPtr(customCursorFormat);
        if(customCursorPtr == nullptr)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Invalid custom cursor image");
            return;
        }

        //If the cursor already exists, we need to clean up the cursor first
        if(CustomCursors.find(cursorName) != CustomCursors.end())
            CustomCursors.erase(cursorName);

        CursorData& cursorData = CustomCursors[cursorName] = 
        {
            std::shared_ptr<ImageInterface>( Factory::CreateImageInterface(), 
                                                    [](ImageInterface* ptr)
                                                    { 
                                                        delete ptr; 
                                                    }),
            hotspot
        };
        
        std::unique_ptr<uint8_t[]> resizedCursorPtr = nullptr;
        
        //We need to convert it to RGBA first if needed, then we resize it to the cursor size
        if(customCursor->GetSize() != cursorSize)
        {
            auto convertedPtr = std::unique_ptr<uint8_t[]>(new uint8_t[ customCursor->GetSize().x * 
                                                                        customCursor->GetSize().y * 
                                                                        4]);
            
            bool result = ssGUI::ImageUtil::FormatToRGBA32(convertedPtr.get(), 
                                                            customCursorPtr, 
                                                            customCursorFormat, 
                                                            customCursor->GetSize());
         
            if(!result)
            {
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "Conversion failed");
                return;
            }
            
            resizedCursorPtr = std::unique_ptr<uint8_t[]>(new uint8_t[cursorSize.x * cursorSize.y * 4]);
            ssGUI::ImageUtil::ResizeRGBA(   convertedPtr.get(), 
                                            customCursor->GetSize().x, 
                                            customCursor->GetSize().y, 
                                            resizedCursorPtr.get(), 
                                            cursorSize.x, 
                                            cursorSize.y);
            
        }
        
        //Convert it to ABGR (Little Endian)
        auto convertedPtr = std::unique_ptr<uint8_t[]>(new uint8_t[cursorSize.x * cursorSize.y * 4]);
        const void* rawCursorPtr =  resizedCursorPtr ? 
                                    resizedCursorPtr.get() : 
                                    customCursor->GetPixelPtr(customCursorFormat);
        
        ImageFormat rawCursorFormat = resizedCursorPtr ? customCursorFormat : ImageFormat();
        bool result = ssGUI::ImageUtil::FormatToABGR32( convertedPtr.get(), 
                                                        rawCursorPtr,
                                                        rawCursorFormat,
                                                        cursorSize);
        
        if(!result)
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Conversion failed");
            return;
        }
        
        ssGUI::ImageFormat convertedFormat;
        convertedFormat.IndexA = 0;
        convertedFormat.IndexB = 1;    
        convertedFormat.IndexG = 2;
        convertedFormat.IndexR = 3;
        cursorData.CursorImage->LoadRawFromMemory(convertedPtr.get(), convertedFormat, cursorSize);
        cursorData.Hotspot = hotspot;
    }
    
    void SystemInputSDL2::SetCurrentCustomCursor(std::string cursorName)
    {
        if(cursorName.empty())
        {
            CurrentCustomCursorName = "";
            return;
        }
        
        if(CustomCursors.find(cursorName) == CustomCursors.end())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Cursor not found: " << cursorName);
            return;
        }
        
        CurrentCustomCursorName = cursorName;
    }

    void SystemInputSDL2::GetCurrentCustomCursor(ImageInterface& customCursor, 
                                                        glm::ivec2& hotspot) const
    {
        if(CurrentCustomCursorName.empty())
            return;
        
        GetCustomCursor(customCursor, CurrentCustomCursorName, hotspot);
    }

    std::string SystemInputSDL2::GetCurrentCustomCursorName() const
    {
        return CurrentCustomCursorName;
    }
    
    void SystemInputSDL2::GetCustomCursor(   ImageInterface& customCursor, 
                                                    std::string cursorName, 
                                                    glm::ivec2& hotspot) const
    {
        if(CustomCursors.find(cursorName) == CustomCursors.end())
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Cursor not found: " << cursorName);
            return;
        }
        
        ssGUI::ImageFormat imgFormat;

        const CursorData& data = CustomCursors.at(cursorName);

        if(data.CursorImage->GetPixelPtr(imgFormat) == nullptr)
            return;

        if(!customCursor.LoadRawFromMemory( data.CursorImage->GetPixelPtr(imgFormat),
                                            imgFormat,
                                            data.CursorImage->GetSize()))
        {
            ssGUI_WARNING(ssGUI_BACKEND_TAG, "Failed to load custom cursor image");
            return;
        }

        hotspot = data.Hotspot;
    }

    bool SystemInputSDL2::HasCustomCursor(std::string cursorName) const
    {
        return CustomCursors.find(cursorName) != CustomCursors.end();
    }

    void SystemInputSDL2::UpdateCursor()
    {
        static_assert((uint8_t)Enums::CursorType::COUNT == 16, "Not Match");
        
        if(CurrentSDLCursor != nullptr)
            SDL_FreeCursor(CurrentSDLCursor);
        
        if(CurrentCustomCursorSurface != nullptr)
            SDL_FreeSurface(CurrentCustomCursorSurface);
        
        switch(CurrentCursorType)
        {
            case Enums::CursorType::NONE:
            {
                int result = SDL_ShowCursor(SDL_FALSE);
                if(result < 0)
                    ssGUI_WARNING(ssGUI_BACKEND_TAG, "SDL2 error while hiding cursor: " << result);
                
                CurrentSDLCursor = nullptr;
                break;
            }
            case Enums::CursorType::NORMAL:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
                break;
            case Enums::CursorType::TEXT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
                break;
            case Enums::CursorType::HAND:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
                break;
            case Enums::CursorType::RESIZE_LEFT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
                break;
            case Enums::CursorType::RESIZE_RIGHT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
                break;
            case Enums::CursorType::RESIZE_UP:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
                break;
            case Enums::CursorType::RESIZE_DOWN:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
                break;
            case Enums::CursorType::RESIZE_TOP_LEFT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                break;
            case Enums::CursorType::RESIZE_TOP_RIGHT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
                break;
            case Enums::CursorType::RESIZE_BOTTOM_RIGHT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
                break;
            case Enums::CursorType::RESIZE_BOTTOM_LEFT:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
                break;
            case Enums::CursorType::MOVE:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
                break;
            case Enums::CursorType::HELP:
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "SDL2 does not have a help cursor");
                break;
            case Enums::CursorType::NOT_ALLOWED:
                CurrentSDLCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
                break;
            case Enums::CursorType::CUSTOM:
            {
                if(CurrentCustomCursorName.empty())
                    break;
                
                ImageInterface* cursorImg = CustomCursors.at(CurrentCustomCursorName)
                                                                .CursorImage
                                                                .get();
                
                glm::ivec2 hotspot = CustomCursors.at(CurrentCustomCursorName).Hotspot;
                
                if(!cursorImg->IsValid())
                    break;
                
                ImageFormat cursorImgFormat;
                void* cursorPixels = cursorImg->GetPixelPtr(cursorImgFormat);
                SDL_Surface* cursorSurface = 
                    SDL_CreateRGBSurfaceWithFormatFrom( cursorPixels, 
                                                        cursorImg->GetSize().x, 
                                                        cursorImg->GetSize().y, 
                                                        32, 
                                                        cursorImg->GetSize().x * 32 + 
                                                            cursorImgFormat.RowPaddingInBytes, 
                                                        SDL_PIXELFORMAT_RGBA32);
                
                if(cursorSurface == NULL)
                {
                    ssGUI_WARNING(ssGUI_BACKEND_TAG, "SDL error while creating cursor surface");
                    break;
                }
                
                CurrentCustomCursorSurface = cursorSurface;
                CurrentSDLCursor = SDL_CreateColorCursor(   cursorSurface, 
                                                            hotspot.x,
                                                            hotspot.y);
                
                if(CurrentSDLCursor == NULL)
                    ssGUI_WARNING(ssGUI_BACKEND_TAG, "SDL error while creating cursor");
                
                break;
            }
        }
        
        if(CurrentSDLCursor != nullptr)
        {
            SDL_SetCursor(CurrentSDLCursor);
            int result = SDL_ShowCursor(SDL_TRUE);
            
            if(result < 0)
                ssGUI_WARNING(ssGUI_BACKEND_TAG, "SDL2 error while showing cursor: " << result);
        }
    }
    
    int SystemInputSDL2::AddRawEventHandler(std::function<bool(  MainWindowInterface*, 
                                                                        void*)> handler)
    {
        RawEventHandlers.push_back(handler);
        return RawEventHandlers.size();
    }
    
    void SystemInputSDL2::RemoveRawEventHandler(int id)
    {
        RawEventHandlers.at(id) = nullptr;
    }
    
    void SystemInputSDL2::ClearRawEventHandler()
    {
        for(int i = 0; i < RawEventHandlers.size(); i++)
            RawEventHandlers.at(i) = nullptr;
    }

    bool SystemInputSDL2::ClearClipboard()
    {
        return clip::clear();
    }

    bool SystemInputSDL2::ClipbaordHasText() const
    {
        return clip::has(clip::text_format());
    }
    
    bool SystemInputSDL2::ClipbaordHasImage() const
    {
        return clip::has(clip::image_format());
    }

    bool SystemInputSDL2::SetClipboardImage(const ImageInterface& imgData)
    {
        if(!imgData.IsValid())
            return false;

        ssGUI::ImageFormat format;
        void* oriImgPtr = imgData.GetPixelPtr(format);

        clip::image_spec spec;
        spec.width = imgData.GetSize().x;
        spec.height = imgData.GetSize().y;
        spec.bits_per_pixel = 32;
        spec.bytes_per_row = spec.width * 4;

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

        result = ssGUI::ImageUtil::FormatToRGBA32(imgPtr, oriImgPtr, format, imgData.GetSize());

        if(!result)
            return false;

        clip::image clipImg = clip::image(static_cast<const void*>(imgPtr), spec);
        result = clip::set_image(clipImg);
        delete[] imgPtr;
        return result;
    }
    
    bool SystemInputSDL2::SetClipboardText(const std::u32string& str)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        return clip::set_text(codec.to_bytes(str));
    }
    
    bool SystemInputSDL2::SetClipboardText(const std::string& str)
    {
        return clip::set_text(str);
    }

    bool SystemInputSDL2::GetClipboardImage(ImageInterface& imgData) const
    {
        clip::image img;

        if(!clip::get_image(img) || img.spec().bits_per_pixel != 32)
            return false;

        ssGUI::ImageFormat format;

        return imgData.LoadRawFromMemory(   static_cast<void*>(img.data()), 
                                            format, 
                                            glm::ivec2(img.spec().width, img.spec().height));
    }

    bool SystemInputSDL2::GetClipboardText(std::u32string& str) const
    {
        std::string temp;
        if(!clip::get_text(temp))
            return false;

        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> codec;
        str = codec.from_bytes(temp);
        return true;
    }
    
    bool SystemInputSDL2::GetClipboardText(std::string& str) const
    {
        std::string temp;
        if(!clip::get_text(temp))
            return false;

        str = temp;
        return true;
    }

    uint64_t SystemInputSDL2::GetElapsedTime() const
    {
        using namespace std::chrono;
        using sysClock = high_resolution_clock;
        return duration_cast<milliseconds>(sysClock::duration(sysClock::now() - StartTime)).count();
    }
}

}