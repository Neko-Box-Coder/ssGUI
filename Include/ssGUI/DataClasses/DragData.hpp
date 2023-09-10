#ifndef SSGUI_DRAG_DATA_HPP
#define SSGUI_DRAG_DATA_HPP

#include "ssGUI/Enums/DragDataType.hpp"
#include <string>

namespace ssGUI
{
    class GUIObject;
    
    /*class: ssGUI::DragData
    This class holds the data that the mouse is currently dragging
    
    Variables:
    ============================== C++ ==============================
    private:
        ssGUI::Enums::DragDataType CurrentDragDataType = ssGUI::Enums::DragDataType::NONE;
        std::string CustomDataIdentifier = "";
        
        union 
        {
            ssGUI::GUIObject* GUIObject;
            void* CustomData;
        } Data;
    =================================================================
    */
    class DragData
    {
        private:
            ssGUI::Enums::DragDataType CurrentDragDataType = ssGUI::Enums::DragDataType::NONE;
            std::string CustomDataIdentifier = "";
            
            union 
            {
                ssGUI::GUIObject* GUIObject;
                void* CustomData;
            } Data;
    
        public:
            //function: UnsetDragData
            //Unsets the drag data, meaning <GetDragDataType> will return <ssGUI::Enums::DragDataType::NONE>
            void UnsetDragData();
        
            //function: GetDragDataType
            //Gets the drag data type
            ssGUI::Enums::DragDataType GetDragDataType() const;
            
            //function: SetCustomDataIdentifier
            //Sets the identifier for custom data for the block data type <ssGUI::Enums::DragDataType::CUSTOM>
            void SetCustomDataIdentifier(const std::string& identifier);
            
            //function: GetCustomDataIdentifier
            //Gets the identifier for custom data for the block data type <ssGUI::Enums::DragDataType::CUSTOM>
            std::string GetCustomDataIdentifier() const;
            
            //function: SetDragData
            //Sets the drag data for <ssGUI::GUIObject>
            void SetDragData(ssGUI::GUIObject* data);
            
            //function: SetDragData
            //Sets the drag data for custom data
            void SetDragData(void* data);
            
            //function: GetDragData
            //Sets the drag data for the corresponding data type. 
            //To see which data type, call <GetDragDataType>
            template<typename T>
            T* GetDragData() const;
    };
}

#endif