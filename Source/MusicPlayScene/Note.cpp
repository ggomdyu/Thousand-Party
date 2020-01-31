#include <fmt/format.h>

#include "Math/Mathematics.h"
#include "Platform/Application.h"
#include "Game/UISpriteRendererComponent.h"
#include "Engine/InputModule.h"
#include "Engine/AssetModule.h"
#include "Engine/TimeModule.h"
#include "Audio/AudioPlayer.h"
#include "Diagnostics/Debug.h"

#include "Note.h"
#include "NoteLineUI.h"

#if _DEBUG
constexpr bool g_needToHitAutomatically = false;
#else
constexpr bool g_needToHitAutomatically = false;
#endif

Note::Note(const std::shared_ptr<NoteLineUI>& noteLine) :
    Component(),
    m_hittedKeyCode(tgon::KeyCode(0)),
    m_noteLine(noteLine),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_keyboard(tgon::Application::GetEngine()->FindModule<tgon::InputModule>()->GetKeyboard())
{
}

void Note::Reset()
{
    m_autoHitted = false;
    m_isHitted = false;
    m_elapsedTime = 0.0f;
    m_hitTime = 0.0f;
    m_noteLineIndex = 0;
    m_noteRendererComponent->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
}

void Note::Initialize()
{
    Super::Initialize();
    
    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    auto gameObject = weakGameObject.lock();

    m_transform = gameObject->GetTransform();
    
    this->InitializeSprite();
}

void Note::SetNoteLineIndex(int32_t index) noexcept
{
    m_noteLineIndex = index;

    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    weakGameObject.lock()->GetTransform()->SetLocalPosition(m_noteLine->GetNoteStartPosition(index));
}

void Note::SetElapsedTime(float elapsedTime) noexcept
{
    m_elapsedTime = elapsedTime;
}

void Note::SetHitTime(float hitTime) noexcept
{
    m_hitTime = hitTime;
}

float Note::GetHitTime() const noexcept
{
    return m_hitTime;
}

bool Note::IsHitted() const noexcept
{
    return m_isHitted;
}

NoteTiming Note::GetHittedTiming() const noexcept
{
    return m_hittedTiming;
}

bool Note::IsHolding() const noexcept
{
    return m_isHolding;
}

void Note::InitializeSprite()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_noteRendererComponent = owner->AddComponent<tgon::UISpriteRendererComponent>();
    m_noteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Object/MusicPlayScene/note.png"));
}

void Note::Update()
{
    Super::Update();

    if (m_isHolding == false)
    {
        if (m_elapsedTime - m_hitTime > 0.0f)
        {
            auto blendColor = m_noteRendererComponent->GetBlendColor();
            blendColor.a = tgon::Lerp(1.0f, 0.0f, (m_elapsedTime - m_hitTime) * 5.0f);
            m_noteRendererComponent->SetBlendColor(blendColor);
        }
        
        m_transform->SetLocalPosition(tgon::Lerp(m_noteLine->GetNoteStartPosition(m_noteLineIndex), m_noteLine->GetNoteHitPosition(m_noteLineIndex), 1.0f + m_elapsedTime - m_hitTime));
    }

    if constexpr (g_needToHitAutomatically)
    {
        if (m_elapsedTime - m_hitTime > 0.0f)
        {
            if (m_autoHitted == false)
            {
                this->PlayHitSound();
                m_autoHitted = true;
            }
        }
    }
}

NoteTiming Note::CheckNoteTiming(float timingOffset) const noexcept
{
    auto distance = m_elapsedTime - (m_hitTime + timingOffset);
    if (-0.05f < distance && distance < 0.05f)
    {
        return NoteTiming::Perfect;
    }
    else if (-0.1f < distance && distance < 0.1f)
    {
        return NoteTiming::Great;
    }
    else if (-0.15f < distance && distance < 0.15f)
    {
        if (distance < 0)
        {
            return NoteTiming::Early;
        }
        else
        {
            return NoteTiming::Late;
        }
    }
    
    return NoteTiming::Miss;
}

void Note::UpdateInput()
{
    if (this->CheckCanHit() == false)
    {
        return;
    }

    constexpr tgon::KeyCode keyCodeTable[] = {
        tgon::KeyCode::Alpha1, tgon::KeyCode::Alpha2, tgon::KeyCode::Alpha3, tgon::KeyCode::Alpha4, tgon::KeyCode::Alpha5, tgon::KeyCode::Alpha6, tgon::KeyCode::Alpha7, tgon::KeyCode::Alpha8, tgon::KeyCode::Alpha9, tgon::KeyCode::Alpha0, tgon::KeyCode::Minus, tgon::KeyCode::Plus,
        tgon::KeyCode::Q, tgon::KeyCode::W, tgon::KeyCode::E, tgon::KeyCode::R, tgon::KeyCode::T, tgon::KeyCode::Y, tgon::KeyCode::U, tgon::KeyCode::I, tgon::KeyCode::O, tgon::KeyCode::P, tgon::KeyCode::LeftBracket, tgon::KeyCode::RightBracket,
        tgon::KeyCode::A, tgon::KeyCode::S, tgon::KeyCode::D, tgon::KeyCode::F, tgon::KeyCode::G, tgon::KeyCode::H, tgon::KeyCode::J, tgon::KeyCode::K, tgon::KeyCode::L, tgon::KeyCode::Semicolon, tgon::KeyCode::Apostrophe,
        tgon::KeyCode::Z, tgon::KeyCode::X, tgon::KeyCode::C, tgon::KeyCode::V, tgon::KeyCode::B, tgon::KeyCode::N, tgon::KeyCode::M, tgon::KeyCode::Comma, tgon::KeyCode::Period, tgon::KeyCode::Slash,
        tgon::KeyCode::Space
    };
    constexpr std::array<std::pair<size_t, size_t>, 5> keyCodeIndexTable = {std::pair<size_t, size_t>
        {0, 11}, {12, 23}, {24, 34}, {35, 44}, {45,45}
    };
    
    if (m_isHolding == false)
    {
        for (size_t i = keyCodeIndexTable[m_noteLineIndex].first; i <= keyCodeIndexTable[m_noteLineIndex].second; ++i)
        {
            if (m_keyboard->IsKeyDown(keyCodeTable[i]))
            {
                this->OnHitNote(keyCodeTable[i], this->CheckNoteTiming());
                break;
            }
        }
    }
}

