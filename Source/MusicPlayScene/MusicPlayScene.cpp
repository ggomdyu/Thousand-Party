#include "Platform/Application.h"
#include "Engine/AssetModule.h"
#include "Engine/AudioModule.h"
#include "Engine/TimeModule.h"
#include "Engine/TimerModule.h"
#include "Engine/TaskModule.h"
#include "Game/UISpriteRendererComponent.h"
#include "Game/UITextRendererComponent.h"

#include "MusicPlayScene.h"
#include "Note.h"
#include "NoteHitInfo.h"
#include "NoteLineUI.h"
#include "MusicLeftTimeUI.h"

MusicPlayScene::MusicPlayScene() :
    m_audioPlayer(*tgon::AudioPlayer::Create()),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_audioModule(tgon::Application::GetEngine()->FindModule<tgon::AudioModule>())
{
}

void MusicPlayScene::Initialize()
{
    Super::Initialize();
    
    this->InitializeBackgroundObject();
    this->InitializeNoteHitInfo();
    this->InitializeNoteLineUI();
    this->InitializeMusicLeftTimeUI();
    this->InitializeNoteObjectPool();
    this->InitializeHoldNoteObjectPool();
    this->InitializeMusicNameObject();
    this->InitializeMusicArtistNameObject();
}

void MusicPlayScene::Update()
{
    Super::Update();
    
    this->UpdateBackgroundObjectPosition();
    
    m_elapsedTime += m_timeModule->GetTickTime();
    
    if (m_isMusicWaiting)
    {
        return;
    }
    
    m_musicLeftTime->SetProgress(m_elapsedTime / m_audioPlayer.GetTotalProgressInSeconds());
    
    this->UpdateNotes();
}

void MusicPlayScene::OnActivate()
{
    m_musicNameRendererComponent->SetText(m_musicInfo.musicName);
    m_musicArtistNameRendererComponent->SetText(m_musicInfo.musicAuthorName);

    auto taskModule = tgon::Application::GetEngine()->FindModule<tgon::TaskModule>();
    taskModule->GetGlobalDispatchQueue().AddAsyncTask([&, taskModule]()
    {
        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        m_audioPlayer.SetAudioBuffer(assetModule->GetResource<tgon::AudioBuffer>(m_musicInfo.musicPath));
        
        taskModule->GetMainDispatchQueue().AddAsyncTask([&]()
        {
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
        });
    });
}

void MusicPlayScene::SetMusicInfo(const MusicInfo& musicInfo)
{
    m_musicInfo = musicInfo;
}

void MusicPlayScene::UpdateNotes()
{
    // Update for note creation
    for (size_t i = m_noteInfoIndex; i < m_musicInfo.noteInfos.size(); ++i)
    {
        auto& noteInfo = m_musicInfo.noteInfos[i];
        if (noteInfo.hitTime - m_elapsedTime < 1.0f)
        {
            NoteObjectPair noteObject;
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
                    m_noteHitInfo->OnMissNote();
                    m_noteObjectPool.push_back(*iter);
                    iter = noteObjects.erase(iter);
                    continue;
                }
                else if (iter->second->IsHolding() == false)
                {
                    m_noteHitInfo->OnMissNote();
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

    // Update for note input update
    for (auto& noteObjects : m_notes)
    {
        for (auto iter = noteObjects.begin(); iter != noteObjects.end();)
        {
            bool canHitNote = iter->second->CheckCanHit();
            if (canHitNote)
            {
                iter->second->UpdateInput();
                if (iter->second->IsHitted())
                {
                    if (iter->second->GetRTTI() != tgon::GetRTTI<HoldNote*>())
                    {
                        m_noteHitInfo->OnHitNote();
                        m_noteObjectPool.push_back(*iter);
                        iter = noteObjects.erase(iter);
                        break;
                    }
                    else if (iter->second->IsHolding() == false)
                    {
                        m_noteHitInfo->OnMissNote();
                        m_holdNoteObjectPool.emplace_back(iter->first, std::static_pointer_cast<HoldNote>(iter->second));
                        iter = noteObjects.erase(iter);
                        continue;
                    }
                }
            }
            
            ++iter;
        }
    }
}

void MusicPlayScene::UpdateBackgroundObjectPosition()
{
    auto backgroundObjectPos = m_backgroundObject->GetTransform()->GetLocalPosition();
    backgroundObjectPos.x -= 3.0f * m_timeModule->GetTickTime();
    m_backgroundObject->GetTransform()->SetLocalPosition(backgroundObjectPos);
}

void MusicPlayScene::InitializeNoteHitInfo()
{
    auto noteHitInfoObject = tgon::GameObject::Create();
    m_noteHitInfo = noteHitInfoObject->AddComponent<NoteHitInfo>();
    this->AddChild(std::move(noteHitInfoObject));
}

void MusicPlayScene::InitializeBackgroundObject()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto backgroundObject = tgon::GameObject::Create();
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    backgroundObject->GetTransform()->SetLocalPosition({-static_cast<float>(clientSize.width) * 0.5f, 0.0f, 0.0f});
    
    auto spriteRendererComponent = backgroundObject->AddComponent<tgon::UISpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>("Resource/Background/MusicPlayScene/green.png"));
    spriteRendererComponent->SetPivot({0.0f, 0.5f});
    
    this->AddChild(backgroundObject);
    
    m_backgroundObject = std::move(backgroundObject);
}

void MusicPlayScene::InitializeNoteLineUI()
{
    auto noteLineObject = tgon::GameObject::Create();
    noteLineObject->GetTransform()->SetLocalPosition(tgon::Vector3(-10.0f, -40.0f, 0.0f));
    m_noteLine = noteLineObject->AddComponent<NoteLineUI>();
    this->AddChild(noteLineObject);
}

void MusicPlayScene::InitializeMusicLeftTimeUI()
{
    auto musicLeftTimeObject = tgon::GameObject::Create();
    musicLeftTimeObject->GetTransform()->SetLocalPosition(tgon::Vector3(161.0f, 96.0f, 0.0f));
    m_musicLeftTime = musicLeftTimeObject->AddComponent<MusicLeftTimeUI>();
    this->AddChild(musicLeftTimeObject);
}

void MusicPlayScene::InitializeNoteObjectPool()
{
    for (size_t i = 0; i < 40; ++i)
    {
        auto noteObject = tgon::GameObject::Create();
        auto noteComponent = noteObject->AddComponent<Note>(m_noteLine);

        m_noteObjectPool.emplace_back(std::move(noteObject), std::move(noteComponent));
    }
}

void MusicPlayScene::InitializeHoldNoteObjectPool()
{
    for (size_t i = 0; i < 20; ++i)
    {
        auto holdNoteObject = tgon::GameObject::Create();
        auto holdNoteComponent = holdNoteObject->AddComponent<HoldNote>(m_noteLine);

        m_holdNoteObjectPool.emplace_back(std::move(holdNoteObject), std::move(holdNoteComponent));
    }
}

void MusicPlayScene::InitializeMusicNameObject()
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

    m_musicNameRendererComponent = textComponent;

    this->AddChild(object);
}

void MusicPlayScene::InitializeMusicArtistNameObject()
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
    
    m_musicArtistNameRendererComponent = textComponent;

    this->AddChild(object);
}

MusicPlayScene::NoteObjectPair MusicPlayScene::GetNoteObjectFromPool()
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

std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<HoldNote>> MusicPlayScene::GetHoldNoteObjectFromPool()
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
