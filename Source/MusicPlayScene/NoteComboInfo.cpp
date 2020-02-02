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
    this->InitializeEarlyText();
    this->InitializeLazyText();
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
        ++m_perfectCount;
        this->RefreshPerfectText();
    }
    else if (noteTiming == NoteTiming::Early)
    {
        ++m_earlyCount;
        this->RefreshEarlyText();
    }
    else if (noteTiming == NoteTiming::Late)
    {
        ++m_lazyCount;
        this->RefreshLazyText();
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
    textComponent->SetSortingLayer(3);
    
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
    textComponent->SetSortingLayer(3);
    
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
    textComponent->SetRect(tgon::I32Rect(18, 30, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetSortingLayer(3);
    textComponent->SetBlendColor({240.0f / 255.0f, 37.0f / 255.0f, 37.0f / 255.0f, 1.0f});

    owner->AddChild(perfectText);

    m_perfectTextComponent = textComponent;
}

void NoteComboInfo::InitializeEarlyText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto earlyText = tgon::GameObject::Create();

    auto textComponent = earlyText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(12);
    textComponent->SetRect(tgon::I32Rect(18, 14, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetSortingLayer(3);
    textComponent->SetBlendColor({238.0f / 255.0f, 167.0f / 255.0f, 13.0f / 255.0f, 1.0f});

    owner->AddChild(earlyText);

    m_earlyTextComponent = textComponent;
}

void NoteComboInfo::InitializeLazyText()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto lazyText = tgon::GameObject::Create();

    auto textComponent = lazyText->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(12);
    textComponent->SetRect(tgon::I32Rect(18, 1, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetSortingLayer(3);
    textComponent->SetBlendColor({238.0f / 255.0f, 167.0f / 255.0f, 13.0f / 255.0f, 1.0f});

    owner->AddChild(lazyText);

    m_lazyTextComponent = textComponent;
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
    textComponent->SetRect(tgon::I32Rect(18, -9, 500, 15));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetSortingLayer(3);
    textComponent->SetBlendColor({127.0f / 255.0f, 124.0f / 255.0f, 125.0f / 255.0f, 1.0f});

    owner->AddChild(missText);

    m_missTextComponent = textComponent;
}

void NoteComboInfo::RefreshText()
{
    this->RefreshCurrentComboText();
    this->RefreshMaxComboText();
    this->RefreshPerfectText();
    this->RefreshEarlyText();
    this->RefreshLazyText();
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

void NoteComboInfo::RefreshEarlyText()
{
    m_earlyTextComponent->SetText(fmt::format("{0} Early", m_earlyCount));
}

void NoteComboInfo::RefreshLazyText()
{
    m_lazyTextComponent->SetText(fmt::format("{0} Lazy", m_lazyCount));
}

void NoteComboInfo::RefreshMissText()
{
    m_missTextComponent->SetText(fmt::format("{0} Miss", m_missCount));
}
