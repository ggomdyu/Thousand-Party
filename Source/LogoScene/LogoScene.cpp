#include "PrecompiledHeader.h"

#include "../TitleScene/TitleScene.h"

#include "TGON.h"
#include "LogoScene.h"

LogoScene::LogoScene()
{
}

void LogoScene::Update()
{
    Super::Update();
    
    this->OnHandleInput();

    auto elapsedTime = tgon::Environment::GetTickCount() - m_beginTime;
    if (elapsedTime >= 8500)
    {
        auto sceneModule = tgon::Application::GetEngine()->FindModule<tgon::SceneModule>();
        sceneModule->ChangeScene<TitleScene>();
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
    m_beginTime = tgon::Environment::GetTickCount();
    m_inputModule = tgon::Application::GetEngine()->FindModule<tgon::InputModule>();
    m_timeModule = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>();
    
    this->InitializeGraphics();
    this->CreateGameObjects();
    this->PreloadResources();
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
    
    auto clientSize = tgon::Application::GetInstance().GetRootWindow()->GetClientSize();
    float halfWidth = static_cast<float>(clientSize.width) * 0.5f;
    float halfHeight = static_cast<float>(clientSize.height) * 0.5f;
    camera->AddComponent<tgon::CameraComponent>(tgon::FRect(-halfWidth, -halfHeight, clientSize.width, clientSize.height), -1.0f, 1024.0f);
    
    tgon::Application::GetEngine()->FindModule<tgon::SceneModule>()->AddGlobalObject(camera);
}

void LogoScene::CreateSpriteObject()
{
    const char* texturePathList[] =
    {
        u8"Resource/Background/IntroScene/Team_logo.png",
        u8"Resource/Background/IntroScene/ON_logo.png",
        u8"Resource/UI/Common/FadeInOut.png",
    };

    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = std::make_shared<tgon::GameObject>(tgon::StringHash(std::to_string(i)));
        object->Initialize();
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

void LogoScene::PreloadResources()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    
    auto noteDirectories = tgon::Directory::GetDirectories("Note");
    for (const auto& directory : noteDirectories)
    {
        auto pngCoverPath = directory + "/cover.png";
        if (tgon::File::Exists(pngCoverPath.c_str()))
        {
            assetModule->GetTexture(pngCoverPath);
            continue;
        }
        
        auto jpgCoverPath = directory + "/cover.jpg";
        if (tgon::File::Exists(jpgCoverPath.c_str()))
        {
            assetModule->GetTexture(jpgCoverPath);
            continue;
        }
    }
}
