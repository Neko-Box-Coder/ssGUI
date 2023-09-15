#ifndef SSGUI_WINDOW_LAYOUT_ITEM_ENFORCER_H
#define SSGUI_WINDOW_LAYOUT_ITEM_ENFORCER_H

#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::WindowLayoutItemEnforcer
    This extension is only for window GUI Objects which allows the user 
    to "pull" and "push" the layout items by recording the container state.

    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::GUIObject* Container;    //See <BindToObject>
        bool Enabled;                   //See <IsEnabled>
        glm::vec2 ContainerStartPos;    //(Internal variable) Used to store the starting position of the window to allow pushing/pulling
        glm::vec2 ContainerStartSize;   //(Internal variable) Used to store the starting size of the window to allow pushing/pulling
        bool ContainerResizeStarted;    //(Internal variable) Flag to indicate if the coontainer is being resized and allow pushing/pulling
        glm::vec2 LastContainerSize;    //(Internal variable) Used to skip recalcuating the position and size if the size is not changed
    =================================================================
    ============================== C++ ==============================
    WindowLayoutItemEnforcer::WindowLayoutItemEnforcer() :  Container(nullptr),
                                                            Enabled(true),
                                                            ContainerStartPos(),
                                                            ContainerStartSize(),
                                                            ContainerResizeStarted(false),
                                                            LastContainerSize()
    {}
    =================================================================
    */
    class WindowLayoutItemEnforcer : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            ssGUI::GUIObject* Container;    //See <BindToObject>
            bool Enabled;                   //See <IsEnabled>
            glm::vec2 ContainerStartPos;    //(Internal variable) Used to store the starting position of the window to allow pushing/pulling
            glm::vec2 ContainerStartSize;   //(Internal variable) Used to store the starting size of the window to allow pushing/pulling
            bool ContainerResizeStarted;    //(Internal variable) Flag to indicate if the coontainer is being resized and allow pushing/pulling
            glm::vec2 LastContainerSize;    //(Internal variable) Used to skip recalcuating the position and size if the size is not changed

            WindowLayoutItemEnforcer& operator=(WindowLayoutItemEnforcer const& other);

        protected:
            WindowLayoutItemEnforcer();
            virtual ~WindowLayoutItemEnforcer() override;
            WindowLayoutItemEnforcer(WindowLayoutItemEnforcer const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
        
        public:
            static const std::string EXTENSION_NAME;

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual WindowLayoutItemEnforcer* Clone() override;
    };
}

}

#endif