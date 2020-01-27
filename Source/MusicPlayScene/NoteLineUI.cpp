#include "Platform/Application.h"
#include "Game/UISpriteRendererComponent.h"
#include "Engine/AssetModule.h"

#include "NoteLineUI.h"

void NoteLineUI::Initialize()
{
    Super::Initialize();
    this->InitializeLines();
}

tgon::Vector3 NoteLineUI::GetNoteStartPosition(int32_t index) const
{
    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return {};
    }

    auto gameObject = weakGameObject.lock();

    tgon::Vector3 ret = m_lines[index]->GetTransform()->GetLocalPosition() + gameObject->GetTransform()->GetLocalPosition();
    ret.x += 604.0f;
    return ret;
}

tgon::Vector3 NoteLineUI::GetNoteHitPosition(int32_t index) const
{
    auto weakOwner = this->GetGameObject();
    if (weakOwner.expired())
    {
        return {};
    }

    auto owner = weakOwner.lock();

    tgon::Vector3 ret = m_lines[index]->GetTransform()->GetLocalPosition() + owner->GetTransform()->GetLocalPosition();
    ret.x -= 303.0f;
    return ret;
}

void NoteLineUI::InitializeLines()
{
    auto weakGameObject = this->GetGameObject();
    if (weakGameObject.expired())
    {
        return;
    }

    auto gameObject = weakGameObject.lock();

    float yPos[] = {80.0f, 40.0f, 0.0f, -40.0f, -80.0f};
    for (size_t i = 0; i < 5; ++i)
    {
        auto lineObject = tgon::GameObject::Create();
        gameObject->AddChild(lineObject);
        lineObject->GetTransform()->SetLocalPosition(tgon::Vector3(8.0f * i, yPos[i], 0.0f));

        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        auto spriteRendererComponent = lineObject->AddComponent<tgon::UISpriteRendererComponent>();;
        spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Object/MusicPlayScene/ring.png"));
        
        m_lines[i] = lineObject;
    }
}
