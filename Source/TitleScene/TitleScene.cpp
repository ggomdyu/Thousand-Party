#include "PrecompiledHeader.h"

#include "../MusicSelectScene/MusicSelectScene.h"

#include "TGON.h"
#include "TitleScene.h"
#include "FireFly.h"
#include "NightSky.h"

TitleScene::TitleScene()
{
}

void TitleScene::Update()
{
    SuperType::Update();

    this->OnHandleInput();

    auto blendColor = m_fadeInSpriteRendererComponent->GetBlendColor();
    blendColor.a -= 1.0f * m_timeModule->GetTickTime();
    m_fadeInSpriteRendererComponent->SetBlendColor(blendColor);
}

void TitleScene::Initialize()
{
    m_inputModule = tgon::Application::GetEngine()->FindModule<tgon::InputModule>();
    m_timeModule = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>();
    
    this->InitializeGraphics();
    
    this->CreateSpriteObjects();
    this->CreateTextObjects();
    this->CreateFireFlyObjects();
}

void TitleScene::InitializeGraphics()
{
    auto engine = tgon::Application::GetInstance().GetEngine();
    
    auto graphicsModule = engine->FindModule<tgon::GraphicsModule>();
    graphicsModule->GetGraphics().DisableDepthTest();
}

void TitleScene::CreateSpriteObjects()
{
    auto nightSky = std::make_shared<NightSky>();
    nightSky->Initialize();
    this->AddObject(nightSky);

    std::string_view texturePathList[] =
    {
        u8"Resource/Backgrounds/TitleScene/star.png",
        u8"Resource/Backgrounds/TitleScene/earth.png",
        u8"Resource/Backgrounds/TitleScene/girl.png",
        u8"Resource/UI/TitleScene/Logo.png",
        u8"Resource/UI/TitleScene/Press.png",
        u8"Resource/UI/Common/FadeInOut.png",
    };
    
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    tgon::Vector3 texturePosList[] =
    {
        tgon::Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 360.0f, 0.0f),
        tgon::Vector3(-halfWindowWidth + 612.0f, halfWindowHeight - 388.0f, 0.0f),
        tgon::Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 221.0f, 0.0f),
        tgon::Vector3(-halfWindowWidth + 172.5f, halfWindowHeight - 92.5f, 0.0f),
        tgon::Vector3(23.0f, 355.0f, 0.0f),
        tgon::Vector3(0.0f, 0.0f, 0.0f),
    };
    
    int32_t sortingLayerList[] = {0, 0, 2, 2, 2, 4};
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = std::make_shared<tgon::GameObject>(tgon::Path::GetFileNameWithoutExtension(texturePathList[i]));
        object->GetTransform()->SetLocalPosition(texturePosList[i]);
        
        auto spriteRendererComponent = object->AddComponent<tgon::SpriteRendererComponent>();
        spriteRendererComponent->SetTexture(assetModule->GetTexture(texturePathList[i]));
        spriteRendererComponent->SetSortingLayer(sortingLayerList[i]);
        this->AddObject(object);
        
        if (object->GetName() == "FadeInOut")
        {
            m_fadeInSpriteRendererComponent = object->GetComponent<tgon::SpriteRendererComponent>();
        }
    }
}

void TitleScene::CreateTextObjects()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();
    
    auto object = std::make_shared<tgon::GameObject>("introSprite1");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(-windowSize.width / 2 + 30.0f, -windowSize.height / 2 + 140.0f, 0.0f));
    
    auto textComponent = object->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Fonts/MaplestoryOTFBold.otf");
    textComponent->SetFontSize(20);
    textComponent->SetText(u8"Press Enter or Space...");
    textComponent->SetRect(tgon::I32Rect(0, 0, 400, 100));
    textComponent->SetTextAlignment(tgon::TextAlignment::LowerLeft);
    textComponent->SetSortingLayer(3);
    
    this->AddObject(object);
}

void TitleScene::CreateFireFlyObjects()
{
    for (int i = 0; i < 15; ++i)
    {
        auto fireFly = std::make_shared<FireFly>(tgon::StringHash(std::to_string(i)));
        fireFly->Initialize();
        this->AddObject(std::move(fireFly));
    }
}

void TitleScene::OnHandleInput()
{
    auto keyboard = m_inputModule->GetKeyboard();
    if (keyboard->IsKeyUp(tgon::KeyCode::Space) || keyboard->IsKeyUp(tgon::KeyCode::Return))
    {
        auto sceneModule = tgon::Application::GetEngine()->FindModule<tgon::SceneModule>();
        sceneModule->ChangeScene<MusicSelectScene>();
    }
}
