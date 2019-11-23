#include "Random/Random.h"
#include "Platform/Application.h"
#include "Engine/AssetModule.h"

#include "FireFly.h"

using namespace tgon;

FireFly::FireFly(const StringHash& name) :
    GameObject(name),
    m_canAdjustOpacity((Random().Next(0, 2) == 0) ? true : false),
    m_needToIncreaseOpacity(Random().Next(0, 1) == 0),
    m_opacityAdjustSpeed(static_cast<float>(Random().NextDouble(0.1, 0.4))),
    m_moveSpeed(static_cast<float>(Random().NextDouble(30, 60)))
{
}

void FireFly::Initialize()
{
    auto windowSize = Application::GetRootWindow()->GetClientSize();
    auto transform = this->GetComponent<Transform>();
    auto scale = static_cast<float>(Random().NextDouble(0.3, 1.0));
    transform->SetLocalScale(Vector3(scale, scale, 1.0f));
    transform->SetLocalPosition(Vector3(static_cast<float>(Random().NextDouble(-windowSize.width / 2, windowSize.width / 2)), static_cast<float>(Random().NextDouble(-windowSize.height / 2 - 80.0f, windowSize.height / 2)), 0.0f));

    auto assetModule = Application::GetEngine()->FindModule<AssetModule>();
    auto texture = assetModule->GetTexture(u8"Resource/Objects/TitleScene/Firefly.png");
    auto sprite = std::make_shared<UISprite>(texture);
    sprite->SetBlendColor(Color4f(1.0f, 1.0f, 1.0f, static_cast<float>(Random().NextDouble(0.4, 1.0))));
    m_spriteRendererComponent = this->AddComponent<SpriteRendererComponent>(sprite);
    m_spriteRendererComponent->SetSortingLayer(Random().Next(0, 2) == 0 ? 1 : 2);
    m_sprite = m_spriteRendererComponent->GetSprite();
}

void FireFly::Reset()
{   
    m_sprite->SetBlendColor(Color4f(1.0f, 1.0f, 1.0f, static_cast<float>(Random().NextDouble(0.4, 1.0))));

    m_canAdjustOpacity = (Random().Next(0, 2) == 0) ? true : false;
    m_opacityAdjustSpeed = static_cast<float>(Random().NextDouble(0.0625, 0.25));
    m_moveSpeed = static_cast<float>(Random().NextDouble(30, 60));

    auto windowSize = Application::GetRootWindow()->GetClientSize();
    auto transform = this->GetComponent<Transform>();
    auto scale = static_cast<float>(Random().NextDouble(0.3, 1.0));
    transform->SetLocalScale(Vector3(scale, scale, 1.0f));
    transform->SetLocalPosition(Vector3(static_cast<float>(Random().NextDouble(-windowSize.width / 2, windowSize.width / 2)), -windowSize.height / 2 - m_sprite->GetTexture()->GetSize().height / 2, 0.0f));
    
    m_spriteRendererComponent->SetSortingLayer(Random().Next(0, 2) == 0 ? 1 : 2);
}

void FireFly::Update()
{
    SuperType::Update();

    auto tickTime = Application::GetEngine()->FindModule<TimeModule>()->GetTickTime();

    auto windowSize = Application::GetRootWindow()->GetClientSize();
    auto transform = this->GetComponent<Transform>();
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
        float alpha = m_sprite->GetBlendColor().a;
        if (alpha >= 1.0f)
        {
            m_needToIncreaseOpacity = false;
        }
        else if (alpha <= 0.0f)
        {
            m_needToIncreaseOpacity = true;
        }

        alpha = (m_needToIncreaseOpacity) ? alpha + m_opacityAdjustSpeed * tickTime : alpha - m_opacityAdjustSpeed * tickTime;
        m_sprite->SetBlendColor(Color4f(1.0f, 1.0f, 1.0f, alpha));
    }
}
