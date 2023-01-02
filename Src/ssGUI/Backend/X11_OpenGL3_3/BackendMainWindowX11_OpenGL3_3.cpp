#include "ssGUI/Backend/X11_OpenGL3_3/BackendMainWindowX11_OpenGL3_3.hpp"
#include <X11/Xatom.h>
#include <cstring>
#include <X11/extensions/Xrandr.h>

#include "ssGUI/Backend/BackendManager.hpp"
#include "ssLogger/ssLog.hpp"

namespace ssGUI
{

namespace Backend
{
    BackendMainWindowX11_OpenGL3_3::BackendMainWindowX11_OpenGL3_3(BackendMainWindowX11_OpenGL3_3 const& other)
    {
        //TODO: Implement this
        ssLOG_LINE("Not implemented");
        ssLOG_EXIT_PROGRAM();
    }

    union XClientEventData 
    {
		char b[20];
		short s[10];
		long l[5];
    };

    //https://stackoverflow.com/a/16235920
    bool SendXClientEvent(Display* display, Window root, Window mywin, const char* eventName, XClientEventData& data, int format)
    {
        //Atom wmStateAbove = XInternAtom( display, "_NET_WM_STATE_FULLSCREEN", 1 );
        ////Atom wmStateAbove = XInternAtom( display, "_NET_WM_STATE_ABOVE", 1 );
        //if( wmStateAbove != None ) {
        //    printf( "_NET_WM_STATE_ABOVE has atom of %ld\n", (long)wmStateAbove );
        //} else {
        //    printf( "ERROR: cannot find atom for _NET_WM_STATE_ABOVE !\n" );
        //    return False;
        //}
        
        Atom targetAtom = XInternAtom( display, eventName, 1 );
        if(!targetAtom) 
            return false;

        XClientMessageEvent xclient;
        memset( &xclient, 0, sizeof (xclient) );
        //
        //window  = the respective client window
        //message_type = _NET_WM_STATE
        //format = 32
        //data.l[0] = the action, as listed below
        //data.l[1] = first property to alter
        //data.l[2] = second property to alter
        //data.l[3] = source indication (0-unk,1-normal app,2-pager)
        //other data.l[] elements = 0
        //
        xclient.type = ClientMessage;
        xclient.window = mywin;              // GDK_WINDOW_XID(window);
        xclient.message_type = targetAtom; //gdk_x11_get_xatom_by_name_for_display( display, "_NET_WM_STATE" );
        xclient.format = format;
        memcpy(&xclient.data, &data, sizeof(XClientMessageEvent::data));
        //xclient.data.l[0] = _NET_WM_STATE_ADD; // add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
        //xclient.data.l[1] = wmStateAbove;      //gdk_x11_atom_to_xatom_for_display (display, state1);
        //xclient.data.l[2] = 0;                 //gdk_x11_atom_to_xatom_for_display (display, state2);
        //xclient.data.l[3] = 0;
        //xclient.data.l[4] = 0;
        //gdk_wmspec_change_state( FALSE, window,
        //  gdk_atom_intern_static_string ("_NET_WM_STATE_BELOW"),
        //  GDK_NONE );

        int result = XSendEvent(display,
                                root,
                                False,
                                SubstructureRedirectMask | SubstructureNotifyMask,
                                (XEvent *)&xclient);

        XFlush(display);

        return result != 0;
    }

    int FindGLXAttributeIndex(int* attributeList, int count, int attribute)
    {
        for(int i = 0; i < count; i += 2)
        {
            if(attributeList[i] == attribute)
                return i;
        }
        
        return -1;
    }

