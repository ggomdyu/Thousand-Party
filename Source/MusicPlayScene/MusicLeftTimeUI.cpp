#include "Math/Mathematics.h"
#include "Platform/Application.h"
#include "Game/UISpriteRendererComponent.h"
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
    this->InitialziePosition();
    this->InitializeBGImage();
    this->InitializeCircleImage();
}

void MusicLeftTimeUI::InitialziePosition()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }
    
    auto rootWindow = tgon::Application::GetRootWindow();
    owner->GetTransform()->SetLocalPosition(tgon::Vector3(rootWindow->GetClientSize().width * 0.5f - 35.0f, rootWindow->GetClientSize().height * 0.5f - 100.0f, 0.0f));
}

void MusicLeftTimeUI::InitializeBGImage()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }
    
    auto spriteRendererComponent = owner->AddComponent<tgon::UISpriteRendererComponent>();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicPlayScene/leftTimeBar.png"));
    spriteRendererComponent->SetSortingLayer(4);
    spriteRendererComponent->SetPivot(tgon::Vector2(1.0f, 0.5f));
    
    auto leftTimeBGMaterial = std::make_shared<tgon::Material>(g_positionColorUVVert, g_scissorFrag);
    spriteRendererComponent->SetMaterial(leftTimeBGMaterial);

    m_leftTimeBGMaterial = leftTimeBGMaterial;
    m_leftTimeBGWidth = spriteRendererComponent->GetTexture()->GetSize().width;
}

void MusicLeftTimeUI::InitializeCircleImage()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }
    
    m_leftTimeCircle = tgon::GameObject::Create();
    
    auto spriteRendererComponent = m_leftTimeCircle->AddComponent<tgon::UISpriteRendererComponent>();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicPlayScene/leftTimeCircle.png"));
    spriteRendererComponent->SetSortingLayer(4);

    owner->AddChild(m_leftTimeCircle);
    
    this->SetProgress(0.0f);
}

void MusicLeftTimeUI::SetProgress(float progress) noexcept
{
    m_progress = std::clamp(progress, 0.0f, 1.0f);
    m_leftTimeBGMaterial->SetParameter4f("clipUV", m_progress, 0.0f, 1.0f, 1.0f);

    float xPos = tgon::Lerp(-m_leftTimeBGWidth, 0.0f, m_progress);
    m_leftTimeCircle->GetTransform()->SetLocalPosition(tgon::Vector3(xPos, 0.0f, 0.0f));
}

float MusicLeftTimeUI::GetProgress() const noexcept
{
    return m_progress;
}
