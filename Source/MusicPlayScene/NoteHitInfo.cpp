#include <fmt/format.h>

#include "TGON.h"
#include "NoteHitInfo.h"

void NoteHitInfo::Initialize()
{
    Super::Initialize();
    
    this->InitializeSprite();
    this->InitializeCurrentComboTextObject();
    this->InitializeMaxComboTextObject();
    
    this->RefreshComboText();
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

void NoteHitInfo::InitializeSprite()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }
    
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();
    gameObject->GetTransform()->SetLocalPosition(tgon::Vector3(windowSize.width / 2 - 35.0f, windowSize.height / 2 - 45.0f, 0.0f));

    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteComponent = gameObject->AddComponent<tgon::SpriteRendererComponent>();
    spriteComponent->SetTexture(assetModule->GetResource<tgon::Texture>("Resource/UI/MusicPlayScene/noteHitInfo.png"));
    spriteComponent->SetSortingLayer(3);
    spriteComponent->SetPivot({1.0f, 0.0f});
}

void NoteHitInfo::InitializeCurrentComboTextObject()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }
    
    auto textComponent = gameObject->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/malgun.ttf");
    textComponent->SetFontSize(40);
    textComponent->SetRect(tgon::I32Rect(-500, 28, 403, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerRight);
    textComponent->SetSortingLayer(3);
    
    m_currComboTextRendererComponent = textComponent;
}

void NoteHitInfo::InitializeMaxComboTextObject()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }
    
    auto textComponent = gameObject->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/malgun.ttf");
    textComponent->SetFontSize(24);
    textComponent->SetRect(tgon::I32Rect(-500, 0, 451, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerRight);
    textComponent->SetSortingLayer(3);
    
    m_maxComboTextRendererComponent = textComponent;
}

void NoteHitInfo::RefreshComboText()
{
    m_currComboTextRendererComponent->SetText(fmt::format("{0}", m_currCombo));
    m_maxComboTextRendererComponent->SetText(fmt::format("{0}", m_maxCombo));
}
