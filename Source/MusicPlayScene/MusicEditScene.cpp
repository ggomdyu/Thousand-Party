#include "Platform/Application.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "Engine/AssetModule.h"
#include "Engine/AudioModule.h"
#include "Engine/TimeModule.h"
#include "Engine/TimerModule.h"
#include "Engine/TaskModule.h"
#include "Engine/InputModule.h"
#include "Hardware/Keyboard.h"
#include "Game/UISpriteRendererComponent.h"
#include "Game/UITextRendererComponent.h"
#include "Graphics/OpenGL/OpenGLShaderCode.h"
#include "Diagnostics/Debug.h"

#include "MusicEditScene.h"
#include "Note.h"
#include "NoteComboInfo.h"
#include "NoteLineUI.h"
#include "MusicLeftTimeUI.h"

#include "../MultipleSceneModule.h"

MusicEditScene::MusicEditScene() :
    m_audioPlayer(*tgon::AudioSource::Create()),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_audioModule(tgon::Application::GetEngine()->FindModule<tgon::AudioModule>()),
    m_keyboard(tgon::Application::GetEngine()->FindModule<tgon::InputModule>()->GetKeyboard())
{
}

void MusicEditScene::Initialize()
{
    Super::Initialize();

    this->InitializeBackgroundObject();
    this->InitializeNoteComboInfo();
    this->InitializeCoverImageUI();
    this->InitializeNoteLineBoxUI();
    this->InitializeMusicLeftTimeUI();
    this->InitializeNoteObjectPool();
    this->InitializeHoldNoteObjectPool();
    this->InitializeMusicNameObject();
    this->InitializeMusicArtistNameObject();
    this->InitializeFadeOutUI();
}

void MusicEditScene::Update()
{
    Super::Update();

    this->UpdateNoteLineEdgeOffset();
    
    if (m_isMusicWaiting)
    {
        return;
    }
    
    m_elapsedTime += m_timeModule->GetTickTime();
    m_musicLeftTime->SetProgress(m_elapsedTime / m_audioPlayer.GetTotalProgressInSeconds());
    
    this->UpdateNotes();
    this->OnHandleInput();
}

void MusicEditScene::UpdateNoteLineEdgeOffset()
{
    m_noteLineEdgeOffset += m_timeModule->GetTickTime() * 0.02;
    m_noteLineEdgeMaterial->SetParameter2f("uvOffset", m_noteLineEdgeOffset, 0.0f);
}

void MusicEditScene::PlayMusicFinishFadeOut(tgon::Delegate<void()>&& onFinishFadeOut)
{
    auto timeModule = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>();
    auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
    timerModule->SetTimer([this, timeModule, weakTimerModule = std::weak_ptr<tgon::TimerModule>(timerModule), onFinishFadeOut = std::move(onFinishFadeOut)](tgon::TimerHandle timerHandle)
    {
        auto blendColor = m_fadeOutSpriteComponent->GetBlendColor();
        blendColor.a += timeModule->GetTickTime() * 3.0f;
        m_fadeOutSpriteComponent->SetBlendColor(blendColor);
        
        if (blendColor.a >= 1.0f)
        {
            if (auto timerModule = weakTimerModule.lock(); timerModule != nullptr)
            {
                timerModule->ClearTimer(timerHandle);
                onFinishFadeOut();
            }
        }
        
    }, 0.0f, true);
}

void MusicEditScene::OnActivate()
{
    std::string musicDirectory = std::string(tgon::Path::GetDirectoryName(m_musicInfo.musicPath));
    std::string coverImagePath = musicDirectory + "/cover.png";
    if (tgon::File::Exists(coverImagePath.c_str()) == false)
    {
        coverImagePath = musicDirectory + "/cover.jpg";
    }
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto coverImageTexture = assetModule->GetResource<tgon::Texture>(coverImagePath);
    m_coverImageSpriteComponent->SetTexture(coverImageTexture);
    m_coverImageSpriteComponent->SetTextureSize({58.0f, 58.0f});
    m_coverImageSpriteComponent->SetTextureRect({0.0f, 0.0f, 58.0f, 58.0f});

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float backgroundSize = std::max(clientSize.width, clientSize.height);
    m_backgroundSpriteComponent->SetTexture(coverImageTexture);
    m_backgroundSpriteComponent->SetTextureSize({backgroundSize, backgroundSize});
    m_backgroundSpriteComponent->SetTextureRect({0, 0, backgroundSize, backgroundSize});
    m_backgroundSpriteComponent->SetPivot({0.5f, 0.5f});

    m_musicNameTextComponent->SetText(m_musicInfo.musicName);
    m_musicArtistNameTextComponent->SetText(m_musicInfo.musicAuthorName);
    
    m_fadeOutSpriteComponent->SetBlendColor({1.0f, 1.0f, 1.0f, 0.0f});

    auto taskModule = tgon::Application::GetEngine()->FindModule<tgon::TaskModule>();
    taskModule->GetGlobalDispatchQueue().AddAsyncTask([&, taskModule, assetModule]()
    {
        m_audioPlayer.SetClip(assetModule->GetResource<tgon::AudioClip>(m_musicInfo.musicPath));
        m_audioPlayer.SetVolume(1.0f);
        m_audioPlayer.SetLoop(false);
        
        taskModule->GetMainDispatchQueue().AddAsyncTask([&]()
        {
            auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
            timerModule->SetTimer([this](tgon::TimerHandle timerHandle)
            {
                m_audioPlayer.Play();
                m_isMusicWaiting = false;
            }, 3.0f, false);
        });
    });
}

