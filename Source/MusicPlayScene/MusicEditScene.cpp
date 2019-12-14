#include "PrecompiledHeader.h"

#include "TGON.h"
#include "MusicEditScene.h"
#include "Note.h"
#include "NoteLine.h"

MusicEditScene::MusicEditScene(const MusicInfo& musicInfo) :
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_audioModule(tgon::Application::GetEngine()->FindModule<tgon::AudioModule>()),
    m_keyboard(tgon::Application::GetEngine()->FindModule<tgon::InputModule>()->GetKeyboard()),
    m_musicInfo(musicInfo)
{
}

void MusicEditScene::Initialize()
{
    Super::Initialize();
    
    this->InitializeBackgroundObject();
    this->InitializeNoteLine();
    this->InitializeNoteObjectPool();
    this->InitializeHoldNoteObjectPool();
    this->InitializeMusicNameObject();
    this->InitializeMusicArtistNameObject();
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_audioPlayer.Initialize(assetModule->GetAudioBuffer(m_musicInfo.musicPath));
    
    auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
    timerModule->SetTimer([this](tgon::TimerHandle timerHandle)
    {
        m_audioPlayer.Play(1.0f, false);
        m_isMusicWaiting = false;
    }, 3.0f, false);
    timerModule->SetTimer([this](tgon::TimerHandle timerHandle)
    {
        m_elapsedTime = m_audioPlayer.GetProgressInSeconds();
    }, 0.5f, true);
}

void MusicEditScene::Update()
{
    Super::Update();
    
    this->UpdateBackgroundObjectPosition();
    
    m_elapsedTime += m_timeModule->GetTickTime();
    
    if (m_isMusicWaiting)
    {
        return;
    }
    
    this->UpdateNotes();
    this->OnHandleInput();
}

void MusicEditScene::OnHandleInput()
{
    if (m_keyboard->IsKeyHold(tgon::KeyCode::LeftArrow))
    {
        float newProgress = m_audioPlayer.GetProgressInSeconds() - 0.5f;
        m_audioPlayer.SetProgressInSeconds(newProgress);
        m_elapsedTime = newProgress;

        this->RefreshNotes();
    }
    else if (m_keyboard->IsKeyHold(tgon::KeyCode::RightArrow))
    {
        float newProgress = m_audioPlayer.GetProgressInSeconds() + 0.5f;
        m_audioPlayer.SetProgressInSeconds(newProgress);
        m_elapsedTime = newProgress;

        this->RefreshNotes();
    }
    else if (m_keyboard->IsKeyDown(tgon::KeyCode::F1))
    {
        for (auto& noteInfo : m_musicInfo.noteInfos)
        {
            const char* noteIndexStr[5] = {":10000", ":01000", ":00100", ":00010", ":00001"};

            double bitPerSeconds = m_musicInfo.bpm / 60.0f;
            auto clap = 8.0f * bitPerSeconds * (noteInfo.hitTime - m_musicInfo.sync);
            tgon::Debug::WriteLine(std::to_string(int(clap)) + noteIndexStr[noteInfo.noteIndex]);
        }
    }
    else
    {
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
        
        for (size_t noteLineIndex = 0; noteLineIndex < 5; ++noteLineIndex)
        {
            for (size_t i = keyCodeIndexTable[noteLineIndex].first; i <= keyCodeIndexTable[noteLineIndex].second; ++i)
            {
                if (m_keyboard->IsKeyDown(keyCodeTable[i]))
                {
                    m_musicInfo.noteInfos.insert(m_musicInfo.noteInfos.begin() + m_noteInfoIndex, NoteInfo{static_cast<int32_t>(noteLineIndex), m_elapsedTime, 0.0f});
                    break;
                }
            }
        }
    }
}

void MusicEditScene::UpdateNotes()
{
    for (size_t i = m_noteInfoIndex; i < m_musicInfo.noteInfos.size(); ++i)
    {
        auto& noteInfo = m_musicInfo.noteInfos[i];
        if (noteInfo.hitTime - m_elapsedTime < 1.0f)
        {
            std::shared_ptr<Note> noteObject;
            if (noteInfo.holdTime == 0.0f)
            {
                noteObject = this->GetNoteObjectFromPool();
            }
            else
            {
                auto holdNoteObject = this->GetHoldNoteObjectFromPool();
                holdNoteObject->SetHoldTime(noteInfo.holdTime);
                noteObject = holdNoteObject;
            }
            noteObject->SetHitTime(noteInfo.hitTime);
            noteObject->SetNoteLineIndex(noteInfo.noteIndex);

            m_notes[noteInfo.noteIndex].push_back(std::move(noteObject));
            ++m_noteInfoIndex;
        }
        else
        {
            break;
        }
    }

    for (auto& noteObjects : m_notes)
    {
        for (auto iter = noteObjects.begin(); iter != noteObjects.end();)
        {
            if ((*iter)->GetHitTime() - m_elapsedTime < -0.5f)
            {
                bool isNormalNote = (*iter)->GetRTTI() != tgon::GetRTTI<HoldNote*>();
                if (isNormalNote)
                {
                    m_noteObjectPool.push_back(*iter);
                    iter = noteObjects.erase(iter);
                    continue;
                }
                else if ((*iter)->IsHolding() == false)
                {
                    m_holdNoteObjectPool.push_back(std::static_pointer_cast<HoldNote>(*iter));
                    iter = noteObjects.erase(iter);
                    continue;
                }
            }
        
            (*iter)->SetElapsedTime(m_elapsedTime);
            (*iter)->Update();
            ++iter;
        }
    }
}