    bool BackendMainWindowX11_OpenGL3_3::ssGUI_CreateWindow()
    {        
        WindowDisplay = XOpenDisplay(NULL);
        if (WindowDisplay == NULL) 
        {
            ssLOG_LINE("cannot connect to X server");
            return false;
        }

        int screen = DefaultScreen(WindowDisplay);

        int loadGLX = gladLoadGLX(WindowDisplay, screen);
        if (!loadGLX) 
        {
            ssLOG_LINE("Unable to load GLX");
            return false;
        }
        
        Window root = RootWindow(WindowDisplay, screen);
        
        // Choose a framebuffer config with multisample support
        int glxAttrib[] = 
        {
            GLX_X_RENDERABLE    , True,
            GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
            GLX_RENDER_TYPE     , GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
            GLX_DOUBLEBUFFER   , True,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_ALPHA_SIZE, 8,
            //Can't use this
                GLX_ACCUM_RED_SIZE, 8,
                GLX_ACCUM_GREEN_SIZE, 8,
                GLX_ACCUM_BLUE_SIZE, 8,
                GLX_ACCUM_ALPHA_SIZE, 8,
            GLX_DEPTH_SIZE      , 24,
            GLX_STENCIL_SIZE    , 8,
            GLX_AUX_BUFFERS,    32,   //This is probably not needed
            GLX_SAMPLE_BUFFERS_ARB, MsaaLevel > 0 ? 1 : 0,
            GLX_SAMPLES_ARB, MsaaLevel,
            None
        };
        
        int numConfigs;
        GLXFBConfig* config = nullptr;
        
        int auxIndex = FindGLXAttributeIndex(glxAttrib, sizeof(glxAttrib) / sizeof(int), GLX_AUX_BUFFERS);
        int accumIndex = FindGLXAttributeIndex(glxAttrib, sizeof(glxAttrib) / sizeof(int), GLX_ACCUM_RED_SIZE);
        int msaaIndex = FindGLXAttributeIndex(glxAttrib, sizeof(glxAttrib) / sizeof(int), GLX_SAMPLE_BUFFERS_ARB);
        int stencilIndex = FindGLXAttributeIndex(glxAttrib, sizeof(glxAttrib) / sizeof(int), GLX_STENCIL_SIZE);
        int depthIndex = FindGLXAttributeIndex(glxAttrib, sizeof(glxAttrib) / sizeof(int), GLX_DEPTH_SIZE);
        
        //Decreasing each bitdepth and see what works
        fallback:
        
        config = glXChooseFBConfig(WindowDisplay, screen, glxAttrib, &numConfigs);
        
        if (!config)
        {
            //Seems like no one uses Auxiliary buffer, this could be a problem
            if(glxAttrib[auxIndex + 1] == 32)
            {
                glxAttrib[auxIndex + 1] = 16;
                ssLOG_LINE("Aux Buffer: "<<glxAttrib[auxIndex + 1]);
                goto fallback;
            }
            else if(glxAttrib[auxIndex + 1] == 16)
            {
                glxAttrib[auxIndex + 1] = 8;
                ssLOG_LINE("Aux Buffer: "<<glxAttrib[auxIndex + 1]);
                goto fallback;
            }
            else if(glxAttrib[auxIndex + 1] == 8)
            {
                glxAttrib[auxIndex + 1] = 0;
                ssLOG_LINE("Aux Buffer: "<<glxAttrib[auxIndex + 1]);
                goto fallback;
            }

            //Maybe try turning off accumulative buffer?
            else if(glxAttrib[accumIndex + 1] == 8)
            {
                glxAttrib[accumIndex + 1] = 0;
                glxAttrib[accumIndex + 3] = 0;
                glxAttrib[accumIndex + 5] = 0;
                glxAttrib[accumIndex + 7] = 0;
                ssLOG_LINE("Accum Buffer: "<<0);
                goto fallback;
            }
            
            //Maybe the MSAA level is not correct?
            else if(MsaaLevel > 0)
            {
                ssLOG_LINE("Trying to disable MSAA and retry...");
                glxAttrib[msaaIndex + 1] = 0;
                glxAttrib[msaaIndex + 3] = 0;
                goto fallback;
            }

            //Normally stencil bit can be a problem when above 8 bits
            else if(glxAttrib[stencilIndex + 1] == 32)
            {
                glxAttrib[stencilIndex + 1] = 16;
                ssLOG_LINE("Stencil bit: "<<glxAttrib[stencilIndex + 1]);
                goto fallback;
            }
            else if(glxAttrib[stencilIndex + 1] == 16)
            {
                glxAttrib[stencilIndex + 1] = 8;
                ssLOG_LINE("Stencil bit: "<<glxAttrib[stencilIndex + 1]);
                goto fallback;
            }

            //Very unlikely for depth buffer to have problem but worth a try
            else if(glxAttrib[depthIndex + 1] == 32)
            {
                glxAttrib[depthIndex + 1] = 16;
                ssLOG_LINE("Depth bit: "<<glxAttrib[depthIndex + 1]);
                goto fallback;
            }
            else if(glxAttrib[depthIndex + 1] == 16)
            {
                glxAttrib[depthIndex + 1] = 8;
                ssLOG_LINE("Depth bit: "<<glxAttrib[depthIndex + 1]);
                goto fallback;
            }
            
            ssLOG_LINE("Failed to resolve GLX Attributes, exited");
            ssLOG_EXIT_PROGRAM();
        }
        
        XVisualInfo *visual_info = glXGetVisualFromFBConfig(WindowDisplay, config[0]);
        XFree(config);

        WindowColormap = XCreateColormap(WindowDisplay, root, visual_info->visual, AllocNone);

        XSetWindowAttributes attributes;
        attributes.event_mask = ExposureMask | 
                                KeyPressMask | 
                                KeyReleaseMask |
                                ButtonPressMask |
                                ButtonReleaseMask |
                                EnterWindowMask |
                                LeaveWindowMask |
                                PointerMotionMask;
        attributes.colormap = WindowColormap;

        WindowId = XCreateWindow(WindowDisplay, root, 0, 0, 500, 500, 0,
                        visual_info->depth, InputOutput, visual_info->visual,
                        CWColormap | CWEventMask, &attributes);

        XMapWindow(WindowDisplay, WindowId);
        
        //Create user event for closing window
        WindowCloseEventId = XInternAtom(WindowDisplay, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(WindowDisplay, WindowId, &WindowCloseEventId, 1);
        
        if (!WindowId) 
            return false;

        WindowContext = glXCreateContext(WindowDisplay, visual_info, NULL, GL_TRUE);
        glXMakeCurrent(WindowDisplay, WindowId, WindowContext);

        int loadGL = gladLoadGL();
        if (!loadGL) 
        {
            ssLOG_LINE("Unable to load GL");
            return false;
        }

        XWindowAttributes gwa;
        XGetWindowAttributes(WindowDisplay, WindowId, &gwa);
        glViewport(0, 0, gwa.width, gwa.height);

        SetFocus(true, false);         
         
        return true; 
    }

    void BackendMainWindowX11_OpenGL3_3::ssGUI_DestroyWindow()
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            if(!OriginalResolutionSet)
            {
                ssLOG_LINE("OriginalResolutionSet not set, this is probably a bug, please report.");
                return;
            }
            
            ReturnToOriginalResolution();
        }

