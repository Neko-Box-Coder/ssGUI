#include "ssGUI/DataClasses/DragData.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    void DragData::UnsetDragData()
    {
        CurrentDragDataType = ssGUI::Enums::DragDataType::NONE;
    }
    
    ssGUI::Enums::DragDataType DragData::GetDragDataType() const
    {
        return CurrentDragDataType;
    }
    
    void DragData::SetCustomDataIdentifier(const std::string& identifier)
    {
        CustomDataIdentifier = identifier;
    }
    
    std::string DragData::GetCustomDataIdentifier() const
    {
        return CustomDataIdentifier;
    }
    
    void DragData::SetDragData(ssGUI::GUIObject* data)
    {
        Data.GUIObject = data;
        CurrentDragDataType = ssGUI::Enums::DragDataType::GUI_OBJECT;
    }
    
    void DragData::SetDragData(void* data)
    {
        Data.CustomData = data;
        CurrentDragDataType = ssGUI::Enums::DragDataType::CUSTOM;
    }
    
    void DragData::SetInterceptor(ssGUI::GUIObject* interceptor)
    {
        Interceptor = interceptor;
    }
    
    bool DragData::IsIntercepted() const
    {
        return Interceptor != nullptr;
    }
    
    ssGUI::GUIObject* DragData::GetInterceptor() const
    {
        return Interceptor;
    }
    
    template<>
    ssGUI::GUIObject* DragData::GetDragData() const
    {
        if(CurrentDragDataType != ssGUI::Enums::DragDataType::GUI_OBJECT)
        {
            ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid access of drag data");
            ssGUI_ERROR(ssGUI_DATA_TAG, "Trying to access GUI Object but get " << 
                                        Enums::DragDataTypeToString(CurrentDragDataType) << " instead.");
            return nullptr;
        }
        
        return Data.GUIObject;
    }
    
    template<>
    void* DragData::GetDragData() const
    {
        if(CurrentDragDataType != ssGUI::Enums::DragDataType::CUSTOM)
        {
            ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid access of drag data");
            ssGUI_ERROR(ssGUI_DATA_TAG, "Trying to access Custom Data but get " << 
                                        Enums::DragDataTypeToString(CurrentDragDataType) << " instead.");
            return nullptr;
        }
        
        return Data.CustomData;
    }
}