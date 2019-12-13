#include "TGON.h"
#include "NoteLine.h"

NoteLine::NoteLine() :
    GameObject()
{
}

void NoteLine::Initialize()
{
    Super::Initialize();
    this->InitializeLines();
}

void NoteLine::Update()
{
    Super::Update();
}

tgon::Vector3 NoteLine::GetNoteStartPosition(int32_t index) const
{
    tgon::Vector3 ret = m_lines[index]->GetTransform()->GetLocalPosition() + this->GetTransform()->GetLocalPosition();
    ret.x += 604.0f;
    return ret;
}

tgon::Vector3 NoteLine::GetNoteHitPosition(int32_t index) const
{
    tgon::Vector3 ret = m_lines[index]->GetTransform()->GetLocalPosition() + this->GetTransform()->GetLocalPosition();
    ret.x -= 303.0f;
    return ret;
}

void NoteLine::InitializeLines()
{
    float yPos[] = {80.0f, 40.0f, 0.0f, -40.0f, -80.0f};
    for (size_t i = 0; i < 5; ++i)
    {
        auto lineObject = tgon::GameObject::Create();
        lineObject->GetTransform()->SetParent(this->GetTransform());
        lineObject->GetTransform()->SetLocalPosition(tgon::Vector3(8.0f * i, yPos[i], 0.0f));

        auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
        auto spriteRendererComponent = lineObject->AddComponent<tgon::SpriteRendererComponent>();;
        spriteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/MusicPlayScene/ring.png"));
        
        m_lines[i] = lineObject;
    }
}