void MusicEditScene::SetMusicInfo(const MusicInfo& musicInfo)
{
    m_musicInfo = musicInfo;
}

void MusicEditScene::UpdateNotes()
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
                    m_noteComboInfo->OnMissNote();
                    m_noteObjectPool.push_back(*iter);
                    iter = noteObjects.erase(iter);
                    continue;
                }
                else if (iter->second->IsHolding() == false)
                {
                    // Call OnMissNote twice because Hold note has two hit timing.
                    m_noteComboInfo->OnMissNote();
                    m_noteComboInfo->OnMissNote();
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
                        m_noteObjectPool.push_back(*iter);
                        iter = noteObjects.erase(iter);
                        break;
                    }
                    else if (iter->second->IsHolding() == false)
                    {
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

void MusicEditScene::InitializeNoteComboInfo()
{
    auto noteComboInfoObject = tgon::GameObject::Create();
    m_noteComboInfo = noteComboInfoObject->AddComponent<NoteComboInfo>();

    this->AddChild(std::move(noteComboInfoObject));
}

void MusicEditScene::InitializeBackgroundObject()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto backgroundObject = tgon::GameObject::Create();
 
    auto spriteComponent = backgroundObject->AddComponent<tgon::UISpriteRendererComponent>();
    spriteComponent->SetBlendColor({0.6f, 0.6f, 0.6f, 1.0f});
    spriteComponent->SetMaterial(std::make_shared<tgon::Material>(g_positionColorUVVert, g_blurFrag));
    
    this->AddChild(backgroundObject);
    
    m_backgroundSpriteComponent = std::move(spriteComponent);
}

void MusicEditScene::InitializeCoverImageUI()
{
    auto coverImage = tgon::GameObject::Create();
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    coverImage->GetTransform()->SetLocalPosition({-clientSize.width * 0.5f + 60.0f, clientSize.height * 0.5f - 54.0f, 0.0f});
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto coverImageSpriteComponent = coverImage->AddComponent<tgon::UISpriteRendererComponent>();
    m_coverImageSpriteComponent = coverImageSpriteComponent;
    
    auto highlight = tgon::GameObject::Create();
    highlight->GetTransform()->SetLocalScale({0.265f, 0.265f, 1.0f});
    auto highlightSpriteComponent = highlight->AddComponent<tgon::UISpriteRendererComponent>();
    highlightSpriteComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicSelectScene/highlight.png"));
    highlightSpriteComponent->SetBlendColor({0.0f, 0.0f, 0.0f, 0.4f});
    coverImage->AddChild(highlight);
    
    this->AddChild(coverImage);
}

