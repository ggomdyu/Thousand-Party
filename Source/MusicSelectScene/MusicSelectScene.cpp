#include "PrecompiledHeader.h"

#include "TGON.h"
#include "MusicSelectScene.h"
#include "MusicSelector.h"
#include "SquareBackground.h"

MusicSelectScene::MusicSelectScene()
{
    Super::Update();
}

void MusicSelectScene::Initialize()
{
    m_timeModule = tgon::Application::GetEngine()->FindModule<tgon::TimeModule>();
    
    this->CreateSpriteObjects();
    this->CreateSquareBackgroundObject();
    this->CreateMusicSelectorObject();
}

void MusicSelectScene::CreateSquareBackgroundObject()
{
    auto squareBackground = std::make_shared<SquareBackground>();
    squareBackground->Initialize();
    squareBackground->FindComponent<tgon::SpriteRendererComponent>()->SetSortingLayer(1);
    this->AddObject(squareBackground);
}

void MusicSelectScene::CreateMusicSelectorObject()
{
    auto musicSelector = std::make_shared<MusicSelector>();
    musicSelector->Initialize();
    this->AddObject(musicSelector);
}

void MusicSelectScene::CreateSpriteObjects()
{
    std::string_view texturePathList[] =
    {
        u8"Resource/Background/MusicSelectScene/Background.png",
        u8"Resource/Background/MusicSelectScene/Shadow.png",
        u8"Resource/UI/MusicSelectScene/MUSIC.png",
        u8"Resource/UI/MusicSelectScene/SELECT.png",
        u8"Resource/Background/MusicSelectScene/HowToPlay.png"
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
        tgon::Vector2(0.0f, 1.0f),
    };
    
    int32_t sortingLayerList[] = {0, 2, 2, 2, 2};
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = std::make_shared<tgon::GameObject>(tgon::Path::GetFileNameWithoutExtension(texturePathList[i]));
        object->Initialize();
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
    Super::Update();

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    
    auto tickTime = m_timeModule->GetTickTime();
    
    // MUSIC
    auto musicTextTransform = m_musicText->GetTransform();
    auto musicTextPos = musicTextTransform->GetLocalPosition();
    if (musicTextPos.x <= -270.0f - clientSize.width / 2)
    {
        musicTextTransform->SetLocalPosition(tgon::Vector3(-clientSize.width / 2 - 50.0f, clientSize.height / 2 + 250.0f, 0.0f));
    }
    else
    {
        musicTextPos += tgon::Vector3(-11.05f * tickTime, -10.15f * tickTime, 0.0f);
        musicTextTransform->SetLocalPosition(musicTextPos);
    }

    // SELECT
    auto selectTextTransform = m_selectText->GetTransform();
    auto selectTextPos = selectTextTransform->GetLocalPosition();
    if (selectTextPos.x <= 418.0f - clientSize.width / 2)
    {
        selectTextTransform->SetLocalPosition(tgon::Vector3(-clientSize.width / 2 + 640.0f, clientSize.height / 2 + 10.0f, 0.0f));
    }
    else
    {
        selectTextPos += tgon::Vector3(-11.1f * tickTime, -11.15f * tickTime, 0.0f);
        selectTextTransform->SetLocalPosition(selectTextPos);
    }
}
