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
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    auto objectPos = tgon::Vector3(0.0f, 0.0f, 0.0f);
    this->GetTransform()->SetLocalPosition(objectPos);
}

void SquareBackground::Update()
{
    SuperType::Update();

    auto transform = this->GetTransform();
    
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    auto newPos = transform->GetLocalPosition();
    if (newPos.x <= -halfWindowWidth + -419.0f)
    {
        newPos = tgon::Vector3(-halfWindowWidth + 1257.0f, halfWindowHeight - 221.0f, 0.0f);
    }

    newPos.x -= 15.0f * m_timeModule->GetTickTime();
    
    transform->SetLocalPosition(newPos);
}