        glXMakeCurrent(WindowDisplay, 0, 0);
        glXDestroyContext(WindowDisplay, WindowContext);

        XDestroyWindow(WindowDisplay, WindowId);
        XFreeColormap(WindowDisplay, WindowColormap);
        XCloseDisplay(WindowDisplay);
    }
    
    void BackendMainWindowX11_OpenGL3_3::UpdateWindowCapability()
    {
        if(GetWindowMode() != ssGUI::Enums::WindowMode::NORMAL)
            return;
    
        //Set the function
        MwmHintsData hintData;
        hintData.flags = MWM_HINTS_FUNCTIONS;
        hintData.functions = (IsResizable() ? MWM_FUNC_RESIZE : 0) | MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE | (HasCloseButton() ? MWM_FUNC_CLOSE : 0);
        
        XChangeProperty(WindowDisplay, 
                        WindowId, 
                        XInternAtom(WindowDisplay, "_MOTIF_WM_HINTS", False), 
                        XInternAtom(WindowDisplay, "_MOTIF_WM_HINTS", False), 
                        32, 
                        PropModeReplace, 
                        (unsigned char*)&hintData, 
                        5);
    }

    void BackendMainWindowX11_OpenGL3_3::UpdateWindowDecor()
    {
        //If we are not in normal mode, then just set window mode
        if(GetWindowMode() != ssGUI::Enums::WindowMode::NORMAL)
            SetWindowMode(GetWindowMode());
        else
        {
            MwmHintsData hintData;
            hintData.flags = MWM_HINTS::MWM_HINTS_DECORATIONS;
            hintData.decorations = HasTitlebar() ? MWM_DECOR::MWM_DECOR_ALL : 0;
        
            XChangeProperty(WindowDisplay, 
                            WindowId, 
                            XInternAtom(WindowDisplay, "_MOTIF_WM_HINTS", False), 
                            XInternAtom(WindowDisplay, "_MOTIF_WM_HINTS", False), 
                            32, 
                            PropModeReplace, 
                            (unsigned char*)&hintData, 
                            5);
        }
    }

    //https://stackoverflow.com/a/36198958
    void BackendMainWindowX11_OpenGL3_3::GetWindowDecor(int& topExtent, int& rightExtent, int& botExtent, int& leftExtent) const
    {
        Atom actualType;
        int actualFormat;
        unsigned long numberOfItems;
        unsigned long bytesAfter;
        unsigned char* data;
        XEvent e;
        
        while(XGetWindowProperty(   WindowDisplay, 
                                    WindowId, 
                                    XInternAtom(WindowDisplay, "_NET_FRAME_EXTENTS", False), 
                                    0, 
                                    4, 
                                    False, 
                                    AnyPropertyType, 
                                    &actualType, 
                                    &actualFormat, 
                                    &numberOfItems, 
                                    &bytesAfter, 
                                    &data) != Success || numberOfItems != 4 || bytesAfter != 0)
        {
            ssLOG_LINE("Attempting to get _NET_FRAME_EXTENTS");
            XNextEvent(WindowDisplay, &e);
        }
        
        long* extents;
        extents = (long*) data;

        leftExtent = extents[0];
        rightExtent = extents[1];
        topExtent = extents[2];
        botExtent = extents[3];
        
        return;
    }

