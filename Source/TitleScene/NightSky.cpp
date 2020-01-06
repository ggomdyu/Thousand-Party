#include "TGON.h"
#include "NightSky.h"

NightSky::NightSky() :
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>())
{
}

void NightSky::Initialize()
{
    Super::Initialize();
    
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }
    
    m_transform = gameObject->GetTransform();

    this->InitializeSpriteComponent();
    this->InitializePosition();
}

void NightSky::InitializeSpriteComponent()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = m_gameObject.lock()->AddComponent<tgon::UISpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Background/TitleScene/nightSky.png"));
}

void NightSky::InitializePosition()
{
    auto objectPos = tgon::Vector3(0.0f, 0.0f, 0.0f);
    m_gameObject.lock()->GetTransform()->SetLocalPosition(objectPos);
}

void NightSky::Update()
{
    Super::Update();

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    auto nightSkyNewPos = m_transform->GetLocalPosition();
    if (nightSkyNewPos.x <= -halfWindowWidth + -419.0f)
    {
        nightSkyNewPos = tgon::Vector3(-halfWindowWidth + 1257.0f, halfWindowHeight - 221.0f, 0.0f);
    }

    nightSkyNewPos.x -= 55.0f * m_timeModule->GetTickTime();
    
    m_transform->SetLocalPosition(nightSkyNewPos);
}
