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
}

void MusicPlayScene::Update()
{
    Super::Update();
    
    this->UpdateBackgroundObjectPosition();
    
    if (m_isMusicWaiting)
    {
        return;
    }
    
    m_elapsedTime += m_timeModule->GetTickTime();
    
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
    
    this->UpdateNoteLine();
}

void MusicPlayScene::UpdateNoteLine()
{
    for (auto& noteObjects : m_noteLine)
    {
        for (auto iter = noteObjects.begin(); iter != noteObjects.end(); ++iter)
        {
            bool canHitNote = (*iter)->CheckCanHit();
            if (canHitNote)
            {
                (*iter)->UpdateInput();
                if ((*iter)->IsHitted())
                {
                    m_noteObjectPool.push_back((*iter));
                    noteObjects.erase(iter);
                }
                break;
            }
        }
    }

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

void MusicPlayScene::UpdateBackgroundObjectPosition()
{
    auto backgroundObjectTransform = m_backgroundObject->GetTransform();
    auto backgroundObjectPos = backgroundObjectTransform->GetLocalPosition();
    backgroundObjectPos.x -= 3.0f * m_timeModule->GetTickTime();
    backgroundObjectTransform->SetLocalPosition(backgroundObjectPos);
}

void MusicPlayScene::InitializeBackgroundObject()
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

void MusicPlayScene::InitializeMusicNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 33.0f, windowSize.height / 2 - 38.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(45);
    textComponent->SetRect(tgon::I32Rect(0, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::UpperLeft);
    textComponent->SetSortingLayer(4);
    textComponent->SetText(m_musicInfo.musicName);

    m_musicNameRendererComponent = textComponent;

    this->AddObject(object);
}

void MusicPlayScene::InitializeMusicArtistNameObject()
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

std::shared_ptr<Note> MusicPlayScene::GetNoteObjectFromPool()
{
    if (m_noteObjectPool.size() == 0)
    {
        m_noteObjectPool.push_back(tgon::GameObject::Create<Note>());
    }
    
    auto ret = m_noteObjectPool.back();
    ret->Reset();
    
    m_noteObjectPool.pop_back();
    
    return ret;
}

std::shared_ptr<LongNote> MusicPlayScene::GetLongNoteObjectFromPool()
{
    if (m_longNoteObjectPool.size() == 0)
    {
        m_longNoteObjectPool.push_back(tgon::GameObject::Create<LongNote>());
    }
    
    auto ret = m_longNoteObjectPool.back();
    ret->Reset();
    
    m_longNoteObjectPool.pop_back();
    
    return ret;
}