void MusicEditScene::InitializeNoteLineBoxUI()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();

    // Create NoteLineUI's background
    auto noteLineBackground = tgon::GameObject::Create();
    noteLineBackground->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, -45.0f, 0.0f));
    auto noteLineBackgroundRendererComponent = noteLineBackground->AddComponent<tgon::UISpriteRendererComponent>();
    noteLineBackgroundRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>("Resource/UI/MusicPlayScene/noteLineBg.png"));
    this->AddChild(noteLineBackground);

    auto noteLineEdgeMaterial = std::make_shared<tgon::Material>(g_positionColorUVVert, g_uvOffsetFrag);
    auto noteLineEdgeTexture = assetModule->GetResource<tgon::Texture>("Resource/UI/MusicPlayScene/line.png");
    noteLineEdgeTexture->SetWrapMode(tgon::WrapMode::Repeat);
    
    auto noteLineEdge1 = tgon::GameObject::Create();
    noteLineEdge1->GetTransform()->SetLocalPosition({0.0f, noteLineBackgroundRendererComponent->GetTexture()->GetSize().height * 0.5f + 1.0f, 0.0f});
    auto noteLineEdge1RendererComponent = noteLineEdge1->AddComponent<tgon::UISpriteRendererComponent>();
    noteLineEdge1RendererComponent->SetTexture(noteLineEdgeTexture);
    noteLineEdge1RendererComponent->SetMaterial(noteLineEdgeMaterial);
    noteLineBackground->AddChild(noteLineEdge1);

    auto noteLineEdge2 = tgon::GameObject::Create();
    noteLineEdge2->GetTransform()->SetLocalPosition({0.0f, -noteLineBackgroundRendererComponent->GetTexture()->GetSize().height * 0.5f - 4.0f, 0.0f});
    auto noteLineEdge2RendererComponent = noteLineEdge2->AddComponent<tgon::UISpriteRendererComponent>();
    noteLineEdge2RendererComponent->SetTexture(noteLineEdgeTexture);
    noteLineEdge2RendererComponent->SetMaterial(noteLineEdgeMaterial);
    noteLineBackground->AddChild(noteLineEdge2);

    m_noteLineEdgeMaterial = std::move(noteLineEdgeMaterial);

    // Create NoteLineUI
    auto noteLineObject = tgon::GameObject::Create();
    noteLineObject->GetTransform()->SetLocalPosition(tgon::Vector3(-10.0f, 4.0f, 0.0f));
    m_noteLine = noteLineObject->AddComponent<NoteLineUI>();
    noteLineBackground->AddChild(noteLineObject);
}

void MusicEditScene::InitializeMusicLeftTimeUI()
{
    auto object = tgon::GameObject::Create();
    m_musicLeftTime = object->AddComponent<MusicLeftTimeUI>();
    this->AddChild(object);
}

void MusicEditScene::InitializeFadeOutUI()
{
    auto object = tgon::GameObject::Create();
    object->GetTransform()->SetLocalScale(tgon::Vector3(858.0f, 462.0f, 1.0f));
        
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto texture = assetModule->GetResource<tgon::Texture>("Resource/UI/Common/board01.png");
    m_fadeOutSpriteComponent = object->AddComponent<tgon::UISpriteRendererComponent>();
    m_fadeOutSpriteComponent->SetTexture(texture);

    this->AddChild(object);
}

void MusicEditScene::InitializeNoteObjectPool()
{
    for (size_t i = 0; i < 40; ++i)
    {
        auto noteObject = tgon::GameObject::Create();
        auto noteComponent = noteObject->AddComponent<Note>(m_noteLine);
        noteComponent->OnHit = tgon::Delegate(&MusicEditScene::OnHitNote, this);

        m_noteObjectPool.emplace_back(std::move(noteObject), std::move(noteComponent));
    }
}

void MusicEditScene::InitializeHoldNoteObjectPool()
{
    for (size_t i = 0; i < 20; ++i)
    {
        auto holdNoteObject = tgon::GameObject::Create();
        auto holdNoteComponent = holdNoteObject->AddComponent<HoldNote>(m_noteLine);
        holdNoteComponent->OnHit = tgon::Delegate(&MusicEditScene::OnHitNote, this);

        m_holdNoteObjectPool.emplace_back(std::move(holdNoteObject), std::move(holdNoteComponent));
    }
}

void MusicEditScene::InitializeMusicNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 103.0f, windowSize.height / 2 - 19.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(31);
    textComponent->SetRect(tgon::I32Rect(0, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleLeft);

    m_musicNameTextComponent = textComponent;

    this->AddChild(object);
}

void MusicEditScene::InitializeMusicArtistNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicArtistName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 104.0f, windowSize.height / 2 - 65.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(16);
    textComponent->SetRect(tgon::I32Rect(0, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::UpperLeft);
    
    m_musicArtistNameTextComponent = textComponent;

    this->AddChild(object);
}

void MusicEditScene::OnHitNote(NoteTiming noteTiming)
{
    m_noteComboInfo->OnHitNote(noteTiming);
}

MusicEditScene::NoteObjectPair MusicEditScene::GetNoteObjectFromPool()
{
    if (m_noteObjectPool.size() == 0)
    {
        auto noteObject = tgon::GameObject::Create();
        auto noteComponent = noteObject->AddComponent<Note>(m_noteLine);
        noteComponent->OnHit = tgon::Delegate(&MusicEditScene::OnHitNote, this);

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
        holdNoteComponent->OnHit = tgon::Delegate(&MusicEditScene::OnHitNote, this);

        m_holdNoteObjectPool.emplace_back(std::move(holdNoteObject), std::move(holdNoteComponent));
    }

    auto ret = m_holdNoteObjectPool.back();
    ret.second->Reset();

    m_holdNoteObjectPool.pop_back();

    return ret;
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
            m_audioPlayer.UnPause();
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
