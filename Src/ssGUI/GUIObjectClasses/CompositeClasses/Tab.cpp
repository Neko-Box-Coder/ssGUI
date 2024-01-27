#include "ssGUI/GUIObjectClasses/CompositeClasses/Tab.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Shape.hpp"

namespace ssGUI
{
    glm::vec2 Tab::DefaultCloseButtonSize = glm::vec2(15, 15);

    Tab::Tab(const Tab& other) : StandardButton(other)
    {
        TabColor = other.TabColor;
        SelectedColor = other.SelectedColor;
        UnderlineColor = other.UnderlineColor;
        UnderlineThickness = other.UnderlineThickness;
        UnderlineDirection = other.UnderlineDirection;
        CloseButton = other.CloseButton;
        CloseButtonShapeIds[0] = other.CloseButtonShapeIds[0];
        CloseButtonShapeIds[1] = other.CloseButtonShapeIds[1];
        ContentObject = other.ContentObject;
        Selected = other.Selected;
    }

    void Tab::SetCloseSymbolColor(glm::u8vec4 color)
    {
        ssGUI::GUIObject* closeButton = CurrentObjectsReferences.GetObjectReference(CloseButton);

        if(closeButton == nullptr)
            return;

        auto* closeButtonShape = closeButton->GetExtension<ssGUI::Extensions::Shape>();

        if(closeButtonShape == nullptr)
            return;

        std::vector<glm::u8vec4>* colorVec =
            closeButtonShape->GetAdditionalShapeColorsWithID(CloseButtonShapeIds[0]);

        for(int i = 0; i < colorVec->size(); ++i)
            colorVec->at(i) = color;

        colorVec = closeButtonShape->GetAdditionalShapeColorsWithID(CloseButtonShapeIds[1]);
        for(int i = 0; i < colorVec->size(); ++i)
            colorVec->at(i) = color;
    }

    void Tab::InitializeCloseButton()
    {
        //Create Close button
        auto* closeButton = AddChildWithWrapper<ssGUI::Button>(true);
        closeButton->AddExtension<ssGUI::Extensions::AdvancedPosition>();
        closeButton->GetParent()->SetMaxSize(glm::vec2(DefaultCloseButtonSize.x * 1.5f,
                                                       closeButton->GetParent()->GetMaxSize().y));

        closeButton->SetMinSize(DefaultCloseButtonSize);
        closeButton->SetMaxSize(DefaultCloseButtonSize);

        ssGUI::EventCallback* ecb =
            closeButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);

        ecb->ClearEventListeners();
        closeButton->RemoveExtension<ssGUI::Extensions::Border>();

        closeButton->SetButtonColor(glm::u8vec4(255, 255, 255, 0));

        auto* closeSymbolShape = closeButton->AddExtension<ssGUI::Extensions::Shape>();
        CloseButtonShapeIds[0] = closeSymbolShape->AddAdditionalLine();
        CloseButtonShapeIds[1] = closeSymbolShape->AddAdditionalLine();
        glm::u8vec4 closeSymbolColor = GetAdaptiveTextColor();

        closeSymbolShape->SetAdditionalLine(CloseButtonShapeIds[0],
                                            glm::vec2(5.f, 5.f),
                                            glm::vec2(10.f, 10.f),
                                            2,
                                            2,
                                            closeSymbolColor,
                                            closeSymbolColor,
                                            false);

        closeSymbolShape->SetAdditionalLine(CloseButtonShapeIds[1],
                                            glm::vec2(10.f, 5.f),
                                            glm::vec2(5.f, 10.f),
                                            2,
                                            2,
                                            closeSymbolColor,
                                            closeSymbolColor,
                                            false);

