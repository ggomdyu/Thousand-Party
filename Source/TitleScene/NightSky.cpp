#include "Platform/Application.h"
#include "Graphics/Texture.h"
#include "Engine/AssetModule.h"
#include "IO/Path.h"
#include "Component/SpriteRendererComponent.h"

#include "NightSky.h"

using namespace tgon;

NightSky::NightSky() :
    GameObject(u8"NightSky"),
    m_timeModule(Application::GetEngine()->FindModule<TimeModule>())
{
}

void NightSky::Initialize()
{
    this->InitializeSpriteComponent();
    this->InitializePosition();
}

void NightSky::InitializeSpriteComponent()
{
    std::string resourceFolderPath = u8"/Users/chajunho/Desktop/Programming/Git/GitHub/Thousand-Party/Resource/";
    std::string texturePath = u8"/Backgrounds/TitleScene/nightSky.png";
    
    auto assetModule = Application::GetEngine()->FindModule<AssetModule>();
    auto texture = assetModule->LoadTexture(texturePath);
    auto sprite = std::make_shared<UISprite>(texture);
    this->AddComponent<SpriteRendererComponent>(sprite);
}

void NightSky::InitializePosition()
{
    auto windowSize = Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = windowSize.width * 0.5f;
    float halfWindowHeight = windowSize.height * 0.5f;
    auto objectPos = Vector3(0.0f, 0.0f, 0.0f);
    this->GetTransform()->SetLocalPosition(objectPos);
}

void NightSky::Update()
{
    auto transform = this->GetTransform();
    
    auto windowSize = Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = windowSize.width * 0.5f;
    float halfWindowHeight = windowSize.height * 0.5f;
    auto nightSkyNewPos = transform->GetLocalPosition();
    if (nightSkyNewPos.x <= -halfWindowWidth + -419.0f)
    {
        nightSkyNewPos = Vector3(-halfWindowWidth + 1257.0f, halfWindowHeight - 221.0f, 0.0f);
    }

    nightSkyNewPos.x -= 55.0f * m_timeModule->GetTickTime();
    
    transform->SetLocalPosition(nightSkyNewPos);
}