void MusicEditScene::UpdateBackgroundObjectPosition()
{
    auto backgroundObjectPos = m_backgroundObject->GetTransform()->GetLocalPosition();
    backgroundObjectPos.x -= 3.0f * m_timeModule->GetTickTime();
    m_backgroundObject->GetTransform()->SetLocalPosition(backgroundObjectPos);
}

void MusicEditScene::InitializeBackgroundObject()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto backgroundObject = tgon::GameObject::Create();
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    backgroundObject->GetTransform()->SetLocalPosition({-static_cast<float>(clientSize.width) * 0.5f, 0.0f, 0.0f});
    
    auto spriteRendererComponent = backgroundObject->AddComponent<tgon::SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture("Resource/Background/MusicPlayScene/green.png"));
    spriteRendererComponent->SetPivot({0.0f, 0.5f});
    
    this->AddObject(backgroundObject);
    
    m_backgroundObject = std::move(backgroundObject);
}

void MusicEditScene::InitializeNoteLine()
{
    auto noteLine = tgon::GameObject::Create<NoteLine>();
    noteLine->GetTransform()->SetLocalPosition(tgon::Vector3(-10.0f, -40.0f, 0.0f));
    this->AddObject(noteLine);
    
    m_noteLine = std::move(noteLine);
}

void MusicEditScene::InitializeNoteObjectPool()
{
    for (size_t i = 0; i < 40; ++i)
    {
        m_noteObjectPool.push_back(tgon::GameObject::Create<Note>({}, std::make_shared<tgon::Transform>(), m_noteLine));
    }
}

void MusicEditScene::InitializeHoldNoteObjectPool()
{
    for (size_t i = 0; i < 20; ++i)
    {
        m_holdNoteObjectPool.push_back(tgon::GameObject::Create<HoldNote>({}, std::make_shared<tgon::Transform>(), m_noteLine));
    }
}

void MusicEditScene::InitializeMusicNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 33.0f, windowSize.height / 2 - 24.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(45);
    textComponent->SetRect(tgon::I32Rect(0, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetSortingLayer(4);
    textComponent->SetText(m_musicInfo.musicName);

    m_musicNameRendererComponent = textComponent;

    this->AddObject(object);
}

void MusicEditScene::InitializeMusicArtistNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicArtistName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 33.0f, windowSize.height / 2 - 82.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(18);
    textComponent->SetRect(tgon::I32Rect(-0, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::UpperLeft);
    textComponent->SetSortingLayer(4);
    textComponent->SetText(m_musicInfo.musicAuthorName);

    m_musicArtistNameRendererComponent = textComponent;

    this->AddObject(object);
}

void MusicEditScene::RefreshNotes()
{
    while (true)
    {
        if (m_noteInfoIndex <= 0)
        {
            break;
        }

        auto& noteInfo = m_musicInfo.noteInfos[std::min(m_noteInfoIndex, m_musicInfo.noteInfos.size() - 1)];
        if (noteInfo.hitTime < m_elapsedTime)
        {
            break;
        }

        --m_noteInfoIndex;
    }

    for (auto& noteObjects : m_notes)
    {
        while (noteObjects.empty() == false)
        {
            auto noteObject = noteObjects.back();
            if (noteObject->GetRTTI() != tgon::GetRTTI<HoldNote*>())
            {
                m_noteObjectPool.push_back(noteObject);
            }
            else
            {
                m_holdNoteObjectPool.push_back(std::static_pointer_cast<HoldNote>(noteObject));
            }

            noteObjects.pop_back();
        }
    }

    this->UpdateNotes();
}

std::shared_ptr<Note> MusicEditScene::GetNoteObjectFromPool()
{
    if (m_noteObjectPool.size() == 0)
    {
        m_noteObjectPool.push_back(tgon::GameObject::Create<Note>({}, std::make_shared<tgon::Transform>(), m_noteLine));
    }
    
    auto ret = m_noteObjectPool.back();
    ret->Reset();
    
    m_noteObjectPool.pop_back();
    
    return ret;
}

std::shared_ptr<HoldNote> MusicEditScene::GetHoldNoteObjectFromPool()
{
    if (m_holdNoteObjectPool.size() == 0)
    {
        m_holdNoteObjectPool.push_back(tgon::GameObject::Create<HoldNote>({}, std::make_shared<tgon::Transform>(), m_noteLine));
    }
    
    auto ret = m_holdNoteObjectPool.back();
    ret->Reset();
    
    m_holdNoteObjectPool.pop_back();
    
    return ret;
}