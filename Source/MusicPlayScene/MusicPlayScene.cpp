#include "PrecompiledHeader.h"

#include "TGON.h"
#include "MusicPlayScene.h"
#include "Note.h"

MusicPlayScene::MusicPlayScene(const MusicInfo& musicInfo) :
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_audioModule(tgon::Application::GetEngine()->FindModule<tgon::AudioModule>()),
    m_musicInfo(musicInfo),
    m_isMusicWaiting(true),
    m_elapsedTime(0.0f),
    m_noteInfoIndex(0)
{
}

void MusicPlayScene::Initialize()
{
    Super::Initialize();
    
    this->InitializeBackgroundObject();
    this->InitializeHitRingObject();
    this->InitializeNoteObjectPool();
    this->InitializeLongNoteObjectPool();
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_audioPlayer.Initialize(assetModule->GetAudioBuffer(m_musicInfo.musicPath));
    
    auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
    timerModule->SetTimer([this](tgon::TimerHandle timerHandle)
    {
        m_audioPlayer.Play(1.0f, false);
        m_isMusicWaiting = false;
    }, 3.0f, false);
}

void MusicPlayScene::Update()
{
    Super::Update();
    
    m_elapsedTime += m_timeModule->GetTickTime();
    
    if (m_isMusicWaiting)
    {
        return;
    }
    
    for (size_t i = m_noteInfoIndex; i < m_musicInfo.noteInfos.size(); ++i)
    {
        auto& noteInfo = m_musicInfo.noteInfos[i];
        if (noteInfo.hitTime - m_elapsedTime < 1.0f)
        {
            auto noteObject = (noteInfo.isLongNote) ? this->GetLongNoteObjectFromPool() : this->GetNoteObjectFromPool();
            noteObject->SetHitTime(noteInfo.hitTime);
            noteObject->SetNoteLineIndex(noteInfo.noteIndex);
            
            m_noteLine[noteInfo.noteIndex].push_back(std::move(noteObject));
            ++m_noteInfoIndex;
        }
        else
        {
            break;
        }
    }
}

void MusicPlayScene::UpdateNoteLine()
{
    for (auto& noteObjects : m_noteLine)
    {
        for (auto iter = noteObjects.begin(); iter != noteObjects.end();)
        {
            if ((*iter)->GetHitTime() - m_elapsedTime < -0.5f)
            {
                if (tgon::DynamicCast<LongNote*>(iter->get()) == nullptr)
                {
                    m_noteObjectPool.push_back(*iter);
                }
                else
                {
                    m_longNoteObjectPool.push_back(std::static_pointer_cast<LongNote>(*iter));
                }
                
                iter = noteObjects.erase(iter);
            }
            else
            {
                (*iter)->SetElapsedTime(m_elapsedTime);
                (*iter)->Update();
                ++iter;
            }
        }
    }
}

void MusicPlayScene::InitializeBackgroundObject()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    
    auto backgroundObject = tgon::GameObject::Create();
    auto spriteRendererComponent = backgroundObject->AddComponent<tgon::SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture("Resource/Background/MusicPlayScene/green.png"));
    
    this->AddObject(std::move(backgroundObject));
}

void MusicPlayScene::InitializeNoteObjectPool()
{
    for (size_t i = 0; i < 50; ++i)
    {
        m_noteObjectPool.push_back(tgon::GameObject::Create<Note>());
    }
}

void MusicPlayScene::InitializeLongNoteObjectPool()
{
    for (size_t i = 0; i < 30; ++i)
    {
//        m_longNoteObjectPool.push_back(tgon::GameObject::Create<LongNote>());
    }
}

void MusicPlayScene::InitializeHitRingObject()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    
    auto hitRingObject = tgon::GameObject::Create();
    auto spriteRendererComponent = hitRingObject->AddComponent<tgon::SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture("Resource/Object/PlayScene/Ring.png"));
    spriteRendererComponent->SetPivot({0.0f, 0.5f});
    
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    hitRingObject->GetTransform()->SetLocalPosition({-static_cast<float>(clientSize.width) * 0.5f + 120.0f, -85.0f, 0.0f});
    this->AddObject(std::move(hitRingObject));
}

std::shared_ptr<Note> MusicPlayScene::GetNoteObjectFromPool()
{
    auto ret = m_noteObjectPool.back();
    m_noteObjectPool.pop_back();
    
    return ret;
}

std::shared_ptr<LongNote> MusicPlayScene::GetLongNoteObjectFromPool()
{
    auto ret = m_longNoteObjectPool.back();
    m_longNoteObjectPool.pop_back();
    
    return ret;
}
