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

void Note::InitializeSprite()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_noteRendererComponent = this->AddComponent<tgon::SpriteRendererComponent>();
    m_noteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/PlayScene/Note.png"));
}

void Note::Update()
{
    Super::Update();

    if (m_elapsedTime - m_hitTime > 0.0f)
    {
        auto blendColor = m_noteRendererComponent->GetBlendColor();
        blendColor.a = std::max(0.0f, blendColor.a - 5.0f * m_timeModule->GetTickTime());
        m_noteRendererComponent->SetBlendColor(blendColor);
    }
    
    m_transform->SetLocalPosition(tgon::Lerp(g_noteStartPosArray[m_noteLineIndex], g_noteEndPosArray[m_noteLineIndex], 1.0f + m_elapsedTime - m_hitTime));
}

NoteTiming Note::CheckNoteTiming() const noexcept
{
    auto distance = m_elapsedTime - m_hitTime;
    if (-0.1f < distance && distance < 0.1f)
    {
        return NoteTiming::Perfect;
    }
    else if (-0.13f < distance && distance < 0.13f)
    {
        return NoteTiming::Great;
    }
    else if (-0.2f < distance && distance < 0.2f)
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

    auto onKeyDown = [&]()
    {
        static auto hitSoundPlayer = [&]()
        {
            auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
            tgon::AudioPlayer hitSoundPlayer;
            hitSoundPlayer.Initialize(assetModule->GetAudioBuffer("Resource/Sound/HOCKEY.wav"));
            return std::move(hitSoundPlayer);
        } ();
        hitSoundPlayer.Play();
        
        m_isHitted = true;


        auto noteTiming = this->CheckNoteTiming();
        if (noteTiming == NoteTiming::Perfect)
        {
            tgon::Debug::WriteLine("Perfect");
        }
        else if (noteTiming == NoteTiming::Great)
        {
            tgon::Debug::WriteLine("Great");
        }
        else if (noteTiming == NoteTiming::Early)
        {
            tgon::Debug::WriteLine("Early");
        }
        else if (noteTiming == NoteTiming::Late)
        {
            tgon::Debug::WriteLine("Late");
        }
    };
    
    switch (m_noteLineIndex)
    {
    case 0:
        if (m_keyboard->IsKeyDown(tgon::KeyCode::Q) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::W) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::E) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::R) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::T) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::Y) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::U) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::I) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::O) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::P) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::LeftBracket) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::RightBracket))
        {
            onKeyDown();
        }
        break;

    case 1:
        if (m_keyboard->IsKeyDown(tgon::KeyCode::A) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::S) || 
            m_keyboard->IsKeyDown(tgon::KeyCode::D) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::F) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::G) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::H) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::J) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::K) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::L) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::Semicolon) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::Apostrophe))
        {
            onKeyDown();
        }
        break;

    case 2:
        if (m_keyboard->IsKeyDown(tgon::KeyCode::Z) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::X) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::C) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::V) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::B) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::N) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::M) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::Comma) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::Period) ||
            m_keyboard->IsKeyDown(tgon::KeyCode::Slash))
        {
            onKeyDown();
        }
        break;

    case 3:
        if (m_keyboard->IsKeyDown(tgon::KeyCode::Space))
        {
            onKeyDown();
        }
        break;
    }
}

bool Note::CheckCanHit() const
{
    auto distance = m_elapsedTime - m_hitTime;
    return -0.2f < distance && distance < 0.2f;
}

void LongNote::Initialize()
{
    Super::Initialize();

    this->InitializeSprite();
}

void LongNote::Update()
{
    Super::Update();
}

bool LongNote::CheckCanHit() const
{
    return Super::CheckCanHit();
}

void LongNote::SetHoldTime(float holdTime) noexcept
{
    m_holdTime = holdTime;
}

float LongNote::GetHoldTime() const noexcept
{
    return m_holdTime;
}

void LongNote::InitializeSprite()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_longNoteRendererComponent = this->AddComponent<tgon::SpriteRendererComponent>();
    m_longNoteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/PlayScene/Note.png"));
}