bool Note::CheckCanHit() const
{
    auto distance = m_elapsedTime - m_hitTime;
    return -0.15f < distance && distance < 0.15f;
}

void Note::PlayHitSound()
{
    auto audioPlayerGenerator = [&]()
    {
        auto audioPlayer = tgon::AudioPlayer::Create();

        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        audioPlayer->SetAudioBuffer(assetModule->GetResource<tgon::AudioBuffer>("Resource/Sound/HOCKEY.wav"));

        return std::move(*audioPlayer);
    };
    static tgon::AudioPlayer hitSoundPlayer[5] = {
        audioPlayerGenerator(),
        audioPlayerGenerator(),
        audioPlayerGenerator(),
        audioPlayerGenerator(),
        audioPlayerGenerator(),
    };
    
    hitSoundPlayer[m_noteLineIndex].Play();
}

void Note::OnHitNote(tgon::KeyCode keyCode, NoteTiming noteTiming)
{
    this->PlayHitSound();
    
    m_isHitted = true;
    m_hittedKeyCode = keyCode;
    m_hittedTiming = noteTiming;
    
    if (noteTiming == NoteTiming::Perfect)
    {
        tgon::Debug::Write("Perfect:");
    }
    else if (noteTiming == NoteTiming::Great)
    {
        tgon::Debug::Write("Great:  ");
    }
    else if (noteTiming == NoteTiming::Early)
    {
        tgon::Debug::Write("Early:  ");
    }
    else if (noteTiming == NoteTiming::Late)
    {
        tgon::Debug::Write("Late:   ");
    }
    else if (noteTiming == NoteTiming::Miss)
    {
        tgon::Debug::Write("Miss:   ");
    }
    
    tgon::Debug::WriteLine(std::to_string(m_elapsedTime));
}

void HoldNote::Reset()
{
    Super::Reset();
    
    m_holdTime = 0.0f;
    m_holdNoteRendererComponent->SetBlendColor({1.0f, 1.0f, 1.0f, 1.0f});
    m_ringObject->GetTransform()->SetLocalScale({1.0f, 1.0f, 1.0f});
    m_autoHitted = false;
}

void HoldNote::Initialize()
{
    Super::Initialize();
    
    this->InitializeSprite();
}

void HoldNote::Update()
{
    Super::Update();
    
    if (m_isHolding == false)
    {
        if (m_elapsedTime - m_hitTime > 0.0f)
        {
            m_holdNoteRendererComponent->SetBlendColor(m_noteRendererComponent->GetBlendColor());
        }
    }
    else
    {
        float interpolated = std::clamp(std::min(1.0f, 1.0f - (0.5f * ((m_elapsedTime - m_hitTime) / m_holdTime))), 0.0f, 1.0f);
        m_ringObject->GetTransform()->SetLocalScale(tgon::Vector3(interpolated, interpolated, 1.0f));
    }
}

void HoldNote::UpdateInput()
{
    if (this->CheckCanHit() == false)
    {
        return;
    }
    
    Super::UpdateInput();
    
    if (m_isHitted)
    {
        if (m_keyboard->IsKeyUp(m_hittedKeyCode))
        {
            this->OnHitNote(m_hittedKeyCode, this->CheckNoteTiming(m_holdTime));
        }
        else if (m_keyboard->IsKeyHold(m_hittedKeyCode))
        {
            float holdEndTime = m_hitTime + m_holdTime;
            if (m_elapsedTime - holdEndTime >= 0.0f)
            {
                this->OnHitNote(m_hittedKeyCode, this->CheckNoteTiming(m_holdTime));
            }
        }
    }
}

bool HoldNote::CheckCanHit() const
{
    return m_isHolding || Super::CheckCanHit();
}

void HoldNote::SetHoldTime(float holdTime) noexcept
{
    m_holdTime = holdTime;
}

float HoldNote::GetHoldTime() const noexcept
{
    return m_holdTime;
}

void HoldNote::OnHitNote(tgon::KeyCode keyCode, NoteTiming noteTiming)
{
    Super::OnHitNote(keyCode, noteTiming);
    
    // Hold note key up occured
    if (m_isHolding)
    {
        m_isHolding = false;
    }
    // Hold note key down occured
    else
    {
        m_isHolding = true;
    }
}

void HoldNote::InitializeSprite()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto ringObject = tgon::GameObject::Create();

    owner->AddChild(ringObject);

    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_holdNoteRendererComponent = ringObject->AddComponent<tgon::UISpriteRendererComponent>();
    m_holdNoteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Object/MusicPlayScene/holdNote.png"));
    m_holdNoteRendererComponent->SetSortingLayer(m_noteRendererComponent->GetSortingLayer() + 1);
    
    m_ringObject = ringObject;
}
