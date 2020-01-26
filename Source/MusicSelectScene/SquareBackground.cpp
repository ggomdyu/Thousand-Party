#include "Platform/Application.h"
#include "Engine/TimeModule.h"
#include "Engine/AssetModule.h"
#include "Component/UISpriteRendererComponent.h"

#include "SquareBackground.h"

SquareBackground::SquareBackground() :
    Component(),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>())
{
}

void SquareBackground::Initialize()
{
    this->InitializeSpriteComponent();
    this->InitializePosition();
}

void SquareBackground::InitializeSpriteComponent()
{
    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    auto gameObject = weakGameObject.lock();

    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = gameObject->AddComponent<tgon::UISpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Background/MusicSelectScene/Layer.png"));
}

void SquareBackground::InitializePosition()
{
    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    auto gameObject = weakGameObject.lock();

    gameObject->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, 0.0f, 0.0f));
}

void SquareBackground::Update()
{
    Super::Update();

    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    auto gameObject = weakGameObject.lock();

    auto transform = gameObject->GetTransform();
    auto newPos = transform->GetLocalPosition();
    if (newPos.x <= -35.0f)
    {
        newPos = tgon::Vector3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        newPos.x -= 15.0f * m_timeModule->GetTickTime();
    }
    
    transform->SetLocalPosition(newPos);
}
