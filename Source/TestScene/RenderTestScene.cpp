#include "TGON.h"
#include "RenderTestScene.h"

using namespace tgon;

std::shared_ptr<GameObject> g_note; 
std::shared_ptr<GameObject> g_ring;
std::shared_ptr<GameObject> g_background;
std::shared_ptr<SpriteRendererComponent> g_noteRendererComponent;
std::shared_ptr<SpriteRendererComponent> g_longNoteRendererComponent;

void InitializeBackground()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto backgroundObject = tgon::GameObject::Create();
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    backgroundObject->GetTransform()->SetLocalPosition({ -static_cast<float>(clientSize.width) * 0.5f, 0.0f, 0.0f });

    auto spriteRendererComponent = backgroundObject->AddComponent<tgon::SpriteRendererComponent>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture("Resource/Background/MusicPlayScene/green.png"));
    spriteRendererComponent->SetPivot({ 0.0f, 0.5f });
    spriteRendererComponent->SetSortingLayer(0);

    g_background = backgroundObject;
}

void InitializeNote()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    auto obj = GameObject::Create();
    g_noteRendererComponent = obj->AddComponent<tgon::SpriteRendererComponent>();
    g_noteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/PlayScene/Note.png"));
    g_noteRendererComponent->SetSortingLayer(1);

    auto ringObject = tgon::GameObject::Create();
    ringObject->GetTransform()->SetParent(obj->GetTransform());
    g_longNoteRendererComponent = ringObject->AddComponent<tgon::SpriteRendererComponent>();
    g_longNoteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/Object/PlayScene/LongNote.png"));
    g_noteRendererComponent->SetSortingLayer(0);

    g_note = obj;
    g_ring = ringObject;
}


RenderTestScene::RenderTestScene()
{
}

void RenderTestScene::Initialize()
{
    auto graphicsModule = Application::GetEngine()->FindModule<GraphicsModule>();
    graphicsModule->GetGraphics().SetClearColor(Color4f(1.0f, 0.5f, 0.5f, 1.0f));
    
    this->CreateCameraObject();

    InitializeBackground();
    InitializeNote();
    this->AddObject(g_note);
}

void RenderTestScene::CreateCameraObject()
{
    auto camera = tgon::GameObject::Create("camera1");

    auto clientSize = tgon::Application::GetInstance().GetRootWindow()->GetClientSize();
    float halfWidth = static_cast<float>(clientSize.width) * 0.5f;
    float halfHeight = static_cast<float>(clientSize.height) * 0.5f;
    camera->AddComponent<tgon::CameraComponent>(tgon::FRect(-halfWidth, -halfHeight, clientSize.width, clientSize.height), -1.0f, 1024.0f);

    this->AddObject(camera);
}

void RenderTestScene::Update()
{
    Super::Update();

    auto keyboard = Application::GetEngine()->FindModule<InputModule>()->GetKeyboard();
    if (keyboard->IsKeyHold(KeyCode::Space))
    {
        auto transform = g_ring->GetTransform();
        auto scale = transform->GetLocalScale();
        scale.x -= 0.01f;
        scale.y -= 0.01f;
        transform->SetLocalScale(scale);
    }

    auto transform = g_note->GetTransform();
    if (keyboard->IsKeyHold(KeyCode::LeftArrow))
    {
        auto pos = transform->GetLocalPosition();
        pos.x -= 3.0f;
        transform->SetLocalPosition(pos);
    }
    if (keyboard->IsKeyHold(KeyCode::RightArrow))
    {
        auto pos = transform->GetLocalPosition();
        pos.x += 3.0f;
        transform->SetLocalPosition(pos);
    }
    if (keyboard->IsKeyHold(KeyCode::UpArrow))
    {
        auto pos = transform->GetLocalPosition();
        pos.y += 3.0f;
        transform->SetLocalPosition(pos);
    }
    if (keyboard->IsKeyHold(KeyCode::DownArrow))
    {
        auto pos = transform->GetLocalPosition();
        pos.y -= 3.0f;
        transform->SetLocalPosition(pos);
    }
}