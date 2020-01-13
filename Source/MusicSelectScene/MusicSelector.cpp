#include "MusicSelector.h"

#include "Platform/Application.h"
#include "Engine/AssetModule.h"
#include "Engine/TimeModule.h"
#include "Engine/SceneModule.h"
#include "Engine/InputModule.h"
#include "Engine/TimerModule.h"
#include "IO/Directory.h"
#include "Component/UISpriteRendererComponent.h"
#include "Math/Mathematics.h"

#include "../MusicPlayScene/MusicPlayScene.h"
#include "../MusicPlayScene/MusicEditScene.h"
#include "../GameDataModule.h"

namespace
{
    
constexpr tgon::Vector3 g_coverImagePositions[] = {
    tgon::Vector3(-180.0f, 0.0f, 0.0f),
    tgon::Vector3(-280.0f, 0.0f, 0.0f),
    tgon::Vector3(-180.0f, 0.0f, 0.0f),
    tgon::Vector3(0.0f, 0.0f, 0.0f),
    tgon::Vector3(180.0f, 0.0f, 0.0f),
    tgon::Vector3(280.0f, 0.0f, 0.0f),
    tgon::Vector3(180.0f, 0.0f, 0.0f),
};
constexpr tgon::Vector3 g_coverImageScales[] = {
    tgon::Vector3(0.45f, 0.45f, 1.0f),
    tgon::Vector3(0.6f, 0.6f, 1.0f),
    tgon::Vector3(0.75f, 0.75f, 1.0f),
    tgon::Vector3(1.0f, 1.0f, 1.0f),
    tgon::Vector3(0.75f, 0.75f, 1.0f),
    tgon::Vector3(0.6f, 0.6f, 1.0f),
    tgon::Vector3(0.45f, 0.45f, 1.0f),
};

} /* namespace */

MusicSelector::MusicSelector() :
    Component(),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_inputModule(tgon::Application::GetEngine()->FindModule<tgon::InputModule>()),
    m_gameDataModule(tgon::Application::GetEngine()->FindModule<GameDataModule>())
{
}

void MusicSelector::Initialize()
{
    this->InitializeMusicCoverObjects();
    this->InitializeHighlightObject();
    
    this->SortMusicList();
    this->SortMusicLayer();
    this->RefreshMusicCoverHighlight();
}

void MusicSelector::Update()
{
    Super::Update();
    
    this->OnHandleInput();
}

void MusicSelector::InitializeMusicCoverObjects()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }

    auto noteDirectories = tgon::Directory::GetDirectories("Note");
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (size_t i = 0; i < noteDirectories.size(); ++i)
    {
        std::shared_ptr<tgon::Texture> texture;
        if (auto pngCoverPath = noteDirectories[i] + "/cover.png"; tgon::File::Exists(pngCoverPath.c_str()))
        {
            texture = assetModule->GetResource<tgon::Texture>(pngCoverPath);
        }
        else if (auto jpgCoverPath = noteDirectories[i] + "/cover.jpg"; tgon::File::Exists(jpgCoverPath.c_str()))
        {
            texture = assetModule->GetResource<tgon::Texture>(jpgCoverPath);
        }
        else
        {
            continue;
        }

        auto coverImageObject = tgon::GameObject::Create();
        coverImageObject->GetTransform()->SetParent(gameObject->GetTransform());
        auto spriteRendererComponent = coverImageObject->AddComponent<tgon::UISpriteRendererComponent>();
        spriteRendererComponent->SetTexture(std::move(texture));
        spriteRendererComponent->SetTextureSize({222.0f, 222.0f});
        spriteRendererComponent->SetTextureRect({0.0f, 0.0f, 222.0f, 222.0f});
        spriteRendererComponent->SetSortingLayer(4);

        m_coverImageObjects.push_back(std::move(coverImageObject));
    }
}

void MusicSelector::InitializeHighlightObject()
{
    m_highlight = tgon::GameObject::Create();
    
    auto spriteRendererComponent = m_highlight->AddComponent<tgon::UISpriteRendererComponent>();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicSelectScene/highlight.png"));
    spriteRendererComponent->SetSortingLayer(7);
}

