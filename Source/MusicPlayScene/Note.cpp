#include "TGON.h"
#include "Note.h"

namespace
{

constexpr tgon::Vector3 g_noteStartPosArray[] = {
    tgon::Vector3(465.301727f, -24.2000046f, 0.0f),
    tgon::Vector3(487.301849f, -64.8999634f, 0.0f),
    tgon::Vector3(508.201965f, -104.499908f, 0.0f),
    tgon::Vector3(529.102081f, -145.199966f, 0.0f),
};

constexpr tgon::Vector3 g_noteEndPosArray[] = {
    tgon::Vector3(-269.500641f, -24.2000046f, 0.0f),
    tgon::Vector3(-247.500519f, -64.8999634f, 0.0f),
    tgon::Vector3(-226.600403f, -104.499908f, 0.0f),
    tgon::Vector3(-205.700287f, -145.199966f, 0.0f),
};

} /* namespace */

Note::Note() :
    GameObject(),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_keyboard(tgon::Application::GetEngine()->FindModule<tgon::InputModule>()->GetKeyboard())
{
}

void Note::Reset()
{
    m_isHitted = false;
    m_elapsedTime = 0.0f;
    m_hitTime = 0.0f;
    m_noteLineIndex = 0;
    m_noteRendererComponent->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
}

void Note::Initialize()
{
    Super::Initialize();
    this->InitializeSprite();
}

void Note::SetNoteLineIndex(int32_t index) noexcept
{
    m_noteLineIndex = index;
    m_transform->SetLocalPosition(g_noteStartPosArray[index]);
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

bool Note::IsHolding() const noexcept
{
    return m_isHolding;
}

void Note::InitializeSprite()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_noteRendererComponent = this->AddComponent<tgon::SpriteRendererComponent>();
    m_noteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/MusicPlayScene/note.png"));
}

void Note::Update()
{
    Super::Update();

    if (m_isHolding == false)
    {
        if (m_elapsedTime - m_hitTime > 0.0f)
        {
            auto blendColor = m_noteRendererComponent->GetBlendColor();
            blendColor.a = std::clamp(tgon::Lerp(1.0f, 0.0f, (m_hitTime - m_elapsedTime) * 3.0f), 0.0f, 1.0f);
            m_noteRendererComponent->SetBlendColor(blendColor);
        }
        
        m_transform->SetLocalPosition(tgon::Lerp(g_noteStartPosArray[m_noteLineIndex], g_noteEndPosArray[m_noteLineIndex], 1.0f + m_elapsedTime - m_hitTime));
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
        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        tgon::AudioPlayer hitSoundPlayer;
        hitSoundPlayer.Initialize(assetModule->GetAudioBuffer("Resource/Sound/HOCKEY.wav"));
        return hitSoundPlayer;
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
    m_holdNoteRendererComponent->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_ringObject->GetTransform()->SetLocalScale(tgon::Vector3(1.0f, 1.0f, 1.0f));
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
        float interpolated = std::min(1.0f, 1.0f - (0.5f * ((m_elapsedTime - m_hitTime) / m_holdTime)));
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
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();

    auto ringObject = tgon::GameObject::Create();
    ringObject->GetTransform()->SetParent(this->GetTransform());
    m_holdNoteRendererComponent = ringObject->AddComponent<tgon::SpriteRendererComponent>();
    m_holdNoteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/MusicPlayScene/holdNote.png"));
    
    m_ringObject = ringObject;
}