    bool BackendMainWindowX11_OpenGL3_3::SetScreenResolution(glm::ivec2 targetResolution)
    {
        int nsize ;
        int screen = DefaultScreen(WindowDisplay);
        XRRScreenSize * sizes = XRRSizes(WindowDisplay, screen, &nsize);   
        int sizeindex = -1;
        
        if(targetResolution == glm::ivec2())
        {
            targetResolution.x = DisplayWidth(WindowDisplay, screen);
            targetResolution.y = DisplayHeight(WindowDisplay, screen);
        }
        
        //ssLOG_LINE("targetResolution: "<<targetResolution.x<<", "<<targetResolution.y);
        
        //Debug
        //for(int i = 0; i < nsize; i++)
        //{
        //    ssLOG_LINE("size: "<<sizes[i].width<<" x "<<sizes[i].height);
        //}
        
        //Check if there's an exact match first
        //The list goes from largest resolution "area" to smallest
        for(int i = 0; i < nsize; i++)
        {
            if (sizes[sizeindex].width == targetResolution.x && sizes[sizeindex].height == targetResolution.y)
            {
                sizeindex = i;
                break;
            }
        }
        
        if(sizeindex < 0)
        {
            for(int i = 0; i < nsize; i++)
            {
                if (sizes[sizeindex].width <= targetResolution.x && sizes[sizeindex].height <= targetResolution.y)
                {
                    sizeindex = i;
                    break;
                }
            }
        }
        
        if (sizeindex >= nsize || sizeindex < 0)
        {
            ssLOG_LINE("Failed to set fullscreen resolution to "<<targetResolution.x<<" x "<<targetResolution.y);
            ssLOG_LINE("Using current resolution instead.");
            
            return false;
        }
        else
        {
            Window root = RootWindow(WindowDisplay, screen);

            XRRScreenConfiguration* conf = XRRGetScreenInfo(WindowDisplay, root);
            Rotation originalRotation;
            SizeID lastResId = XRRConfigCurrentConfiguration(conf, &originalRotation);
            
            if(!OriginalResolutionSet)
            {
                OriginalResolutionSet = true;
                OriginalResolutionId = lastResId;
            }

            if(!conf)
            {
                ssLOG_LINE("Failed to get screen info");
                return false;
            }

            Status status = XRRSetScreenConfig( WindowDisplay, 
                                                conf,
                                                root,
                                                (SizeID) sizeindex,
                                                originalRotation,
                                                CurrentTime);
            
            
            if(status != Success)
            {
                ssLOG_LINE("Failed to set resolution: "<<sizeindex);
                return false;
            }

            XRRFreeScreenConfigInfo(conf);
        }
        
        return true;
    }    
    
    void BackendMainWindowX11_OpenGL3_3::ReturnToOriginalResolution()
    {
        int screen = DefaultScreen(WindowDisplay);
        Window root = RootWindow(WindowDisplay, screen);

        XRRScreenConfiguration* conf = XRRGetScreenInfo(WindowDisplay, root);
        Rotation originalRotation;
        SizeID lastResId = XRRConfigCurrentConfiguration(conf, &originalRotation);
        
        if(!OriginalResolutionSet)
        {
            OriginalResolutionSet = true;
            OriginalResolutionId = lastResId;
        }

        if(!conf)
        {
            ssLOG_LINE("Failed to get screen info");
            return;
        }

        Status status = XRRSetScreenConfig( WindowDisplay, 
                                            conf,
                                            root,
                                            OriginalResolutionId,
                                            originalRotation,
                                            CurrentTime);
        
        
        if(status != Success)
        {
            ssLOG_LINE("Failed to set resolution: "<<OriginalResolutionId);
            return;
        }

        XRRFreeScreenConfigInfo(conf);
    }
    
    
    //TODO: Maybe remove this
    //bool BackendMainWindowX11_OpenGL3_3::GetAllowedWindowActions(Atom** atoms, int& count)
    //{
    //    // Get the _NET_WM_ALLOWED_ACTIONS property
    //    Atom actualType;
    //    int actualFormat;
    //    unsigned long numberOfItems;
    //    unsigned long bytesAfter;
    //    Atom* data;
    //    int result = XGetWindowProperty(WindowDisplay, 
    //                                    WindowId, 
    //                                    XInternAtom(WindowDisplay, "_NET_WM_ALLOWED_ACTIONS", False), 
    //                                    0, 
    //                                    65536, 
    //                                    False, 
    //                                    XA_ATOM, 
    //                                    &actualType, 
    //                                    &actualFormat, 
    //                                    &numberOfItems, 
    //                                    &bytesAfter, 
    //                                    (unsigned char**)&data);

