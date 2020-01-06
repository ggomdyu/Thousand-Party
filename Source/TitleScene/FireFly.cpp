#include "TGON.h"
#include "FireFly.h"

FireFly::FireFly() :
    Component(),
    m_canAdjustOpacity((tgon::Random().Next(0, 2) == 0) ? true : false),
    m_needToIncreaseOpacity(tgon::Random().Next(0, 1) == 0),
    m_opacityAdjustSpeed(static_cast<float>(tgon::Random().NextDouble(0.1, 0.4))),
    m_moveSpeed(static_cast<float>(tgon::Random().NextDouble(30, 60)))
{
}

void FireFly::Initialize()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    auto transform = gameObject->FindComponent<tgon::Transform>();
    auto scale = static_cast<float>(tgon::Random().NextDouble(0.3, 1.0));
    transform->SetLocalScale(tgon::Vector3(scale, scale, 1.0f));
    transform->SetLocalPosition(tgon::Vector3(static_cast<float>(tgon::Random().NextDouble(-clientSize.width / 2, clientSize.width / 2)), static_cast<float>(tgon::Random().NextDouble(-clientSize.height / 2 - 80.0f, clientSize.height / 2)), 0.0f));
    
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    m_spriteRendererComponent = gameObject->AddComponent<tgon::UISpriteRendererComponent>();
    m_spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Object/TitleScene/Firefly.png"));
    m_spriteRendererComponent->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, static_cast<float>(tgon::Random().NextDouble(0.4, 1.0))));
    m_spriteRendererComponent->SetSortingLayer(tgon::Random().Next(0, 2) == 0 ? 1 : 2);
}

void FireFly::Reset()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }

    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();
    auto transform = gameObject->FindComponent<tgon::Transform>();
    auto scale = static_cast<float>(tgon::Random().NextDouble(0.3, 1.0));
    transform->SetLocalScale(tgon::Vector3(scale, scale, 1.0f));
    transform->SetLocalPosition(tgon::Vector3(static_cast<float>(tgon::Random().NextDouble(-windowSize.width / 2, windowSize.width / 2)), static_cast<float>(-windowSize.height / 2 - m_spriteRendererComponent->GetTexture()->GetSize().height / 2), 0.0f));
    
    m_canAdjustOpacity = (tgon::Random().Next(0, 2) == 0) ? true : false;
    m_opacityAdjustSpeed = static_cast<float>(tgon::Random().NextDouble(0.0625, 0.25));
    m_moveSpeed = static_cast<float>(tgon::Random().NextDouble(30, 60));
    
    m_spriteRendererComponent->SetSortingLayer(tgon::Random().Next(0, 2) == 0 ? 1 : 2);
    m_spriteRendererComponent->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, static_cast<float>(tgon::Random().NextDouble(0.4, 1.0))));
}

void FireFly::Update()
{
    Super::Update();

    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }

    auto tickTime = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()->GetTickTime();

    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();
    auto transform = gameObject->FindComponent<tgon::Transform>();
    auto newPos = transform->GetLocalPosition();
    if (newPos.y >= windowSize.height / 2 + 123.0F)
    {
        Reset();
    }
    else
    {
        newPos.y += m_moveSpeed * tickTime;
        transform->SetLocalPosition(newPos);
    }

    if (m_canAdjustOpacity)
    {
        float alpha = m_spriteRendererComponent->GetBlendColor().a;
        if (alpha >= 1.0f)
        {
            m_needToIncreaseOpacity = false;
        }
        else if (alpha <= 0.0f)
        {
            m_needToIncreaseOpacity = true;
        }

        alpha = (m_needToIncreaseOpacity) ? alpha + m_opacityAdjustSpeed * tickTime : alpha - m_opacityAdjustSpeed * tickTime;
        m_spriteRendererComponent->SetBlendColor(tgon::Color4f(1.0f, 1.0f, 1.0f, alpha));
    }
}
