#ifndef SSGUI_BLOCK_DATA_HPP
#define SSGUI_BLOCK_DATA_HPP

#include "ssGUI/Enums/BlockDataType.hpp"
#include <string>

namespace ssGUI
{
    class GUIObject;
    class EventCallback;
    namespace Extensions
    {
        class Extension;
    }
    
    //class: ssGUI::BlockData
    //This class holds the data that is currently blocking the input
    class BlockData
    {
        private:
            ssGUI::Enums::BlockDataType CurrentBlockDataType = ssGUI::Enums::BlockDataType::NONE;
            std::string CustomDataIdentifier = "";
            
            union 
            {
                ssGUI::GUIObject* CurrentGUIObject;
                ssGUI::Extensions::Extension* CurrentExtension;
                ssGUI::EventCallback* CurrentEventCallback;
                void* CurrentCustomData;
            } Data;
    
        public:
            //function: UnsetBlockData
            //Unsets the block data, meaning <GetBlockDataType> will return <ssGUI::Enums::BlockDataType::NONE>
            void UnsetBlockData();
        
            //function: GetBlockDataType
            //Gets the block data type
            ssGUI::Enums::BlockDataType GetBlockDataType() const;
            
            //function: SetCustomDataIdentifier
            //Sets the identifier for custom data for the block data type <ssGUI::Enums::BlockDataType::CUSTOM>
            void SetCustomDataIdentifier(const std::string& identifier);
            
            //function: GetCustomDataIdentifier
            //Gets the identifier for custom data for the block data type <ssGUI::Enums::BlockDataType::CUSTOM>
            std::string GetCustomDataIdentifier() const;
            
            //function: SetBlockData
            //Sets the block data for <ssGUI::GUIObject>
            void SetBlockData(ssGUI::GUIObject* data);
            
            //function: SetBlockData
            //Sets the block data for <ssGUI::Extensions::Extension>
            void SetBlockData(ssGUI::Extensions::Extension* data);
            
            //function: SetBlockData
            //Sets the block data for <ssGUI::EventCallback>
            void SetBlockData(ssGUI::EventCallback* data);
            
            //function: SetBlockData
            //Sets the block data for custom data
            void SetBlockData(void* data);
            
            //function:  GetBlockData
            //Sets the block data for the corresponding data type. 
            //To see which data type, call <GetBlockDataType>
            template<typename T>
            T* GetBlockData() const;
    };
}

#endif