void MusicSelector::OnHandleInput()
{
    auto keyboard = m_inputModule->GetKeyboard();
    if (keyboard->IsKeyDown(tgon::KeyCode::LeftArrow))
    {
        if (m_currSelectedCoverImageIndex > -3)
        {
            --m_currSelectedCoverImageIndex;
            this->AnimateMusicCoverObject();

            if (OnChangeSelectedMusic != nullptr)
            {
                OnChangeSelectedMusic(m_gameDataModule->GetMusicInfos()[m_currSelectedCoverImageIndex + 3]);
            }
        }
    }
    else if (keyboard->IsKeyDown(tgon::KeyCode::RightArrow))
    {
        if (m_currSelectedCoverImageIndex < static_cast<int32_t>(m_coverImageObjects.size()) - 4)
        {
            ++m_currSelectedCoverImageIndex;
            this->AnimateMusicCoverObject();

            if (OnChangeSelectedMusic != nullptr)
            {
                OnChangeSelectedMusic(m_gameDataModule->GetMusicInfos()[m_currSelectedCoverImageIndex + 3]);
            }
        }
    }
    else if (keyboard->IsKeyDown(tgon::KeyCode::Space))
    {
        auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
        if (m_animationTimer != tgon::TimerHandle{})
        {
            timerModule->ClearTimer(m_animationTimer);
        }
        
        auto sceneModule = tgon::Application::GetEngine()->FindModule<tgon::SceneModule>();
        sceneModule->ChangeScene<MusicPlayScene>(m_gameDataModule->GetMusicInfos()[m_currSelectedCoverImageIndex + 3]);
    }
    else if (keyboard->IsKeyDown(tgon::KeyCode::F1))
    {
        auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
        if (m_animationTimer != tgon::TimerHandle{})
        {
            timerModule->ClearTimer(m_animationTimer);
        }

        auto sceneModule = tgon::Application::GetEngine()->FindModule<tgon::SceneModule>();
        sceneModule->ChangeScene<MusicEditScene>(m_gameDataModule->GetMusicInfos()[m_currSelectedCoverImageIndex + 3]);
    }
}

void MusicSelector::AnimateMusicCoverObject()
{
    this->SortMusicLayer();
    this->RefreshMusicCoverHighlight();

    if (m_animationTimer == tgon::TimerHandle())
    {
        auto engine = tgon::Application::GetEngine();
        auto timerModule = engine->FindModule<tgon::TimerModule>();
        auto timeModule = engine->FindModule<tgon::TimeModule>();
        
        m_animationTimer = timerModule->SetTimer([this, timerModule, timeModule](tgon::TimerHandle timerHandle)
        {
            int j = 0;
            for (int32_t i = m_currSelectedCoverImageIndex; i < m_currSelectedCoverImageIndex + 7; ++i, ++j)
            {
                if (i < 0 || i >= static_cast<int32_t>(m_coverImageObjects.size()))
                {
                    continue;
                }

                auto transform = m_coverImageObjects[i]->GetTransform();
                auto currPos = transform->GetLocalPosition();
                auto newPos = tgon::Lerp(currPos, g_coverImagePositions[j], 10.0f * timeModule->GetTickTime());
                transform->SetLocalPosition(newPos);

                auto currScale = transform->GetLocalScale();
                auto newScale = tgon::Lerp(currScale, g_coverImageScales[j], 10.0f * timeModule->GetTickTime());
                transform->SetLocalScale(newScale);
            }

            auto transform = m_coverImageObjects[m_currSelectedCoverImageIndex + 3]->GetTransform();
            auto currPos = transform->GetLocalPosition();
            if (std::abs(g_coverImagePositions[3].x - currPos.x) < 0.0001f)
            {
                timerModule->ClearTimer(timerHandle);
                m_animationTimer = {};
            }
        }, 0.0f, true);
    }
}

void MusicSelector::RefreshMusicCoverHighlight()
{
    auto transform = m_coverImageObjects[m_currSelectedCoverImageIndex + 3]->GetTransform();
    m_highlight->GetTransform()->SetParent(transform);
}

void MusicSelector::SortMusicLayer()
{
    int32_t j = -3;
    for (int32_t i = 0; i < 7; ++i, ++j)
    {
        if (m_currSelectedCoverImageIndex + i < 0 || m_currSelectedCoverImageIndex + i >= static_cast<int32_t>(m_coverImageObjects.size()))
        {
            continue;
        }

        m_coverImageObjects[m_currSelectedCoverImageIndex + i]->FindComponent<tgon::UISpriteRendererComponent>()->SetSortingLayer(7 - std::abs(j));
    }
}

void MusicSelector::SortMusicList()
{
    for (int32_t i = 0; i < static_cast<int32_t>(m_coverImageObjects.size()); ++i)
    {
        if (i < 0)
        {
            continue;
        }

        auto transform = m_coverImageObjects[i]->GetTransform();
        transform->SetLocalPosition(g_coverImagePositions[std::min(i + 3, 6)]);
        transform->SetLocalScale(g_coverImageScales[std::min(i + 3, 6)]);
    }
}
