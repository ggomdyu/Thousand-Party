#include "Platform/Application.h"
#include "Engine/TimeModule.h"
#include "Engine/AssetModule.h"
#include "Game/UISpriteRendererComponent.h"
#include "Graphics/OpenGL/OpenGLShaderCode.h"

#include "SquareBackground.h"

SquareBackground::SquareBackground() :
    Component(),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>())
{
}

void SquareBackground::Initialize()
{
    this->InitializeSpriteComponent();
}

void SquareBackground::InitializeSpriteComponent()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = owner->AddComponent<tgon::UISpriteRendererComponent>();
    auto material = std::make_shared<tgon::Material>(g_positionColorUVVert, g_uvOffsetFrag);
    spriteRendererComponent->SetMaterial(material);

    auto texture = assetModule->GetResource<tgon::Texture>(u8"Resource/Background/MusicSelectScene/Layer.png");
    texture->SetWrapMode(tgon::WrapMode::Repeat);

    spriteRendererComponent->SetTexture(std::move(texture));
    spriteRendererComponent->SetTextureSize({838, 441});

    m_material = std::move(material);
}

void SquareBackground::Update()
{
    Super::Update();

    m_backgroundXOffset += m_timeModule->GetTickTime() * 0.012f;
    m_material->SetParameter2f("uvOffset", m_backgroundXOffset, 0.0f);
}
