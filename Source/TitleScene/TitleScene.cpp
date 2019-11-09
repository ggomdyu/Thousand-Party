#include "TGON.h"

#include "TitleScene.h"
#include "FireFly.h"
#include "NightSky.h"

using namespace tgon;

TitleScene::TitleScene()
{
}

void TitleScene::Update()
{
    SuperType::Update();

    this->OnHandleInput();

    auto blendColor = m_fadeInSprite->GetBlendColor();
    blendColor.a -= 1.0f * m_timeModule->GetTickTime();
    m_fadeInSprite->SetBlendColor(blendColor);
}

void TitleScene::Initialize()
{
    m_inputModule = Application::GetEngine()->FindModule<InputModule>();
    m_timeModule = Application::GetEngine()->FindModule<TimeModule>();
    
    this->InitializeGraphics();
    this->CreateGameObjects();
}

void TitleScene::InitializeGraphics()
{
    auto engine = Application::GetInstance().GetEngine();
    
    auto graphicsModule = engine->FindModule<GraphicsModule>();
    graphicsModule->GetGraphics().DisableDepthTest();
}

void TitleScene::CreateUIObjects()
{
    std::string texturePathList[] =
    {
        u8"Resource/Backgrounds/TitleScene/star.png",
        u8"Resource/Backgrounds/TitleScene/earth.png",
        u8"Resource/Backgrounds/TitleScene/girl.png",
        u8"Resource/UI/TitleScene/Logo.png",
        u8"Resource/UI/TitleScene/Press.png",
        u8"Resource/UI/Common/FadeInOut.png",
    };
    
    auto windowSize = Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = windowSize.width * 0.5f;
    float halfWindowHeight = windowSize.height * 0.5f;
    Vector3 texturePosList[] =
    {
        Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 360.0f, 0.0f),
        Vector3(-halfWindowWidth + 612.0f, halfWindowHeight - 388.0f, 0.0f),
        Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 221.0f, 0.0f),
        Vector3(-halfWindowWidth + 172.5f, halfWindowHeight - 92.5f, 0.0f),
        Vector3(23.0f, 355.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
    };
    int32_t sortingLayerList[] = {0, 0, 2, 2, 2, 3};

    auto nightSky = std::make_shared<NightSky>();
    nightSky->Initialize();
    this->AddObject(nightSky);

    auto assetModule = Application::GetEngine()->FindModule<AssetModule>();
    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto texture = assetModule->LoadTexture(texturePathList[i]);
        auto objectName = StringHash(Path::GetFileNameWithoutExtension(texturePathList[i]));
        auto object = std::make_shared<GameObject>(objectName);
        object->GetTransform()->SetLocalPosition(texturePosList[i]);
        auto spriteComponent = object->AddComponent<SpriteRendererComponent>(std::make_shared<UISprite>(texture));
        spriteComponent->SetSortingLayer(sortingLayerList[i]);
        this->AddObject(object);
    }

    auto fadeInObject = this->FindObject("FadeInOut");
    m_fadeInSprite = fadeInObject->GetComponent<SpriteRendererComponent>()->GetSprite();   
}

void TitleScene::CreateFireFlyObjects()
{
    for (int i = 0; i < 15; ++i)
    {
        auto fireFly = std::make_shared<FireFly>(StringHash(std::to_string(i)));
        fireFly->Initialize();
        this->AddObject(std::move(fireFly));
    }
}

void TitleScene::CreateGameObjects()
{
    this->CreateUIObjects();
    this->CreateFireFlyObjects();
}

void TitleScene::OnHandleInput()
{
}
