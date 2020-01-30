#include <fmt/format.h>

#include "Platform/Application.h"
#include "Engine/AssetModule.h"
#include "Game/UISpriteRendererComponent.h"
#include "Game/UITextRendererComponent.h"

#include "NoteHitInfo.h"

void NoteHitInfo::Initialize()
{
    Super::Initialize();
    
    this->InitializeLine();
    this->InitializeCurrentComboText();
    this->InitializeMaxComboText();
    
    this->RefreshComboText();
}

void NoteHitInfo::InitializeLine()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteComponent = owner->AddComponent<tgon::UISpriteRendererComponent>();
    spriteComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicPlayScene/line2.png"));
}

void NoteHitInfo::OnHitNote()
{
    ++m_currCombo;
    m_maxCombo = std::max(m_maxCombo, m_currCombo);
    this->RefreshComboText();
}

void NoteHitInfo::OnMissNote()
{
    m_currCombo = 0;
    this->RefreshComboText();
}

void NoteHitInfo::InitializeCurrentComboText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto currComboText = tgon::GameObject::Create();
    currComboText->GetTransform()->SetLocalPosition({0.0f, 15.0f, 0.0f});
    
    auto textComponent = currComboText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothic.otf");
    textComponent->SetFontSize(28);
    textComponent->SetRect(tgon::I32Rect(-500, -50, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleCenter);
    textComponent->SetSortingLayer(3);
    
    owner->AddChild(currComboText);
    
    m_currComboTextRendererComponent = textComponent;
}

void NoteHitInfo::InitializeMaxComboText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto maxComboText = tgon::GameObject::Create();
    maxComboText->GetTransform()->SetLocalPosition({0.0f, -15.0f, 0.0f});
    
    auto textComponent = maxComboText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothic.otf");
    textComponent->SetFontSize(22);
    textComponent->SetRect(tgon::I32Rect(-500, -50, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleCenter);
    textComponent->SetSortingLayer(3);
    
    owner->AddChild(maxComboText);
    
    m_maxComboTextRendererComponent = textComponent;
}

void NoteHitInfo::RefreshComboText()
{
    m_currComboTextRendererComponent->SetText(fmt::format("{0} Combo", m_currCombo));
    m_maxComboTextRendererComponent->SetText(fmt::format("{0} Max", m_maxCombo));
}
