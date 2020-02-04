#include <fmt/format.h>

#include "Platform/Application.h"
#include "IO/Path.h"
#include "IO/File.h"
#include "Engine/AssetModule.h"
#include "Engine/AudioModule.h"
#include "Engine/TimeModule.h"
#include "Engine/TimerModule.h"
#include "Engine/InputModule.h"
#include "Math/Mathematics.h"
#include "Game/UISpriteRendererComponent.h"
#include "Game/UITextRendererComponent.h"
#include "Graphics/OpenGL/OpenGLShaderCode.h"

#include "../MultipleSceneModule.h"
#include "../GameDataModule.h"
#include "../MusicSelectScene/MusicSelectScene.h"

#include "ThousandPartyCircle.h"
#include "MusicResultScene.h"

namespace
{

float EaseOutCubic(float t)
{
    return 1 + (--t) * t * t;
}

} /* namespace */

MusicResultScene::MusicResultScene() :
    m_timerModule(tgon::Application::GetEngine()->FindModule<tgon::TimerModule>()),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_inputModule(tgon::Application::GetEngine()->FindModule<tgon::InputModule>())

{
}

void MusicResultScene::Initialize()
{
    Super::Initialize();

    this->InitializeCircles();
    this->InitializeGirl();
    this->InitializeResultText();
    this->InitializeResultInfo();
}

void MusicResultScene::Update()
{
    Super::Update();

    m_elapsedAnimTime += m_timeModule->GetTickTime() * 1.1f;
    m_elapsedAnimTime = std::clamp(m_elapsedAnimTime, 0.0f, 1.0f);

    if (m_elapsedAnimTime >= 1.0f)
    {
        if (m_inputModule->GetKeyboard()->IsKeyDown(tgon::KeyCode::Space))
        {
            auto sceneModule = tgon::Application::GetEngine()->FindModule<MultipleSceneModule>();
            auto gameDataModule = tgon::Application::GetEngine()->FindModule<GameDataModule>();
            sceneModule->ChangeScene(MultipleSceneChangeAnimType::RightToLeftAnim, gameDataModule->GetCachedScene<MusicSelectScene>());
        }
    }
}

void MusicResultScene::OnActivate()
{
    m_elapsedAnimTime = 0.0f;

    m_girl->GetTransform()->SetLocalPosition(m_girlInitialPos);
    m_resultText->GetTransform()->SetLocalPosition(m_resultTextInitialPos);
    m_resultInfo->GetTransform()->SetLocalPosition(m_resultInfoInitialPos);

    this->InitializeGirlAnimTimer();
    this->InitializeResultTextAnimTimer();
    this->InitializeResultInfoAnimTimer();
    this->ResetAllCircles();
}

void MusicResultScene::SetMusicResultInfo(const MusicResultInfo& musicResultInfo)
{
    m_perfectTextComponent->SetText(fmt::format("PERFECT {0}", musicResultInfo.perfectCount));
    m_greatTextComponent->SetText(fmt::format("GREAT {0}", musicResultInfo.greatCount));
    m_goodTextComponent->SetText(fmt::format("GOOD {0}", musicResultInfo.goodCount));
    m_missTextComponent->SetText(fmt::format("MISS {0}", musicResultInfo.missCount));

    auto comboCount = musicResultInfo.perfectCount + musicResultInfo.greatCount + musicResultInfo.goodCount + musicResultInfo.missCount;
    m_maxComboTextComponent->SetText(fmt::format("{0}/{1}", musicResultInfo.maxCombo, comboCount));
}

void MusicResultScene::InitializeResultText()
{
    auto object = tgon::GameObject::Create();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = object->AddComponent<tgon::UISpriteRendererComponent>();
    auto texture = assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicResultScene/RESULT.png");
    spriteRendererComponent->SetTexture(texture);
    spriteRendererComponent->SetPivot({0.0f, 0.0f});
    spriteRendererComponent->SetSortingLayer(1);
    this->AddChild(object);

    m_resultText = object;

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    m_resultTextInitialPos = tgon::Vector3(-halfWindowWidth, halfWindowHeight - 30.0f, 0.0f);
    m_resultTextInitialPos.x -= texture->GetSize().width;
}