    //    if (result != Success) 
    //    {
    //        XFree(data);
    //        return false;
    //    }

    //    *atoms = data;
    //    count = numberOfItems;
        
    //    //printf("_NET_WM_ALLOWED_ACTIONS:\n");
    //    //for (unsigned long i = 0; i < numberOfItems; i++) 
    //    //{
    //    //    char* atomName = XGetAtomName(WindowDisplay, data[i]);
    //    //    printf("  %s\n", atomName);
    //    //    XFree(atomName);
    //    //}
        
    //    return true;
    //}

    //bool BackendMainWindowX11_OpenGL3_3::AddAllowedWindowActionIfNotExists(const char* actionName)
    //{
    //    Atom* atoms = nullptr;
    //    int count;
    
    //    bool result = GetAllowedWindowActions(&atoms, count);
        
    //    if(!result)
    //        return false;
        
    //    bool exists = false;
        
    //    for (unsigned long i = 0; i < count; i++) 
    //    {
    //        char* atomName = XGetAtomName(WindowDisplay, atoms[i]);
    //        //printf("  %s\n", atomName);
            
    //        //If they are both equals
    //        if(std::strcmp(actionName, atomName) == 0)
    //        {
    //            exists = true;
    //            XFree(atomName);
    //            break;
    //        }

    //        XFree(atomName);            
    //    }
        
    //    if(!exists)
    //    {
    //        XChangeProperty(WindowDisplay, 
    //                        WindowId, 
    //                        XInternAtom(WindowDisplay, "_NET_WM_ALLOWED_ACTIONS", False), 
    //                        XA_ATOM, 
    //                        8, 
    //                        PropModeAppend, 
    //                        (unsigned char*)XInternAtom(WindowDisplay, actionName, False), 
    //                        1);        
    //    }

    //    XFree(atoms);
        
    //    return true;
    //}
    
    //bool BackendMainWindowX11_OpenGL3_3::RemoveAllowedWindowActionIfExists(const char* actionName)
    //{
    //    Atom* atoms = nullptr;
    //    int count;
    
    //    bool result = GetAllowedWindowActions(&atoms, count);
        
    //    if(!result)
    //        return false;
        
    //    int existIndex = -1;
        
    //    for (unsigned long i = 0; i < count; i++) 
    //    {
    //        char* atomName = XGetAtomName(WindowDisplay, atoms[i]);
    //        //printf("  %s\n", atomName);
            
    //        //If they are both equals
    //        if(std::strcmp(actionName, atomName) == 0)
    //        {
    //            existIndex = i;
    //            XFree(atomName);
    //            break;
    //        }

    //        XFree(atomName);            
    //    }
        
    //    if(existIndex >= 0)
    //    {
    //        Atom* newAtoms = new Atom[count - 1];
        
    //        int offset = 0;
    //        for(unsigned long i = 0; i < count; i++)
    //        {
    //            if(i == existIndex)
    //            {
    //                offset += 1;
    //                continue;
    //            }
                
    //            newAtoms[i - offset] = atoms[i];
    //        }
        
    //        XChangeProperty(WindowDisplay, 
    //                        WindowId, 
    //                        XInternAtom(WindowDisplay, "_NET_WM_ALLOWED_ACTIONS", False), 
    //                        XA_ATOM, 
    //                        32, 
    //                        PropModeReplace, 
    //                        (unsigned char*)newAtoms, 
    //                        count - 1);

    //        delete[] newAtoms;
    //    }

    //    XFree(atoms);
        
    //    return true;
    //}
    
    BackendMainWindowX11_OpenGL3_3::BackendMainWindowX11_OpenGL3_3()
    {        
        BackendMainWindowX11_OpenGL3_3::ssGUI_CreateWindow();
        
        ssGUI::Backend::BackendManager::AddMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }

