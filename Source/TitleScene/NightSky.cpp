#include "TGON.h"
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
    auto assetModule = Application::GetEngine()->FindModule<AssetModule>();
    auto spriteRendererComponent = this->AddComponent<SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Backgrounds/TitleScene/nightSky.png"));
}

void NightSky::InitializePosition()
{
    auto objectPos = Vector3(0.0f, 0.0f, 0.0f);
    this->GetTransform()->SetLocalPosition(objectPos);
}

void NightSky::Update()
{
    SuperType::Update();

    auto transform = this->GetTransform();
    
    auto clientSize = Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    auto nightSkyNewPos = transform->GetLocalPosition();
    if (nightSkyNewPos.x <= -halfWindowWidth + -419.0f)
    {
        nightSkyNewPos = Vector3(-halfWindowWidth + 1257.0f, halfWindowHeight - 221.0f, 0.0f);
    }

    nightSkyNewPos.x -= 55.0f * m_timeModule->GetTickTime();
    
    transform->SetLocalPosition(nightSkyNewPos);
}
