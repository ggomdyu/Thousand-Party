#include "PrecompiledHeader.h"

#include "TGON.h"
#include "MusicSelectScene.h"
#include "MusicSelector.h"
#include "SquareBackground.h"

#include "../GameDataModule.h"

MusicSelectScene::MusicSelectScene() :
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_gameDataModule(tgon::Application::GetEngine()->FindModule<GameDataModule>())
{
    Super::Update();
}

void MusicSelectScene::Initialize()
{
    this->CreateSpriteObjects();
    this->CreateSquareBackgroundObject();
    this->CreateMusicSelectorObject();
    this->CreateMusicNameObject();
    this->CreateMusicArtistNameObject();
}

void MusicSelectScene::CreateSquareBackgroundObject()
{
    auto squareBackground = tgon::GameObject::Create();
    squareBackground->AddComponent<SquareBackground>();
    squareBackground->FindComponent<tgon::SpriteRendererComponent>()->SetSortingLayer(1);
    this->AddObject(squareBackground);
}

void MusicSelectScene::CreateMusicSelectorObject()
{
    auto musicSelectorObject = tgon::GameObject::Create();
    musicSelectorObject->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, 20.0f, 0.0f));

    auto musicSelectorComponent = musicSelectorObject->AddComponent<MusicSelector>();
    musicSelectorComponent->OnChangeSelectedMusic = tgon::Delegate(&MusicSelectScene::OnChangeSelectedMusic, this);
    
    this->AddObject(musicSelectorObject);
}

void MusicSelectScene::CreateMusicNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, -windowSize.height / 2 + 100.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(27);
    textComponent->SetBlendColor(tgon::Color4f(0.0f, 0.0f, 0.0f, 1.0f));
    textComponent->SetRect(tgon::I32Rect(-250, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleCenter);
    textComponent->SetSortingLayer(4);
    
    auto musicInfos = m_gameDataModule->GetMusicInfos();
    if (musicInfos.size() > 0)
    {
        textComponent->SetText(musicInfos[0].musicName);
    }

    m_musicNameRendererComponent = textComponent;

    this->AddObject(object);
}

void MusicSelectScene::CreateMusicArtistNameObject()
{
    auto windowSize = tgon::Application::GetRootWindow()->GetClientSize();

    auto object = tgon::GameObject::Create(u8"musicArtistName");
    object->GetTransform()->SetLocalPosition(tgon::Vector3(0.0f, -windowSize.height / 2 + 72.0f, 0.0f));

    auto textComponent = object->AddComponent<tgon::TextRendererComponent>();
    textComponent->SetFontAtlas(u8"Resource/Font/NanumBarunGothicBold.otf");
    textComponent->SetFontSize(16);
    textComponent->SetBlendColor(tgon::Color4f(0.0f, 0.0f, 0.0f, 1.0f));
    textComponent->SetRect(tgon::I32Rect(-250, 0, 500, 50));
    textComponent->SetTextAlignment(tgon::TextAlignment::MiddleCenter);
    textComponent->SetSortingLayer(4);
    
    auto musicInfos = m_gameDataModule->GetMusicInfos();
    if (musicInfos.size() > 0)
    {
        textComponent->SetText(musicInfos[0].musicAuthorName);
    }

    m_musicArtistNameRendererComponent = textComponent;

    this->AddObject(object);
}

void MusicSelectScene::OnChangeSelectedMusic(const MusicInfo& musicInfo)
{
    m_musicNameRendererComponent->SetText(musicInfo.musicName);
    m_musicArtistNameRendererComponent->SetText(musicInfo.musicAuthorName);
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
        auto object = tgon::GameObject::Create(tgon::Path::GetFileNameWithoutExtension(texturePathList[i]));
        object->GetTransform()->SetLocalPosition(texturePosList[i]);
        
        auto spriteRendererComponent = object->AddComponent<tgon::SpriteRendererComponent>();
        spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(texturePathList[i]));
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
