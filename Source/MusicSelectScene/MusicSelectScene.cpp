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
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    tgon::Vector3 texturePosList[] =
    {
        tgon::Vector3(0.0f, 0.0f, 0.0f),
        tgon::Vector3(0.0f, 0.0f, 0.0f),
        tgon::Vector3(-clientSize.width / 2 - 50, clientSize.height / 2 + 250, 0.0f),
        tgon::Vector3(-clientSize.width / 2 + 640.0f, clientSize.height / 2 + 10.0f, 0.0f),
        tgon::Vector3(10000.0f, 0.0f, 0.0f),
    };
    tgon::Vector2 pivotList[] =
    {
        tgon::Vector2(0.5f, 0.5f),
        tgon::Vector2(0.5f, 0.5f),
        tgon::Vector2(0.0f, 0.0f),
        tgon::Vector2(0.0f, 0.0f),
        tgon::Vector2(0.5f, 0.5f),
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
        spriteRendererComponent->SetPivot(pivotList[i]);
        this->AddObject(object);
    }

    m_musicText = this->FindObject("MUSIC");
    m_selectText = this->FindObject("SELECT");
}
    
void MusicSelectScene::Update()
{
    SuperType::Update();

    // MUSIC
    auto musicTextTransform = m_musicText->GetTransform();
    auto musicTextPos = musicTextTransform->GetLocalPosition();
    if (musicTextPos.x <= -270.0f)
    {
        auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
        musicTextTransform->SetLocalPosition(tgon::Vector3(-clientSize.width / 2 - 50.0f, clientSize.height / 2 + 250.0f, 0.0f));
    }
    else
    {
        musicTextPos += tgon::Vector3(-0.221f, 0.203f, 0.0f);
        musicTextTransform->SetLocalPosition(musicTextPos);
    }

    // SELECT
    //if (m_pBack[4]->GetPosition().x <= 418.0F)
    //    m_pBack[4]->SetPosition(D3DXVECTOR2(640.0F, -10.0F));
    //else
    //    m_pBack[4]->Move(D3DXVECTOR2(-0.222F, 0.223F));
}
