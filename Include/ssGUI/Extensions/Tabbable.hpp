#ifndef SSGUI_TABBABLE_HPP
#define SSGUI_TABBABLE_HPP

#include "ssGUI/Extensions/Extension.hpp"

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::Tabbable
    A template Extension. Use this to create new Extension Classes.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;        //See <BindToObject>
        bool Enabled;                       //See <IsEnabled>
    =================================================================
    ============================== C++ ==============================
    Tabbable::Tabbable() :    Container(nullptr),
                                                Enabled(true)
    {}
    =================================================================
    */
    class Tabbable : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            Tabbable& operator=(Tabbable const& other);
        
        protected:
            ssGUI::GUIObject* Container;        //See <BindToObject>
            bool Enabled;                       //See <IsEnabled>

            ssGUIObjectIndex TabAreaObject;
            
            ObjectsReferences CurrentObjectsReferences;                     //See <Internal_GetObjectsReferences>
            ssGUIObjectIndex TopLevelParent;                                //See <GetTopLevelParent>
            bool Untabbable;                                                //See 
            
            glm::vec2 LastMouseDragBeginPosition = glm::vec2();
            

            Tabbable();
            virtual ~Tabbable() override;
            Tabbable(Tabbable const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(   ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset) override;

            //virtual void DrawTabPreview(glm::vec2 mousePos);

        public:
            //====================================================================
            //Group: Constants
            //====================================================================

            //string: EXTENSION_NAME
            static const std::string EXTENSION_NAME;


            virtual void SetTopLevelParent(ssGUI::GUIObject* topLevelParent);
            virtual ssGUI::GUIObject* GetTopLevelParent() const;
            virtual void SetUntabbable(bool untabbable);
            virtual bool IsUntabbable() const;

            virtual glm::vec2 GetLastMouseDragBeginPosition() const;
            virtual void SetLastMouseDragBeginPosition(glm::vec2 pos);

            //virtual void AddExternalTab(ssGUI::GUIObject* exteranlObject, bool dockAfter);
            ssGUI::GUIObject* GetTabAreaObject() const;
            void SetTabAreaObject(ssGUI::GUIObject* tabArea);


            //====================================================================
            //Group: Overrides
            //====================================================================

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
            virtual void Internal_Draw( bool isPreRender, 
                                        ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                        ssGUI::GUIObject* mainWindow, 
                                        glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() const override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(const ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual Tabbable* Clone() override;
    };
}

}

#endif