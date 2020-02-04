#include <fmt/format.h>

#include "Platform/Application.h"
#include "Random/Random.h"
#include "Engine/AssetModule.h"
#include "Engine/TimeModule.h"
#include "Game/UISpriteRendererComponent.h"

#include "ThousandPartyCircle.h"

ThousandPartyCircle::ThousandPartyCircle() :
    Component(),
    m_moveSpeed(static_cast<float>(tgon::Random().NextDouble(30, 60))),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>())
{
}

void ThousandPartyCircle::Initialize()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    m_bgSpriteComponent = owner->AddComponent<tgon::UISpriteRendererComponent>();
    m_textSpriteComponent = owner->AddComponent<tgon::UISpriteRendererComponent>();

    this->Reset();
}

void ThousandPartyCircle::Reset()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto randIndex = tgon::Random().Next(1, 8);
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto bgTexture = assetModule->GetResource<tgon::Texture>(fmt::format(u8"Resource/Object/MusicResultScene/alphaCircle0{0}.png", randIndex));
    m_bgSpriteComponent->SetTexture(bgTexture);

    auto textTexture = assetModule->GetResource<tgon::Texture>(fmt::format(u8"Resource/Object/MusicResultScene/alphaCircle0{0}_text.png", randIndex));
    m_textSpriteComponent->SetTexture(textTexture);

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    auto newPos = tgon::Vector3((float)tgon::Random().NextDouble(-clientSize.width * 0.5f + 100.0f, clientSize.width * 0.5f - 100.0f), -clientSize.height * 0.5f - bgTexture->GetSize().height * 0.5f - (float)tgon::Random().NextDouble(0.0f, 30.0f), 0.0f);
    owner->FindComponent<tgon::Transform>()->SetLocalPosition(newPos);
}


void ThousandPartyCircle::Update()
{
    Super::Update();

    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    if (owner->GetTransform()->GetLocalPosition().y > clientSize.height * 0.5f + 62.0f)
    {
        this->Reset();
        return;
    }

    auto currPos = owner->GetTransform()->GetLocalPosition();
    currPos.y += m_timeModule->GetTickTime() * m_moveSpeed;
    owner->GetTransform()->SetLocalPosition(currPos);
}
