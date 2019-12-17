#include "TGON.h"
#include "NightSky.h"

NightSky::NightSky() :
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>())
{
    this->InitializeSpriteComponent();
    this->InitializePosition();
}

void NightSky::InitializeSpriteComponent()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = m_gameObject.lock()->AddComponent<tgon::SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Background/TitleScene/nightSky.png"));
}

void NightSky::InitializePosition()
{
    auto objectPos = tgon::Vector3(0.0f, 0.0f, 0.0f);
    m_gameObject.lock()->GetTransform()->SetLocalPosition(objectPos);
}

void NightSky::Update()
{
    Super::Update();

    auto transform = m_gameObject.lock()->GetTransform();
    
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    auto nightSkyNewPos = transform->GetLocalPosition();
    if (nightSkyNewPos.x <= -halfWindowWidth + -419.0f)
    {
        nightSkyNewPos = tgon::Vector3(-halfWindowWidth + 1257.0f, halfWindowHeight - 221.0f, 0.0f);
    }

    nightSkyNewPos.x -= 55.0f * m_timeModule->GetTickTime();
    
    transform->SetLocalPosition(nightSkyNewPos);
}
