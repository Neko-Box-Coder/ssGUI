#include "ssGUI/DataClasses/BlockData.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    void BlockData::UnsetBlockData()
    {
        CurrentBlockDataType = ssGUI::Enums::BlockDataType::NONE;
    }
    
    ssGUI::Enums::BlockDataType BlockData::GetBlockDataType() const
    {
        return CurrentBlockDataType;
    }
    
    void BlockData::SetCustomDataIdentifier(const std::string& identifier)
    {
        CustomDataIdentifier = identifier;
    }
    
    std::string BlockData::GetCustomDataIdentifier() const
    {
        return CustomDataIdentifier;
    }
    
    void BlockData::SetBlockData(ssGUI::GUIObject* data)
    {
        Data.CurrentGUIObject = data;
        CurrentBlockDataType = ssGUI::Enums::BlockDataType::GUI_OBJECT;
    }
    
    void BlockData::SetBlockData(ssGUI::Extensions::Extension* data)
    {
        Data.CurrentExtension = data;
        CurrentBlockDataType = ssGUI::Enums::BlockDataType::EXTENSION;
    }
    
    void BlockData::SetBlockData(ssGUI::EventCallback* data)
    {
        Data.CurrentEventCallback = data;
        CurrentBlockDataType = ssGUI::Enums::BlockDataType::EVENT_CALLBACK;
    }
    
    void BlockData::SetBlockData(void* data)
    {
        Data.CurrentCustomData = data;
        CurrentBlockDataType = ssGUI::Enums::BlockDataType::CUSTOM;
    }
    
    template<>
    ssGUI::GUIObject* BlockData::GetBlockData() const
    {
        if(CurrentBlockDataType != ssGUI::Enums::BlockDataType::GUI_OBJECT)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid access of Block data");
            ssGUI_ERROR(ssGUI_TAG_DATA, "Trying to access GUI Object but get " << 
                                        Enums::BlockDataTypeToString(CurrentBlockDataType) << " instead.");
            return nullptr;
        }
        
        return Data.CurrentGUIObject;
    }
    
    template<>
    ssGUI::Extensions::Extension* BlockData::GetBlockData() const
    {
        if(CurrentBlockDataType != ssGUI::Enums::BlockDataType::EXTENSION)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid access of Block data");
            ssGUI_ERROR(ssGUI_TAG_DATA, "Trying to access Extension but get " << 
                                        Enums::BlockDataTypeToString(CurrentBlockDataType) << " instead.");
            return nullptr;
        }
        
        return Data.CurrentExtension;
    }
    
    template<>
    ssGUI::EventCallback* BlockData::GetBlockData() const
    {
        if(CurrentBlockDataType != ssGUI::Enums::BlockDataType::EVENT_CALLBACK)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid access of Block data");
            ssGUI_ERROR(ssGUI_TAG_DATA, "Trying to access Event Callback but get " << 
                                        Enums::BlockDataTypeToString(CurrentBlockDataType) << " instead.");
            return nullptr;
        }
        
        return Data.CurrentEventCallback;
    }
    
    template<>
    void* BlockData::GetBlockData() const
    {
        if(CurrentBlockDataType != ssGUI::Enums::BlockDataType::CUSTOM)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid access of Block data");
            ssGUI_ERROR(ssGUI_TAG_DATA, "Trying to access Custom Data but get " << 
                                        Enums::BlockDataTypeToString(CurrentBlockDataType) << " instead.");
            return nullptr;
        }
        
        return Data.CurrentCustomData;
    }
}