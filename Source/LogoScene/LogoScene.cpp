#include <fmt/format.h>

#include "Diagnostics/Debug.h"
#include "Platform/Environment.h"
#include "Platform/Application.h"
#include "Game/UISpriteRendererComponent.h"
#include "Engine/TimeModule.h"
#include "Engine/InputModule.h"

#include "../TitleScene/TitleScene.h"
#include "../GameDataModule.h"
#include "../MultipleSceneModule.h"

#include "LogoScene.h"

using namespace tgon;

void LogoScene::Update()
{
    Super::Update();

    this->OnHandleInput();

    auto elapsedTime = tgon::Environment::GetTickCount() - m_beginTime;
    if (elapsedTime >= 8500)
    {
        auto engine = tgon::Application::GetEngine();

        auto gameDataModule = engine->FindModule<GameDataModule>();
        auto sceneModule = engine->FindModule<MultipleSceneModule>();
        sceneModule->ChangeScene(MultipleSceneChangeAnimType::NoAnim, gameDataModule->GetCachedScene<TitleScene>());
    }
    else if (elapsedTime >= 7500)
    {
        {
            auto blendColor = m_logoSpriteComponents[2]->GetBlendColor();
            if (blendColor.a <= 1.0f)
            {
                blendColor.a += 2.5f * m_timeModule->GetTickTime();
                m_logoSpriteComponents[2]->SetBlendColor(blendColor);
            }
        }

        {
            auto blendColor = m_logoSpriteComponents[1]->GetBlendColor();
            if (blendColor.a <= 1.0f)
            {
                blendColor.a += 2.0f * m_timeModule->GetTickTime();
                m_logoSpriteComponents[1]->SetBlendColor(blendColor);
            }
        }
    }
    else if (elapsedTime >= 5000)
    {
        auto blendColor = m_logoSpriteComponents[1]->GetBlendColor();
        if (blendColor.a <= 1.0f)
        {
            blendColor.a += 2.0f * m_timeModule->GetTickTime();
            m_logoSpriteComponents[1]->SetBlendColor(blendColor);
        }
    }
    else if (elapsedTime >= 3500)
    {
        auto blendColor = m_logoSpriteComponents[0]->GetBlendColor();
        if (blendColor.a >= 0.0f)
        {
            blendColor.a -= 2.0f * m_timeModule->GetTickTime();
            m_logoSpriteComponents[0]->SetBlendColor(blendColor);
        }
    }
    else if (elapsedTime >= 1000)
    {
        auto blendColor = m_logoSpriteComponents[0]->GetBlendColor();
        if (blendColor.a <= 1.0f)
        {
            blendColor.a += 2.0f * m_timeModule->GetTickTime();
            m_logoSpriteComponents[0]->SetBlendColor(blendColor);
        }
    }
}

void LogoScene::Initialize()
{
    m_beginTime = tgon::Environment::GetTickCount();
    m_inputModule = tgon::Application::GetEngine()->FindModule<tgon::InputModule>();
    m_timeModule = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>();
    
    this->CreateGameObjects();
}

void LogoScene::CreateSpriteObject()
{
    const char* texturePathList[] =
    {
        u8"Resource/Background/IntroScene/Team_logo.png",
        u8"Resource/Background/IntroScene/ON_logo.png",
        u8"Resource/UI/Common/board01.png",
    };
    tgon::Vector3 scaleList[] =
    {
        tgon::Vector3(1.0f, 1.0f, 1.0f),
        tgon::Vector3(1.0f, 1.0f, 1.0f),
        tgon::Vector3(858.0f, 462.0f, 1.0f),
    };

    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = tgon::GameObject::Create(tgon::StringHash(std::to_string(i)));
        object->GetTransform()->SetLocalScale(scaleList[i]);

        m_logoSpriteComponents[i] = object->AddComponent<tgon::UISpriteRendererComponent>();
        m_logoSpriteComponents[i]->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, 0.0f));
        m_logoSpriteComponents[i]->SetTexture(std::make_shared<tgon::Texture>(*Image::Create(texturePathList[i]), tgon::FilterMode::Linear, tgon::WrapMode::Repeat, false, false));
        
        this->AddChild(object);
    }
}

void LogoScene::CreateGameObjects()
{
    this->CreateSpriteObject();
}

void LogoScene::OnHandleInput()
{
    decltype(auto) keyboard = m_inputModule->GetKeyboard();
    if (keyboard->IsKeyDown(tgon::KeyCode::Space))
    {
        auto elapsedTime = tgon::Environment::GetTickCount() - m_beginTime;
        if (elapsedTime >= 7500)
        {
        }
        else if (elapsedTime >= 5000)
        {
            m_beginTime -= 7500 - elapsedTime;
        }
        else if (elapsedTime >= 3500)
        {
        }
        else if (elapsedTime >= 1000)
        {
            m_beginTime -= 3500 - elapsedTime;
        }
        else
        {
            m_beginTime -= 1000 - elapsedTime;
        }
    }
}
