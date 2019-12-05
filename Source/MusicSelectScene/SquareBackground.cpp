#include "TGON.h"
#include "SquareBackground.h"

SquareBackground::SquareBackground() :
    GameObject(u8"SquareBackground"),
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
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto spriteRendererComponent = this->AddComponent<tgon::SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Backgrounds/SelectScene/Layer.png"));
}

void SquareBackground::InitializePosition()
{
    this->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, 0.0f, 0.0f));
}

void SquareBackground::Update()
{
    Super::Update();

    auto transform = this->GetTransform();
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
