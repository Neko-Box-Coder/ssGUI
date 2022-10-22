/*
#include <iostream>
#include "ProjectConfig.hpp"
#include "ssGUI/ssGUIManager.hpp"
#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
*/


#include <iostream>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssLogger/ssLog.hpp"

#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"



// void PrintEnter(ssGUI::ssGUIManager& manager)
// {
//     ssGUI::Backend::BackendSystemInputInterface* inputInterface = manager.GetBackendInputInterface();

//     if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
//         !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
//     {
//         std::cout<<"Enter key down\n";
        
//     }
//     else if(!inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
//         inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
//     {
        
//         std::cout<<"Enter key up\n";
//     }
// }

// void MoveWindow(ssGUI::ssGUIManager& manager, ssGUI::Window& window, ssGUI::Window& childWindow)
// {
//     ssGUI::Backend::BackendSystemInputInterface* inputInterface = manager.GetBackendInputInterface();
//     ssGUI::KeyPresses currentKeyPresses = inputInterface->GetCurrentKeyPresses();

//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT))
//         window.SetPosition(window.GetPosition() + glm::ivec2(-1, 0));
//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::RIGHT))
//         window.SetPosition(window.GetPosition() + glm::ivec2(1, 0));
//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::DOWN))
//         window.SetPosition(window.GetPosition() + glm::ivec2(0, 1));
//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::UP))
//         window.SetPosition(window.GetPosition() + glm::ivec2(0, -1));

//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::A))
//         window.SetSize(window.GetSize() + glm::ivec2(-1, 0));
//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::D))
//         window.SetSize(window.GetSize() + glm::ivec2(1, 0));
//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::S))
//         window.SetSize(window.GetSize() + glm::ivec2(0, 1));
//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::W))
//         window.SetSize(window.GetSize() + glm::ivec2(0, -1));

//     if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
//         !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
//     {
//         window.SetEnabled(window.IsEnabled() ? false : true);
//         //std::cout<<"WindowPos: "<<window.GetGlobalPosition().x<<", "<<window.GetGlobalPosition().y<<"\n";
//         //std::cout<<"childWindowPos: "<<childWindow.GetGlobalPosition().x<<", "<<childWindow.GetGlobalPosition().y<<"\n";
//     }
// }