    BackendMainWindowX11_OpenGL3_3::~BackendMainWindowX11_OpenGL3_3()
    {
        if(!IsClosed())
        {
            ssGUI_DestroyWindow();        
        }
        
        ssGUI::Backend::BackendManager::RemoveMainWindowInterface(static_cast<ssGUI::Backend::BackendMainWindowInterface*>(this));
    }
    
    void BackendMainWindowX11_OpenGL3_3::SetWindowPosition(glm::ivec2 pos)
    {
        //Don't support setting window position when in fullscreen mode
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
            return;
        
        XMoveWindow(WindowDisplay, WindowId, pos.x, pos.y);
    }


    glm::ivec2 BackendMainWindowX11_OpenGL3_3::GetWindowPosition() const
    {
        XWindowAttributes attr;
        if(!XGetWindowAttributes(WindowDisplay, WindowId, &attr))
        {
            ssLOG_LINE("Failed to XGetWindowAttributes");
        }

        //https://stackoverflow.com/a/23940869/7519584
        int x, y;
        Window curWindow = WindowId;
        XTranslateCoordinates(WindowDisplay, WindowId, RootWindow(WindowDisplay, DefaultScreen(WindowDisplay)), 
                                0, 0, &x, &y, &curWindow );

        int top, right, left, bot;
        GetWindowDecor(top, right, bot, left);

        return glm::ivec2(x - attr.x - left, y - attr.y - top);
    }

    void BackendMainWindowX11_OpenGL3_3::SyncPositionOffset()
    {
        //TODO: Remove this function
    }

    glm::ivec2 BackendMainWindowX11_OpenGL3_3::GetPositionOffset() const
    {
        int top, right, left, bot;
        GetWindowDecor(top, right, bot, left);
        
        return glm::ivec2(left, top);
    }