        ssGUIObjectIndex tabId = ecb->AddObjectReference(this);
        ecb->AddEventListener(ListenerKey,
                              this,
                              [tabId](ssGUI::EventInfo& info)
                              {
                                  ssGUI::Tab* tab =
                                      info.References->GetObjectReference<ssGUI::Tab>(tabId);

                                  if(tab == nullptr)
                                  {
                                      info.DeleteCurrentListener = true;
                                      return;
                                  }

                                  ssGUI::Button* closeButton =
                                      static_cast<ssGUI::Button*>(info.Container);
                                  glm::u8vec4 closeButtonColor = closeButton->GetButtonColor();
                                  glm::u8vec4 closeSymbolColor = tab->GetAdaptiveTextColor();

                                  static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6,
                                                "Make sure this is updated");
                                  switch(closeButton->GetButtonState())
                                  {
                                      case ssGUI::Enums::ButtonState::NORMAL:
                                          closeButtonColor.a = 0;
                                          break;
                                      case ssGUI::Enums::ButtonState::HOVER:
                                          closeButtonColor.a = 100;
                                          break;
                                      case ssGUI::Enums::ButtonState::ON_CLICK:
                                      case ssGUI::Enums::ButtonState::CLICKING:
                                          closeButtonColor.a = 50;
                                          break;
                                      case ssGUI::Enums::ButtonState::CLICKED:
                                      case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                                          closeButtonColor.a = 0;
                                          break;
                                  }

                                  closeButton->SetBackgroundColor(closeButtonColor);
                                  tab->SetCloseSymbolColor(closeSymbolColor);
                              });

        closeButton->NotifyButtonEventCallbackManually();
        CloseButton = CurrentObjectsReferences.AddObjectReference(closeButton);
    }

    void Tab::MainLogic(ssGUI::Backend::SystemInputInterface* inputInterface,
                        ssGUI::InputStatus& currentInputStatus,
                        ssGUI::InputStatus& lastInputStatus,
                        ssGUI::GUIObject* mainWindow)
    {
        StandardButton::MainLogic(inputInterface, currentInputStatus, lastInputStatus, mainWindow);

        ssGUI::Button* closeButton =
            (ssGUI::Button*)CurrentObjectsReferences.GetObjectReference(CloseButton);

        if(currentInputStatus.MouseInputBlockedData.GetBlockDataType() ==
               ssGUI::Enums::BlockDataType::GUI_OBJECT &&
           currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>() == closeButton)
        {
            SetButtonState(ssGUI::Enums::ButtonState::HOVER);
        }
    }

    const std::string Tab::ListenerKey = "Tab";

    Tab::Tab() :
        StandardButton(),
        TabColor(100, 100, 100, 255),
        SelectedColor(127, 127, 127, 255),
        UnderlineColor(255, 255, 255, 127),
        UnderlineThickness(3),
        UnderlineDirection(ssGUI::Enums::Direction::BOTTOM),
        CloseButton(-1),
        CloseButtonShapeIds{-1, -1},
        ContentObject(-1),
        Selected(false)
    {
        GetExtension<ssGUI::Extensions::Layout>()->ClearAllPreferredSizeMultiplier();
        RemoveExtension<ssGUI::Extensions::RoundedCorners>();
        RemoveExtension<ssGUI::Extensions::Outline>();
        auto* boxShadow = GetExtension<ssGUI::Extensions::BoxShadow>();
        boxShadow->SetSizeOffset(glm::vec2(boxShadow->GetSizeOffset().x, 0));

        GetButtonTextObject()->SetNewTextFontSize(14);

        InitializeCloseButton();
    }

    Tab::~Tab()
    {
    }

    void Tab::SetAssociatedContent(ssGUI::GUIObject* content)
    {
        ContentObject = CurrentObjectsReferences.AddObjectReference(content);
    }

    ssGUI::GUIObject* Tab::GetAssociatedContent() const
    {
        if(ContentObject < 0)
            return nullptr;

        return CurrentObjectsReferences.GetObjectReference(ContentObject);
    }

    void Tab::SetTabColor(glm::u8vec4 color)
    {
        TabColor = color;
        if(!Selected)
            SetButtonColor(TabColor);

        glm::u8vec4 closeButtonColor = GetAdaptiveTextColor();
        SetCloseSymbolColor(closeButtonColor);
    }

    glm::u8vec4 Tab::GetTabColor() const
    {
        return TabColor;
    }

    void Tab::SetSelectedColor(glm::u8vec4 color)
    {
        SelectedColor = color;
        if(Selected)
            SetButtonColor(SelectedColor);

        glm::u8vec4 closeButtonColor = GetAdaptiveTextColor();
        SetCloseSymbolColor(closeButtonColor);
    }

    glm::u8vec4 Tab::GetSelectedColor() const
    {
        return SelectedColor;
    }

    void Tab::SetUnderlineColor(glm::u8vec4 color)
    {
        UnderlineColor = color;
    }

    glm::u8vec4 Tab::GetUnderlineColor() const
    {
        return UnderlineColor;
    }

    void Tab::SetUnderlineDirection(ssGUI::Enums::Direction direction)
    {
        UnderlineDirection = direction;
    }

    ssGUI::Enums::Direction Tab::GetUnderlineDirection() const
    {
        return UnderlineDirection;
    }

    void Tab::SetUnderlineThickness(float thickness)
    {
        UnderlineThickness = thickness;
    }

    float Tab::GetUnderlineThickness() const
    {
        return UnderlineThickness;
    }

    void Tab::SetCloseButton(ssGUI::Button* button)
    {
        ssGUI::GUIObject* closeButtonObj = nullptr;

        if(CloseButton >= 0)
            closeButtonObj = CurrentObjectsReferences.GetObjectReference(CloseButton);

        if(button == closeButtonObj)
            return;

        if(closeButtonObj != nullptr)
            closeButtonObj->Delete();

        if(button == nullptr)
        {
            CloseButton = -1;
            return;
        }

        button->SetParent(this);
        CloseButton = CurrentObjectsReferences.AddObjectReference(button);
    }

    ssGUI::Button* Tab::GetCloseButton() const
    {
        if(CloseButton < 0)
            return nullptr;

        return (ssGUI::Button*)CurrentObjectsReferences.GetObjectReference(CloseButton);
    }

    void Tab::SelectTab(bool select)
    {
        Selected = select;

        if(Selected)
            SetButtonColor(SelectedColor);
        else
            SetButtonColor(TabColor);

        glm::u8vec4 closeButtonColor = GetAdaptiveTextColor();
        SetCloseSymbolColor(closeButtonColor);

        //Add border extension if not exists
        auto* border = AddExtension<ssGUI::Extensions::Border>();

        border->SetInnerBorder(true);

        border->ShowBorderTop(false);
        border->ShowBorderRight(false);
        border->ShowBorderLeft(false);
        border->ShowBorderBottom(false);

        if(select)
        {
            if(UnderlineDirection == ssGUI::Enums::Direction::TOP)
                border->ShowBorderTop(true);
            else if(UnderlineDirection == ssGUI::Enums::Direction::RIGHT)
                border->ShowBorderRight(true);
            else if(UnderlineDirection == ssGUI::Enums::Direction::BOTTOM)
                border->ShowBorderBottom(true);
            else if(UnderlineDirection == ssGUI::Enums::Direction::LEFT)
                border->ShowBorderLeft(true);
        }

        border->SetBorderColor(UnderlineColor);
        border->SetBorderWidth(UnderlineThickness);
    }

    bool Tab::IsTabSelected() const
    {
        return Selected;
    }

    void Tab::SetTabIcon(ssGUI::Image* icon)
    {
        StandardButton::SetButtonIconObject(icon);
    }

    ssGUI::Image* Tab::GetTabIcon() const
    {
        return StandardButton::GetButtonIconObject();
    }

    void Tab::SetTabTitleObject(ssGUI::Text* title)
    {
        StandardButton::SetButtonTextObject(title);
    }

    ssGUI::Text* Tab::GetTabTitleObject() const
    {
        return StandardButton::GetButtonTextObject();
    }

    ssGUI::Enums::GUIObjectType Tab::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::TAB | Button::GetType();
    }

    Tab* Tab::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        Tab* temp = new Tab(*this);
        CloneExtensionsAndEventCallbacks(temp);

        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}