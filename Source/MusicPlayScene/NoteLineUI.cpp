#include "Platform/Application.h"
#include "Component/UISpriteRendererComponent.h"
#include "Engine/AssetModule.h"

#include "NoteLineUI.h"

void NoteLineUI::Initialize()
{
    Super::Initialize();
    this->InitializeLines();
}

tgon::Vector3 NoteLineUI::GetNoteStartPosition(int32_t index) const
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return {};
    }

    tgon::Vector3 ret = m_lines[index]->GetTransform()->GetLocalPosition() + gameObject->GetTransform()->GetLocalPosition();
    ret.x += 604.0f;
    return ret;
}

tgon::Vector3 NoteLineUI::GetNoteHitPosition(int32_t index) const
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return {};
    }

    tgon::Vector3 ret = m_lines[index]->GetTransform()->GetLocalPosition() + gameObject->GetTransform()->GetLocalPosition();
    ret.x -= 303.0f;
    return ret;
}

void NoteLineUI::InitializeLines()
{
    auto gameObject = this->GetGameObject();
    if (gameObject == nullptr)
    {
        return;
    }

    float yPos[] = {80.0f, 40.0f, 0.0f, -40.0f, -80.0f};
    for (size_t i = 0; i < 5; ++i)
    {
        auto lineObject = tgon::GameObject::Create();
        lineObject->GetTransform()->SetParent(gameObject->GetTransform());
        lineObject->GetTransform()->SetLocalPosition(tgon::Vector3(8.0f * i, yPos[i], 0.0f));

        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        auto spriteRendererComponent = lineObject->AddComponent<tgon::UISpriteRendererComponent>();;
        spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/Object/MusicPlayScene/ring.png"));
        
        m_lines[i] = lineObject;
    }
}
