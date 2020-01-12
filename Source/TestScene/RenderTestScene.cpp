#include "Platform/Application.h"
#include "Component/CameraComponent.h"
#include "Component/UITextRendererComponent.h"
#include "Math/Rect.h"
#include "Math/Vector3.h"
#include "Math/Rect.h"
#include "UI/UIText.h"

#include "RenderTestScene.h"

void RenderTestScene::Initialize()
{
    this->CreateCameraObject();
    this->CreateFontObjects();
}

void RenderTestScene::CreateCameraObject()
{
    auto camera = tgon::GameObject::Create("camera1");

    auto rootWindowSize = tgon::Application::GetInstance().GetRootWindow()->GetClientSize();
    float halfWidth = static_cast<float>(rootWindowSize.width) * 0.5f;
    float halfHeight = static_cast<float>(rootWindowSize.height) * 0.5f;
    camera->AddComponent<tgon::CameraComponent>(tgon::FRect(-halfWidth, -halfHeight, rootWindowSize.width, rootWindowSize.height), -1.0f, 1024.0f);
    this->AddObject(camera);
}

void RenderTestScene::CreateFontObjects()
{
    const char chArray[] = u8"The problem is finding some way to pass the additional argument to operator new. You can't add a parenthesized argument list after the type name in the new-expression because the compiler will interpret that list as arguments to a constructor, not as arguments to an operator new. That is:";
    
    auto object = tgon::GameObject::Create("introSprite1");
    auto transform = object->GetTransform();
    transform->SetLocalScale({1.0f, 1.0f, 1.0f});
    object->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, 0.0f, 0.0f));
    auto textComponent = object->AddComponent<tgon::UITextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Fonts/MaplestoryOTFBold.otf");
    textComponent->SetFontSize(30);
    textComponent->SetText(chArray);
    textComponent->SetRect(tgon::I32Rect(-200, 100, 400, 200));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleCenter);

    this->AddObject(object);
}
