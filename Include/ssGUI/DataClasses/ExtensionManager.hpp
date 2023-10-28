#ifndef SSGUI_EXTENSION_MANAGER_H
#define SSGUI_EXTENSION_MANAGER_H

#include <unordered_map>
#include <type_traits>
#include <vector>
#include <string>
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/Extensions/Extension.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Renderer;
    class GUIObject;

    /*class: ssGUI::ExtensionManager
    This manages all the extensions for a GUI Object

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        std::unordered_map<std::string, ssGUI::Extensions::Extension*> Extensions;      //See <GetExtension>
        std::vector<std::string> ExtensionsDrawOrder;                                   //See <GetExtensionDrawOrder>
        std::vector<std::string> ExtensionsUpdateOrder;                                 //See <GetExtensionUpdateOrder>

        ssGUI::Renderer* CurrentRenderer;                                               //(Internal variable) Used to send redraw signal
        ssGUI::GUIObject* CurrentObject;                                                //(Internal variable) Used to register container
    =================================================================
    ============================== C++ ==============================
    ExtensionManager::ExtensionManager() :  Extensions(),
                                            ExtensionsDrawOrder(),
                                            ExtensionsUpdateOrder(),
                                            CurrentRenderer(nullptr),
                                            CurrentObject(nullptr)
    {}
    =================================================================
    */
    class ExtensionManager
    {
        protected:
            std::unordered_map<std::string, ssGUI::Extensions::Extension*> Extensions;      //See <GetExtension>
            std::vector<std::string> ExtensionsDrawOrder;                                   //See <GetExtensionDrawOrder>
            std::vector<std::string> ExtensionsUpdateOrder;                                 //See <GetExtensionUpdateOrder>

            ssGUI::Renderer* CurrentRenderer;                                               //(Internal variable) Used to send redraw signal
            ssGUI::GUIObject* CurrentObject;                                                //(Internal variable) Used to register container

            ExtensionManager(ExtensionManager const&);

            ssGUI::Extensions::Extension* GetExtension(std::string extensionName);

            bool IsExtensionExist(std::string extensionName) const;

            void RemoveExtension(std::string extensionName);

            int GetExtensionDrawOrder(std::string extensionName) const;

            void ChangeExtensionDrawOrder(std::string extensionName, int order);

            int GetExtensionUpdateOrder(std::string extensionName) const;

            void ChangeExtensionUpdateOrder(std::string extensionName, int order);

        public:
            ExtensionManager();
            virtual ~ExtensionManager();

            virtual void SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::GUIObject* obj);
            
            //====================================================================
            //Group: Adding, Getting And Removing Extension
            //====================================================================
            
            //function: AddExtension
            //Adds an extension to this GUI Object. If the extension already exists, nothing will be modified.
            template<typename T>
            T* AddExtension()
            {
                if(std::is_base_of<ssGUI::Extensions::Extension, T>::value)
                {
                    if(IsExtensionExist<T>())
                        return GetExtension<T>();

                    auto* extension = ssGUI::Factory::Create<T>();
                    Extensions[extension->GetExtensionName()] = extension;
                    ExtensionsDrawOrder.push_back(extension->GetExtensionName());
                    ExtensionsUpdateOrder.push_back(extension->GetExtensionName());
                    extension->BindToObject(CurrentObject);
                    CurrentRenderer->RedrawObject();
                    return extension;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non extension type");
                    return nullptr;
                }                
            }
            
            //function: AddExtensionCopy
            //Adds an extension to this GUI Object by copying an already existing extension. 
            //If the extension already exists in this GUI object, nothing will be modified.
            template<typename T>
            T* AddExtensionCopy(T* copy)
            {
                if(std::is_base_of<ssGUI::Extensions::Extension, T>::value)
                {
                    if(IsExtensionExist(copy->GetExtensionName()))
                        return static_cast<T*>(GetExtension(copy->GetExtensionName()));

                    auto* extension = copy->Clone();
                    Extensions[extension->GetExtensionName()] = extension;
                    ExtensionsDrawOrder.push_back(extension->GetExtensionName());
                    ExtensionsUpdateOrder.push_back(extension->GetExtensionName());
                    extension->BindToObject(CurrentObject);
                    CurrentRenderer->RedrawObject();
                    return extension;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non extension type");
                    return nullptr;
                }                
            }
            
            //function: GetExtension
            //Gets the extension by the name of it. Nullptr will be returned if not found.
            template <typename T>
            T* GetExtension()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return static_cast<T*>(GetExtension(T::EXTENSION_NAME));
            }
            
            //function: GetListOfExtensions
            //Returns all the extensions on the GUI Object
            virtual std::vector<ssGUI::Extensions::Extension*> GetListOfExtensions();

            //function: IsExtensionExist
            //Returns true if the extension exists on this GUI Object
            template <typename T>
            bool IsExtensionExist()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return IsExtensionExist(T::EXTENSION_NAME);
            }
            
            //function: RemoveExtension
            //Removes the extension by the name of it
            template <typename T>
            void RemoveExtension()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                RemoveExtension(T::EXTENSION_NAME);
            }
            
            //function: GetExtensionsCount
            //Returns the number of extensions on this GUI Object
            virtual int GetExtensionsCount() const;
            
            //====================================================================
            //Group: Changing Extension Execution Order
            //====================================================================
            
            //function: GetExtensionDrawOrder
            //Returns the draw order of the extension by the name of it
            template <typename T>
            int GetExtensionDrawOrder()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return GetExtensionDrawOrder(T::EXTENSION_NAME);
            }
            
            //function: ChangeExtensionDrawOrder
            //Changes the draw order of the extension by the name of it
            template <typename T>
            void ChangeExtensionDrawOrder(int order)
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                ChangeExtensionDrawOrder(T::EXTENSION_NAME, order);
            }
            
            //function: GetExtensionUpdateOrder
            //Returns the update order of the extension by the name of it
            template <typename T>
            int GetExtensionUpdateOrder()
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                return GetExtensionUpdateOrder(T::EXTENSION_NAME);
            }
            
            //function: ChangeExtensionUpdateOrder
            //Changes the update order of the extension by the name of it
            template <typename T>
            void ChangeExtensionUpdateOrder(int order)
            {
                static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                static_assert(!std::is_same<ssGUI::Extensions::Extension, T>::value, "Invalid Type to get extension");
                ChangeExtensionUpdateOrder(T::EXTENSION_NAME, order);
            }
    };
}

#endif