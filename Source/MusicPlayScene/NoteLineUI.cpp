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
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return {};
    }

    auto matWorld = m_lines[index]->GetTransform()->GetWorldMatrix();
    tgon::Vector3 ret = tgon::Vector3(matWorld.m31, matWorld.m32, matWorld.m33);
    ret.x += 604.0f;
    return ret;
}

tgon::Vector3 NoteLineUI::GetNoteHitPosition(int32_t index) const
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return {};
    }

    auto matWorld = m_lines[index]->GetTransform()->GetWorldMatrix();
    tgon::Vector3 ret = tgon::Vector3(matWorld.m31, matWorld.m32, matWorld.m33);
    ret.x -= 303.0f;
    return ret;
}

void NoteLineUI::InitializeLines()
{
    auto owner = this->GetGameObject().lock();
    if (owner == nullptr)
    {
        return;
    }

    float yPos[] = {80.0f, 40.0f, 0.0f, -40.0f, -80.0f};
    for (size_t i = 0; i < 5; ++i)
    {
        auto lineObject = tgon::GameObject::Create();
        owner->AddChild(lineObject);
        lineObject->GetTransform()->SetLocalPosition(tgon::Vector3(8.0f * i, yPos[i], 0.0f));

        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        auto spriteRendererComponent = lineObject->AddComponent<tgon::UISpriteRendererComponent>();;
        spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(u8"Resource/UI/MusicPlayScene/ring.png"));
        
        m_lines[i] = lineObject;
    }
}
