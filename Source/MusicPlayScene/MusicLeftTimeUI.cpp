#include "Math/Mathematics.h"
#include "Platform/Application.h"
#include "Component/UISpriteRendererComponent.h"
#include "Engine/InputModule.h"
#include "Engine/AssetModule.h"
#include "Engine/TimeModule.h"
#include "Audio/AudioPlayer.h"
#include "Diagnostics/Debug.h"
#include "Graphics/Material.h"
#include "Graphics/OpenGL/OpenGLShaderCode.h"

#include "MusicLeftTimeUI.h"

void MusicLeftTimeUI::Initialize()
{
    this->InitializeLeftTimeBGImage();
    this->InitializeLeftTimeCircle();
}

void MusicLeftTimeUI::SetProgress(float progress) noexcept
{
    m_progress = std::clamp(progress, 0.0f, 1.0f);
    
    m_leftTimeBGMaterial->SetParameter4f("clipUV", m_progress, 0.0f, 1.0f, 1.0f);
    
    float xPos = tgon::Lerp(m_leftTimeCircleStartXPos, m_leftTimeCircleEndXPos, m_progress);
    m_leftTimeCircle->GetTransform()->SetLocalPosition(tgon::Vector3(xPos, 0.0f, 0.0f));
}

float MusicLeftTimeUI::GetProgress() const noexcept
{
    return m_progress;
}

void MusicLeftTimeUI::InitializeLeftTimeBGImage()
{
    m_leftTimeBG = tgon::GameObject::Create();
    m_leftTimeBG->GetTransform()->SetParent(this->GetGameObject()->GetTransform());

    auto spriteRendererComponent = m_leftTimeBG->AddComponent<tgon::UISpriteRendererComponent>();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicPlayScene/leftTimeBar.png"));
    spriteRendererComponent->SetSortingLayer(4);
    
    auto leftTimeBGMaterial = std::make_shared<tgon::Material>(g_positionColorUVVert, g_scissorFrag);
    spriteRendererComponent->SetMaterial(leftTimeBGMaterial);

    m_leftTimeBGMaterial = leftTimeBGMaterial;
    m_leftTimeBGRendererComponent = spriteRendererComponent;
}

void MusicLeftTimeUI::InitializeLeftTimeCircle()
{
    m_leftTimeCircleStartXPos = -m_leftTimeBGRendererComponent->GetTexture()->GetSize().width / 2;
    m_leftTimeCircleEndXPos = m_leftTimeCircleStartXPos + m_leftTimeBGRendererComponent->GetTexture()->GetSize().width;

    m_leftTimeCircle = tgon::GameObject::Create();

    auto leftTimeCircleTransform = m_leftTimeCircle->GetTransform();
    leftTimeCircleTransform->SetParent(this->GetGameObject()->GetTransform());
    leftTimeCircleTransform->SetLocalPosition(tgon::Vector3(m_leftTimeCircleStartXPos, 0.0f, 0.0f));

    auto spriteRendererComponent = m_leftTimeCircle->AddComponent<tgon::UISpriteRendererComponent>();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicPlayScene/leftTimeCircle.png"));
    spriteRendererComponent->SetSortingLayer(4);

    m_leftTimeBGRendererComponent = spriteRendererComponent;
}
