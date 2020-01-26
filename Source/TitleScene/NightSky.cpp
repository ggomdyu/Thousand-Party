#include "Platform/Application.h"
#include "Game/UISpriteRendererComponent.h"
#include "Engine/TimeModule.h"
#include "Engine/AssetModule.h"
#include "Graphics/OpenGL/OpenGLShaderCode.h"

#include "NightSky.h"

NightSky::NightSky() :
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>())
{
}

void NightSky::Initialize()
{
    Super::Initialize();
    
    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    auto gameObject = weakGameObject.lock();

    m_transform = gameObject->GetTransform();

    this->InitializeSpriteComponent();
    this->InitializePosition();
}

void NightSky::InitializeSpriteComponent()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = m_gameObject.lock()->AddComponent<tgon::UISpriteRendererComponent>();
    auto material = std::make_shared<tgon::Material>(g_positionColorUVVert, g_uvOffsetFrag);
    spriteRendererComponent->SetMaterial(material);

    auto texture = assetModule->GetResource<tgon::Texture>(u8"Resource/Background/TitleScene/nightSky.png");
    texture->SetWrapMode(tgon::WrapMode::Repeat);
    spriteRendererComponent->SetTexture(std::move(texture));

    m_material = std::move(material);
}

void NightSky::InitializePosition()
{
    auto objectPos = tgon::Vector3(0.0f, 0.0f, 0.0f);
    m_gameObject.lock()->GetTransform()->SetLocalPosition(objectPos);
}

void NightSky::Update()
{
    Super::Update();

    m_nightSkyXOffset += m_timeModule->GetTickTime() * 0.06f;
    m_material->SetParameter2f("uvOffset", m_nightSkyXOffset, 0.0f);
}
