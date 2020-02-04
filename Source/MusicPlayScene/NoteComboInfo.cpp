#include <fmt/format.h>

#include "Platform/Application.h"
#include "Engine/AssetModule.h"
#include "Game/UISpriteRendererComponent.h"
#include "Game/UITextRendererComponent.h"

#include "NoteComboInfo.h"

void NoteComboInfo::Initialize()
{
    Super::Initialize();
    
    this->InitializePosition();
    this->InitializeLine();
    this->InitializeCurrentComboText();
    this->InitializeMaxComboText();
    this->InitializePerfectText();
    this->InitializeGreatText();
    this->InitializeGoodText();
    this->InitializeMissText();
    
    this->RefreshText();
}

void NoteComboInfo::InitializePosition()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    owner->GetTransform()->SetLocalPosition({clientSize.width * 0.5f - 105.0f, clientSize.height * 0.5f - 58.0f, 0.0f});
}

void NoteComboInfo::InitializeLine()
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

void NoteComboInfo::OnHitNote(NoteTiming noteTiming)
{
    if (noteTiming == NoteTiming::Miss)
    {
        m_currCombo = 0;
        ++m_missCount;
        this->RefreshCurrentComboText();
        this->RefreshMissText();
        return;
    }
    else if (noteTiming == NoteTiming::Perfect)
    {
        ++m_perfectCount;
        this->RefreshPerfectText();
    }
    else if (noteTiming == NoteTiming::Great)
    {
        ++m_greatCount;
        this->RefreshGreatText();
    }
    else if (noteTiming == NoteTiming::Good)
    {
        ++m_goodCount;
        this->RefreshGoodText();
    }

    ++m_currCombo;
    m_maxCombo = std::max(m_maxCombo, m_currCombo);
    this->RefreshCurrentComboText();
    this->RefreshMaxComboText();
}

void NoteComboInfo::OnMissNote()
{
    this->OnHitNote(NoteTiming::Miss);
}

void NoteComboInfo::Reset()
{
    m_currCombo = 0;
    m_maxCombo = 0;
    m_perfectCount = 0;
    m_greatCount = 0;
    m_goodCount = 0;
    m_missCount = 0;
    
    this->RefreshText();
}

void NoteComboInfo::InitializeCurrentComboText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto currComboText = tgon::GameObject::Create();
    
    auto textComponent = currComboText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothic.otf");
    textComponent->SetFontSize(27);
    textComponent->SetRect(tgon::I32Rect(-500, 34, 482, 30));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerRight);
    
    owner->AddChild(currComboText);
    
    m_currComboTextComponent = textComponent;
}

void NoteComboInfo::InitializeMaxComboText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto maxComboText = tgon::GameObject::Create();
    
    auto textComponent = maxComboText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothic.otf");
    textComponent->SetFontSize(20);
    textComponent->SetRect(tgon::I32Rect(-500, -4, 482, 30));
    textComponent->SetTextAlignment(tgon::TextAlignment::UpperRight);
    
    owner->AddChild(maxComboText);
    
    m_maxComboTextComponent = textComponent;
}

void NoteComboInfo::InitializePerfectText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto perfectText = tgon::GameObject::Create();
    auto textComponent = perfectText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(12);
    textComponent->SetRect(tgon::I32Rect(18, 31, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({240.0f / 255.0f, 37.0f / 255.0f, 37.0f / 255.0f, 1.0f});

    owner->AddChild(perfectText);

    m_perfectTextComponent = textComponent;
}

void NoteComboInfo::InitializeGreatText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto greatText = tgon::GameObject::Create();

    auto textComponent = greatText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(12);
    textComponent->SetRect(tgon::I32Rect(18, 17, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({238.0f / 255.0f, 167.0f / 255.0f, 13.0f / 255.0f, 1.0f});

    owner->AddChild(greatText);

    m_greatTextComponent = textComponent;
}

void NoteComboInfo::InitializeGoodText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto goodText = tgon::GameObject::Create();

    auto textComponent = goodText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(12);
    textComponent->SetRect(tgon::I32Rect(18, 3, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({238.0f / 255.0f, 167.0f / 255.0f, 13.0f / 255.0f, 1.0f});

    owner->AddChild(goodText);

    m_goodTextComponent = textComponent;
}

void NoteComboInfo::InitializeMissText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto missText = tgon::GameObject::Create();

    auto textComponent = missText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(12);
    textComponent->SetRect(tgon::I32Rect(18, -11, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({127.0f / 255.0f, 124.0f / 255.0f, 125.0f / 255.0f, 1.0f});

    owner->AddChild(missText);

    m_missTextComponent = textComponent;
}

void NoteComboInfo::RefreshText()
{
    this->RefreshCurrentComboText();
    this->RefreshMaxComboText();
    this->RefreshPerfectText();
    this->RefreshGreatText();
    this->RefreshGoodText();
    this->RefreshMissText();
}

void NoteComboInfo::RefreshCurrentComboText()
{
    m_currComboTextComponent->SetText(fmt::format("{0} Combo", m_currCombo));
}

void NoteComboInfo::RefreshMaxComboText()
{
    m_maxComboTextComponent->SetText(fmt::format("{0} Max", m_maxCombo));
}

void NoteComboInfo::RefreshPerfectText()
{
    m_perfectTextComponent->SetText(fmt::format("{0} Perfect", m_perfectCount));
}

void NoteComboInfo::RefreshGreatText()
{
    m_greatTextComponent->SetText(fmt::format("{0} Great", m_greatCount));
}

void NoteComboInfo::RefreshGoodText()
{
    m_goodTextComponent->SetText(fmt::format("{0} Good", m_goodCount));
}

void NoteComboInfo::RefreshMissText()
{
    m_missTextComponent->SetText(fmt::format("{0} Miss", m_missCount));
}
