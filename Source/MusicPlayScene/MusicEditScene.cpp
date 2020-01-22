#include "Platform/Application.h"
#include "Engine/AssetModule.h"
#include "Engine/AudioModule.h"
#include "Engine/InputModule.h"
#include "Engine/TimeModule.h"
#include "Engine/TimerModule.h"
#include "Component/UISpriteRendererComponent.h"
#include "Component/UITextRendererComponent.h"
#include "Diagnostics/Debug.h"

#include "MusicEditScene.h"
#include "Note.h"
#include "NoteLineUI.h"

MusicEditScene::MusicEditScene(const MusicInfo& musicInfo) :
    m_musicInfo(musicInfo),
    m_audioPlayer([&]()
    {
        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        return *tgon::AudioPlayer::Create(assetModule->GetResource<tgon::AudioBuffer>(musicInfo.musicPath));
    } ()),
    m_keyboard(tgon::Application::GetEngine()->FindModule<tgon::InputModule>()->GetKeyboard()),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_audioModule(tgon::Application::GetEngine()->FindModule<tgon::AudioModule>())
{
}

void MusicEditScene::Initialize()
{
    Super::Initialize();
    
    this->InitializeBackgroundObject();
    this->InitializeNoteLineUI();
    this->InitializeNoteObjectPool();
    this->InitializeHoldNoteObjectPool();
    this->InitializeMusicNameObject();
    this->InitializeMusicArtistNameObject();
  
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
    
    if (m_isMusicStopped == false)
    {
        m_elapsedTime += m_timeModule->GetTickTime();
    }
    
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
            const char* noteIndexStr[5] = {"10000:", "01000:", "00100:", "00010:", "00001:"};

            double bitPerSeconds = m_musicInfo.bpm / 60.0f;
            auto clap = 8.0f * bitPerSeconds * (noteInfo.hitTime - m_musicInfo.sync);
            tgon::Debug::WriteLine(std::string(noteIndexStr[noteInfo.noteIndex]) + std::to_string(int(clap)));
        }
    }
    else if (m_keyboard->IsKeyDown(tgon::KeyCode::F2))
    {
        if (m_isMusicStopped)
        {
            m_isMusicStopped = false;
            m_audioPlayer.Resume();
        }
        else
        {
            m_isMusicStopped = true;
            m_audioPlayer.Pause();
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
            std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<Note>> noteObject;
            if (noteInfo.holdTime == 0.0f)
            {
                noteObject = this->GetNoteObjectFromPool();
            }
            else
            {
                auto holdNoteObject = this->GetHoldNoteObjectFromPool();
                holdNoteObject.second->SetHoldTime(noteInfo.holdTime);
                noteObject = holdNoteObject;
            }
            noteObject.second->SetHitTime(noteInfo.hitTime);
            noteObject.second->SetNoteLineIndex(noteInfo.noteIndex);

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
            if (iter->second->GetHitTime() - m_elapsedTime < -0.5f)
            {
                bool isNormalNote = iter->second->GetRTTI() != tgon::GetRTTI<HoldNote*>();
                if (isNormalNote)
                {
                    m_noteObjectPool.push_back(*iter);
                    iter = noteObjects.erase(iter);
                    continue;
                }
                else if (iter->second->IsHolding() == false)
                {
                    m_holdNoteObjectPool.emplace_back(iter->first, std::static_pointer_cast<HoldNote>(iter->second));
                    iter = noteObjects.erase(iter);
                    continue;
                }
            }
        
            iter->second->SetElapsedTime(m_elapsedTime);
            iter->first->Update();
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
    
    auto spriteRendererComponent = backgroundObject->AddComponent<tgon::UISpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>("Resource/Background/MusicPlayScene/green.png"));
    spriteRendererComponent->SetPivot({0.0f, 0.5f});
    
    this->AddObject(backgroundObject);
    
    m_backgroundObject = std::move(backgroundObject);
}

void MusicEditScene::InitializeNoteLineUI()
{
    auto noteLineObject = tgon::GameObject::Create();
    noteLineObject->GetTransform()->SetLocalPosition(tgon::Vector3(-10.0f, -40.0f, 0.0f));
    auto noteLineComponent = noteLineObject->AddComponent<NoteLineUI>();
    this->AddObject(noteLineObject);

    m_noteLine = std::move(noteLineComponent);
}

void MusicEditScene::InitializeNoteObjectPool()
{
    for (size_t i = 0; i < 40; ++i)
    {
        auto noteObject = tgon::GameObject::Create();
        auto noteComponent = noteObject->AddComponent<Note>(m_noteLine);

        m_noteObjectPool.emplace_back(std::move(noteObject), std::move(noteComponent));
    }
}

void MusicEditScene::InitializeHoldNoteObjectPool()
{
    for (size_t i = 0; i < 20; ++i)
    {
        auto holdNoteObject = tgon::GameObject::Create();
        auto holdNoteComponent = holdNoteObject->AddComponent<HoldNote>(m_noteLine);

        m_holdNoteObjectPool.emplace_back(std::move(holdNoteObject), std::move(holdNoteComponent));
    }
}

void MusicEditScene::InitializeMusicNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 33.0f, windowSize.height / 2 - 24.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::UITextRendererComponent>();
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

    auto textComponent = object->AddComponent<tgon::UITextRendererComponent>();
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
            if (noteObject.second->GetRTTI() != tgon::GetRTTI<HoldNote*>())
            {
                m_noteObjectPool.push_back(noteObject);
            }
            else
            {
                m_holdNoteObjectPool.emplace_back(noteObject.first, std::static_pointer_cast<HoldNote>(noteObject.second));
            }

            noteObjects.pop_back();
        }
    }

    this->UpdateNotes();
}

std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<Note>> MusicEditScene::GetNoteObjectFromPool()
{
    if (m_noteObjectPool.size() == 0)
    {
        auto noteObject = tgon::GameObject::Create();
        auto noteComponent = noteObject->AddComponent<Note>(m_noteLine);

        m_noteObjectPool.emplace_back(std::move(noteObject), std::move(noteComponent));
    }
    
    auto ret = m_noteObjectPool.back();
    ret.second->Reset();
    
    m_noteObjectPool.pop_back();
    
    return ret;
}

std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<HoldNote>> MusicEditScene::GetHoldNoteObjectFromPool()
{
    if (m_holdNoteObjectPool.size() == 0)
    {
        auto holdNoteObject = tgon::GameObject::Create();
        auto holdNoteComponent = holdNoteObject->AddComponent<HoldNote>(m_noteLine);

        m_holdNoteObjectPool.emplace_back(std::move(holdNoteObject), std::move(holdNoteComponent));
    }
    
    auto ret = m_holdNoteObjectPool.back();
    ret.second->Reset();
    
    m_holdNoteObjectPool.pop_back();
    
    return ret;
}
