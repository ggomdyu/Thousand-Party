#include "PrecompiledHeader.h"

#include "TGON.h"
#include "TitleScene.h"
#include "LogoScene.h"

LogoScene::LogoScene()
{
}

void LogoScene::Update()
{
    SuperType::Update();
    
    this->OnHandleInput();

    auto elapsedTime = tgon::Environment::GetTickCount() - m_beginTime;
    if (elapsedTime >= 8500)
    {
        auto gameSceneModule = tgon::Application::GetEngine()->FindModule<tgon::SceneModule>();
        gameSceneModule->ChangeScene<TitleScene>();
    }
    else if (elapsedTime >= 7500)
    {
        {
            auto blendColor = m_logoSpriteRendererComponents[2]->GetBlendColor();
            if (blendColor.a <= 1.0f)
            {
                blendColor.a += 2.5f * m_timeModule->GetTickTime();
                m_logoSpriteRendererComponents[2]->SetBlendColor(blendColor);
            }
        }

        {
            auto blendColor = m_logoSpriteRendererComponents[1]->GetBlendColor();
            if (blendColor.a <= 1.0f)
            {
                blendColor.a += 2.0f * m_timeModule->GetTickTime();
                m_logoSpriteRendererComponents[1]->SetBlendColor(blendColor);
            }
        }
    }
    else if (elapsedTime >= 5000)
    {
        auto blendColor = m_logoSpriteRendererComponents[1]->GetBlendColor();
        if (blendColor.a <= 1.0f)
        {
            blendColor.a += 2.0f * m_timeModule->GetTickTime();
            m_logoSpriteRendererComponents[1]->SetBlendColor(blendColor);
        }
    }
    else if (elapsedTime >= 3500)
    {
        auto blendColor = m_logoSpriteRendererComponents[0]->GetBlendColor();
        if (blendColor.a >= 0.0f)
        {
            blendColor.a -= 2.0f * m_timeModule->GetTickTime();
            m_logoSpriteRendererComponents[0]->SetBlendColor(blendColor);
        }
    }
    else if (elapsedTime >= 1000)
    {
        auto blendColor = m_logoSpriteRendererComponents[0]->GetBlendColor();
        if (blendColor.a <= 1.0f)
        {
            blendColor.a += 2.0f * m_timeModule->GetTickTime();
            m_logoSpriteRendererComponents[0]->SetBlendColor(blendColor);
        }
    }
}

void LogoScene::Initialize()
{
    this->InitializeGraphics();
    this->CreateGameObjects();

    m_beginTime = tgon::Environment::GetTickCount();
    m_inputModule = tgon::Application::GetEngine()->FindModule<tgon::InputModule>();
    m_timeModule = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>();
}

void LogoScene::InitializeGraphics()
{
    auto engine = tgon::Application::GetInstance().GetEngine();
    
    auto graphicsModule = engine->FindModule<tgon::GraphicsModule>();
    graphicsModule->GetGraphics().DisableDepthTest();
}

void LogoScene::CreateCameraObject()
{
    auto camera = std::make_shared<tgon::GameObject>("camera1");
    
    auto rootWindowSize = tgon::Application::GetInstance().GetRootWindow()->GetClientSize();
    float halfWidth = static_cast<float>(rootWindowSize.width) * 0.5f;
    float halfHeight = static_cast<float>(rootWindowSize.height) * 0.5f;
    camera->AddComponent<tgon::CameraComponent>(tgon::FRect(-halfWidth, -halfHeight, rootWindowSize.width, rootWindowSize.height), -1.0f, 1024.0f);
    this->AddGlobalObject(camera);
}

void LogoScene::CreateSpriteObject()
{
    const char* texturePathList[] =
    {
        u8"Resource/Backgrounds/IntroScene/Team_logo.png",
        u8"Resource/Backgrounds/IntroScene/ON_logo.png",
        u8"Resource/UI/Common/FadeInOut.png",
    };

    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = std::make_shared<tgon::GameObject>(tgon::StringHash(std::to_string(i)));
        m_logoSpriteRendererComponents[i] = object->AddComponent<tgon::SpriteRendererComponent>();
        m_logoSpriteRendererComponents[i]->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, 0.0f));
        m_logoSpriteRendererComponents[i]->SetTexture(std::make_shared<tgon::Texture>(texturePathList[i], tgon::FilterMode::Bilinear, tgon::WrapMode::Repeat, true, false));
        
        this->AddObject(object);
    }
}

void LogoScene::CreateGameObjects()
{
    this->CreateCameraObject();
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