void MusicResultScene::InitializeResultInfo()
{
    auto object = tgon::GameObject::Create();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteComponent = object->AddComponent<tgon::UISpriteRendererComponent>();
    auto texture = assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicResultScene/resultBox.png");
    spriteComponent->SetTexture(texture);
    spriteComponent->SetPivot({0.0f, 1.0f});
    spriteComponent->SetSortingLayer(1);
    this->AddChild(object);

    m_resultInfo = object;
    
    // Add Perfect Count
    auto textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumSquareEB.ttf");
    textComponent->SetFontSize(26);
    textComponent->SetRect(tgon::I32Rect(51, 167, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({1.0f, 0.0f, 0.0f, 1.0f});
    textComponent->SetSortingLayer(1);
    m_perfectTextComponent = textComponent;

    // Add Great Count
    textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumSquareEB.ttf");
    textComponent->SetFontSize(26);
    textComponent->SetRect(tgon::I32Rect(51, 139, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({242.0f / 255.0f, 165.0f / 255.0f, 0.0f, 1.0f});
    textComponent->SetSortingLayer(1);
    m_greatTextComponent = textComponent;

    // Add Good Count
    textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumSquareEB.ttf");
    textComponent->SetFontSize(26);
    textComponent->SetRect(tgon::I32Rect(51, 112, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({242.0f / 255.0f, 165.0f / 255.0f, 0.0f, 1.0f});
    textComponent->SetSortingLayer(1);
    m_goodTextComponent = textComponent;

    // Add Miss Count
    textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumSquareEB.ttf");
    textComponent->SetFontSize(26);
    textComponent->SetRect(tgon::I32Rect(51, 84, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetBlendColor({129.0f / 255.0f, 129.0f / 255.0f, 129.0f / 255.0f, 1.0f});
    textComponent->SetSortingLayer(1);
    m_missTextComponent = textComponent;

    // Add Curr/Max Combo Count
    textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumSquareEB.ttf");
    textComponent->SetFontSize(26);
    textComponent->SetRect(tgon::I32Rect(53, 107, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleCenter);
    textComponent->SetBlendColor({129.0f / 255.0f, 129.0f / 255.0f, 129.0f / 255.0f, 1.0f});
    textComponent->SetSortingLayer(1);
    m_maxComboTextComponent = textComponent;

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    m_resultInfoInitialPos = tgon::Vector3(-halfWindowWidth, -halfWindowHeight + 75.0f, 0.0f);
    m_resultInfoInitialPos.x -= texture->GetSize().width;
}

void MusicResultScene::InitializeCircles()
{
    for (int i = 0; i < 8; i++)
    {
        auto object = tgon::GameObject::Create();
        m_circles.push_back(object->AddComponent<ThousandPartyCircle>());
        this->AddChild(object);
    }
}

void MusicResultScene::InitializeGirl()
{
    auto object = tgon::GameObject::Create();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = object->AddComponent<tgon::UISpriteRendererComponent>();
    auto texture = assetModule->GetResource<tgon::Texture>(u8"Resource/Object/MusicResultScene/girl.png");
    spriteRendererComponent->SetTexture(texture);
    spriteRendererComponent->SetPivot({1.0f, 0.5f});
    spriteRendererComponent->SetSortingLayer(1);
    this->AddChild(object);

    m_girl = object;

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    m_girlInitialPos = tgon::Vector3(halfWindowWidth, 0.0f, 0.0f);
    m_girlInitialPos.x += texture->GetSize().width;
}

void MusicResultScene::InitializeGirlAnimTimer()
{
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    m_timerModule->SetTimer([this, clientSize](tgon::TimerHandle timerHandle)
    {
        float destXPos = clientSize.width * 0.5f;
        auto currPos = m_girl->GetTransform()->GetLocalPosition();
        if (std::abs(currPos.x - destXPos) <= 0.0001f)
        {
            currPos.x = destXPos;
            m_girl->GetTransform()->SetLocalPosition(currPos);
            m_timerModule->ClearTimer(timerHandle);
            return;
        }

        auto newXPos = tgon::Lerp(m_girlInitialPos.x, destXPos, EaseOutCubic(m_elapsedAnimTime));
        currPos.x = newXPos;
        m_girl->GetTransform()->SetLocalPosition(currPos);
    }, 0.0f, true);
}

void MusicResultScene::InitializeResultTextAnimTimer()
{
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    m_timerModule->SetTimer([this, clientSize](tgon::TimerHandle timerHandle)
    {
        float destXPos = -clientSize.width * 0.5f + 30.0f;
        auto currPos = m_resultText->GetTransform()->GetLocalPosition();
        if (std::abs(currPos.x - destXPos) <= 0.0001f)
        {
            currPos.x = destXPos;
            m_resultText->GetTransform()->SetLocalPosition(currPos);
            m_timerModule->ClearTimer(timerHandle);
            return;
        }

        auto newXPos = tgon::Lerp(m_resultTextInitialPos.x, destXPos, EaseOutCubic(m_elapsedAnimTime));
        currPos.x = newXPos;
        m_resultText->GetTransform()->SetLocalPosition(currPos);
    }, 0.0f, true);
}

void MusicResultScene::InitializeResultInfoAnimTimer()
{
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    m_timerModule->SetTimer([this, clientSize](tgon::TimerHandle timerHandle)
    {
        float destXPos = -clientSize.width * 0.5f + 20.0f;
        auto currPos = m_resultInfo->GetTransform()->GetLocalPosition();
        if (std::abs(currPos.x - destXPos) <= 0.0001f)
        {
            currPos.x = destXPos;
            m_resultInfo->GetTransform()->SetLocalPosition(currPos);
            m_timerModule->ClearTimer(timerHandle);
            return;
        }

        auto newXPos = tgon::Lerp(m_resultInfoInitialPos.x, destXPos, EaseOutCubic(m_elapsedAnimTime));
        currPos.x = newXPos;
        m_resultInfo->GetTransform()->SetLocalPosition(currPos);
    }, 0.0f, true);
}

void MusicResultScene::ResetAllCircles()
{
    for (auto& circle : m_circles)
    {
        circle->Reset();
    }
}