    void BackendMainWindowX11_OpenGL3_3::SetWindowSize(glm::ivec2 size)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            UpdateWindowDecor();
            return;
        }
        
        int top, right, left, bot;
        GetWindowDecor(top, right, bot, left);
        
        
        SetRenderSize(size - glm::ivec2(left + right, top + bot));
    }

    glm::ivec2 BackendMainWindowX11_OpenGL3_3::GetWindowSize() const
    {
        int top, right, left, bot;
        GetWindowDecor(top, right, bot, left);
        
        return GetRenderSize() + glm::ivec2(left + right, top + bot);
    }

    void BackendMainWindowX11_OpenGL3_3::SetRenderSize(glm::ivec2 size)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            UpdateWindowDecor();
            return;
        }

        XResizeWindow(WindowDisplay, WindowId, size.x, size.y);
    }
    
    glm::ivec2 BackendMainWindowX11_OpenGL3_3::GetRenderSize() const
    {        
        XWindowAttributes attr;
        XGetWindowAttributes(WindowDisplay, WindowId, &attr);
        return glm::ivec2(attr.width, attr.height);
    }

    bool BackendMainWindowX11_OpenGL3_3::IsClosed() const
    {
        return Closed;
    }

    void BackendMainWindowX11_OpenGL3_3::Close()
    {
        for(int i = 0; i < OnCloseCallback.size(); i++)
        {
            if(OnCloseCallback[i] != nullptr)
                OnCloseCallback[i]();

            if(IsClosingAborted)
            {
                IsClosingAborted = false;
                return;
            }
        }
        
        Closed = true;
        ssGUI_DestroyWindow();
    }
    
    void BackendMainWindowX11_OpenGL3_3::AbortClosing()
    {
        IsClosingAborted = true;
    }

    int BackendMainWindowX11_OpenGL3_3::AddOnCloseEvent(std::function<void()> func)
    {
        OnCloseCallback.push_back(func);
        return OnCloseCallback.size() - 1;
    }

    void BackendMainWindowX11_OpenGL3_3::RemoveOnCloseEvent(int index)
    {
        OnCloseCallback[index] = nullptr;
    }

    void BackendMainWindowX11_OpenGL3_3::SetTitle(std::wstring title)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::string s = converter.to_bytes(title);
        
        XStoreName(WindowDisplay,WindowId, s.data());
        
        //Taken from https://github.com/godotengine/godot/pull/3575/files
        Atom _net_wm_name = XInternAtom(WindowDisplay, "_NET_WM_NAME", false);
        Atom utf8_string = XInternAtom(WindowDisplay, "UTF8_STRING", false);
        XChangeProperty(WindowDisplay, WindowId, _net_wm_name, utf8_string, 8, PropModeReplace, (unsigned char*)s.data(), s.length());
        
        //From https://github.com/godotengine/godot/issues/2952 discussion
        //Xutf8SetWMProperties(x11_display, x11_window, p_title.utf8().get_data(), NULL, NULL, 0, NULL, NULL, NULL);
    }

    std::wstring BackendMainWindowX11_OpenGL3_3::GetTitle() const
    {
        return Title;
    }

    void BackendMainWindowX11_OpenGL3_3::SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage)
    {
        //TODO

    }

    void BackendMainWindowX11_OpenGL3_3::SetVisible(bool visible)
    {
        if(Visible == visible)
            return;
        
        if(!visible)
        {
            LastPositionBeforeHidden = GetWindowPosition();
            XUnmapWindow(WindowDisplay, WindowId);
        }
        else
        {
            XMapWindow(WindowDisplay, WindowId);
            SetWindowPosition(LastPositionBeforeHidden);    
        }

        Visible = visible;
    }

    bool BackendMainWindowX11_OpenGL3_3::IsVisible() const
    {
        return Visible;
    }

    void BackendMainWindowX11_OpenGL3_3::SetVSync(bool vSync)
    {
        SetGLContext();
        GLXDrawable drawable = glXGetCurrentDrawable();
        if(drawable == None)
        {
            ssLOG_LINE("Failed to set VSync, maybe this window is not visible?");
            return;
        }

        glXSwapIntervalEXT(WindowDisplay, glXGetCurrentDrawable(), (int)vSync);
    }

    bool BackendMainWindowX11_OpenGL3_3::IsVSync() const
    {
        unsigned int interval;
        glXQueryDrawable(WindowDisplay, WindowId, GLX_SWAP_INTERVAL_EXT, &interval);
        
        return interval > 0;
    }

    void BackendMainWindowX11_OpenGL3_3::SetFocus(bool focus, bool externalByUser)
    {
        if(externalByUser)
        {
            for(int i = 0; i < ExternalFocusChangedCallback.size(); i++)
            {
                if(ExternalFocusChangedCallback[i] != nullptr)
                    ExternalFocusChangedCallback[i](focus);
            }
        }
        else if(focus)
        {
            if(!XSetInputFocus(WindowDisplay, WindowId, RevertToNone, CurrentTime))
            {
                ssLOG_LINE("Failed to set focus");
            }
        }
    }
    
    //https://gist.github.com/kui/2622504
    Window get_focus_window(Display* d)
    {
        //TODO: Maybe use XSetErrorHandler(handle_error) to handle error
        Window w = 0;
        int revert_to;
        if(!XGetInputFocus(d, &w, &revert_to))
        {
            ssLOG_LINE("Failed to get focused window");
        }

        return w;
    }
    
    bool BackendMainWindowX11_OpenGL3_3::IsFocused() const
    {
        return get_focus_window(WindowDisplay) == WindowId;
    }

    int BackendMainWindowX11_OpenGL3_3::AddFocusChangedByUserEvent(std::function<void(bool focused)> func)
    {
        ExternalFocusChangedCallback.push_back(func);
        return ExternalFocusChangedCallback.size() - 1;
    }

    void BackendMainWindowX11_OpenGL3_3::RemoveFocusChangedByUserEvent(int index)
    {
        ExternalFocusChangedCallback[index] = nullptr;
    }

    void BackendMainWindowX11_OpenGL3_3::SetMSAA(int level)
    {
        MsaaLevel = level;
        ssGUI_DestroyWindow();
        ssGUI_CreateWindow();
        UpdateWindowDecor();
        UpdateWindowCapability();
    }

    int BackendMainWindowX11_OpenGL3_3::GetMSAA() const
    {    
        return MsaaLevel;
    }

    void BackendMainWindowX11_OpenGL3_3::SetTitlebar(bool titlebar)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
    
        Titlebar = titlebar;
        UpdateWindowDecor();
    }

    bool BackendMainWindowX11_OpenGL3_3::HasTitlebar() const
    {
       if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return false;
       
        //XWindowAttributes windowAttribsBack;
        //XGetWindowAttributes(WindowDisplay, WindowId, &windowAttribsBack);
        //windowAttribsBack.override_redirect
        return Titlebar;
    }

    void BackendMainWindowX11_OpenGL3_3::SetResizable(bool resizable)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        Resizable = resizable;
        UpdateWindowCapability();
    }

    bool BackendMainWindowX11_OpenGL3_3::IsResizable() const
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return false;
                
        return Resizable;
    }

    void BackendMainWindowX11_OpenGL3_3::SetCloseButton(bool closeButton)
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return;
        
        //NOTE: For some reason, this doesn't work for my KDE setup. It could be just my setup, idk.
        //bool result = true;
        
        //if(closeButton)
        //    result = AddAllowedWindowActionIfNotExists("_NET_WM_ACTION_CLOSE");
        //else
        //    result = RemoveAllowedWindowActionIfExists("_NET_WM_ACTION_CLOSE");
        
        //if(!result)
        //{
        //    ssLOG_LINE("Failed to set close button using _NET_WM: "<<closeButton);
        //}
        
        CloseButton = closeButton;
        UpdateWindowCapability();
    }

    bool BackendMainWindowX11_OpenGL3_3::HasCloseButton() const
    {
        if(GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN || GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS)
            return false;

        return CloseButton;
    }

    void BackendMainWindowX11_OpenGL3_3::SetWindowMode(ssGUI::Enums::WindowMode windowMode)
    {        
        //NOTE: Using _NET_WM (from https://specifications.freedesktop.org/wm-spec/wm-spec-1.5.html spec)
        //      Is really a hit or miss to be honest, at least on my system.
        //      Some of the function works and some don't.
        //
        //      Motif (MWM) seems to work relatively reliable (again at least on my system)
        //      so I am using it instead. 
        //      Technically, I can set both but this has driven me crazy rn so I will leave this for another day.
        
        //TODO: Set multiple Window System Atoms for backward and forward compatibility
        
        //If we are coming from fullscreen, then set the resoluton back
        if(CurrentWindowMode == ssGUI::Enums::WindowMode::FULLSCREEN && windowMode != ssGUI::Enums::WindowMode::FULLSCREEN)
        {
            if(!OriginalResolutionSet)
            {
                ssLOG_LINE("OriginalResolutionSet not set, this is probably a bug, please report.");
                return;
            }
            
            ReturnToOriginalResolution();   
        }
        
        if(windowMode == Enums::WindowMode::NORMAL)
        {
            XClientEventData data;
            data.l[0] = 0;  //_NET_WM_STATE_REMOVE
            data.l[1] = XInternAtom(WindowDisplay, "_NET_WM_STATE_FULLSCREEN", False);
            data.l[2] = 0;
            data.l[3] = 0;
            data.l[4] = 0;
            
            if(!SendXClientEvent(WindowDisplay, RootWindow(WindowDisplay, DefaultScreen(WindowDisplay)), WindowId, "_NET_WM_STATE", data, 32))
            {
                ssLOG_LINE("Failed to call SendXClientEvent");
            }

            MwmHintsData hintData;
            hintData.flags = MWM_HINTS::MWM_HINTS_DECORATIONS;
            hintData.decorations = MWM_DECOR::MWM_DECOR_ALL;

            XChangeProperty(WindowDisplay, 
                            WindowId, 
                            XInternAtom(WindowDisplay, "_MOTIF_WM_HINTS", False), 
                            XInternAtom(WindowDisplay, "_MOTIF_WM_HINTS", False), 
                            32, 
                            PropModeReplace, 
                            (unsigned char*)&hintData, 
                            5);
        }
        else
        {
            if(windowMode == ssGUI::Enums::WindowMode::FULLSCREEN)
            {
                glm::ivec2 renderSize = GetRenderSize();
                SetScreenResolution(renderSize);   
            }
            
            XClientEventData data;
            data.l[0] = 1;  //_NET_WM_STATE_ADD
            data.l[1] = XInternAtom(WindowDisplay, "_NET_WM_STATE_FULLSCREEN", False);
            data.l[2] = 0;
            data.l[3] = 0;
            data.l[4] = 0;
            
            if(!SendXClientEvent(WindowDisplay, RootWindow(WindowDisplay, DefaultScreen(WindowDisplay)), WindowId, "_NET_WM_STATE", data, 32))
            {
                ssLOG_LINE("Failed to call SendXClientEvent");
            }
        }
       
        CurrentWindowMode = windowMode;
    }

    ssGUI::Enums::WindowMode BackendMainWindowX11_OpenGL3_3::GetWindowMode() const
    {
        return CurrentWindowMode;
    }

    bool BackendMainWindowX11_OpenGL3_3::SetGLContext()
    {
        int result = glXMakeCurrent(WindowDisplay, WindowId, WindowContext);
        return result == True;
    }
    
    BackendMainWindowInterface* BackendMainWindowX11_OpenGL3_3:: Clone()
    {
        return new BackendMainWindowX11_OpenGL3_3(*this);
    }

    void* BackendMainWindowX11_OpenGL3_3::GetRawHandle()
    {
        CurrentHandle = {
            WindowDisplay,
            WindowColormap,
            WindowId,
            WindowContext,
            WindowCloseEventId
        };
        
        return &CurrentHandle;
    }
}

}