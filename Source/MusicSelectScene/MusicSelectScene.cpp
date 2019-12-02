#include "PrecompiledHeader.h"

#include "TGON.h"
#include "MusicSelectScene.h"
#include "SquareBackground.h"

MusicSelectScene::MusicSelectScene()
{
    SuperType::Update();
}

void MusicSelectScene::Initialize()
{
    this->CreateSpriteObjects();
}

void MusicSelectScene::CreateSpriteObjects()
{
    auto squareBackground = std::make_shared<SquareBackground>();
    squareBackground->Initialize();
    squareBackground->GetComponent<tgon::SpriteRendererComponent>()->SetSortingLayer(1);
    this->AddObject(squareBackground);

    std::string_view texturePathList[] =
    {
        u8"Resource/Backgrounds/SelectScene/Background.png",
        u8"Resource/Backgrounds/SelectScene/Shadow.png",
        u8"Resource/UI/SelectScene/MUSIC.png",
        u8"Resource/UI/SelectScene/SELECT.png",
        u8"Resource/Backgrounds/SelectScene/HowToPlay.png"
    };
    tgon::Vector3 texturePosList[] =
    {
        tgon::Vector3(0.0f, 0.0f, 0.0f),
        tgon::Vector3(0.0f, 0.0f, 0.0f),
        tgon::Vector3(0.0f, 0.0f, 0.0f),
        tgon::Vector3(-50.0f, -250.0f, 0.0f),
        tgon::Vector3(640.0f, -10.0f, 0.0f),
    };
    
    int32_t sortingLayerList[] = {0, 2, 2, 2, 3};
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = std::make_shared<tgon::GameObject>(tgon::Path::GetFileNameWithoutExtension(texturePathList[i]));
        object->GetTransform()->SetLocalPosition(texturePosList[i]);
        
        auto spriteRendererComponent = object->AddComponent<tgon::SpriteRendererComponent>();
        spriteRendererComponent->SetTexture(assetModule->GetTexture(texturePathList[i]));
        spriteRendererComponent->SetSortingLayer(sortingLayerList[i]);
        this->AddObject(object);
    }
}
    
void MusicSelectScene::Update()
{
    SuperType::Update();
}
