#include "TGON.h"
#include "MusicSelector.h"

MusicSelector::MusicSelector() :
    GameObject(u8"MusicSelector"),
    m_timeModule(tgon::Application::GetEngine()->FindModule<tgon::TimeModule>()),
    m_inputModule(tgon::Application::GetEngine()->FindModule<tgon::InputModule>())
{
}

void MusicSelector::Initialize()
{
    auto noteDirectories = tgon::Directory::GetDirectories("Note");
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (size_t i = 0; i < noteDirectories.size(); ++i)
    {
        std::shared_ptr<tgon::Texture> texture;
        if (auto pngCoverPath = noteDirectories[i] + "/cover.png"; tgon::File::Exists(pngCoverPath.c_str()))
        {
            texture = assetModule->GetTexture(pngCoverPath);
        }
        else if (auto jpgCoverPath = noteDirectories[i] + "/cover.jpg"; tgon::File::Exists(jpgCoverPath.c_str()))
        {
            texture = assetModule->GetTexture(jpgCoverPath);
        }
        
        m_coverImageTextures.push_back(std::move(texture));
    }
    
    for (size_t i = 0; i < m_coverImageObjectPool.size(); ++i)
    {
        m_coverImageObjectPool[i] = std::make_shared<tgon::GameObject>();
        m_coverImageObjectPool[i]->Initialize();
        m_coverImageObjectPool[i]->GetTransform()->SetParent(this->GetTransform());
        
        auto spriteRendererComponent = m_coverImageObjectPool[i]->AddComponent<tgon::SpriteRendererComponent>();
        spriteRendererComponent->SetTexture(m_coverImageTextures[i]);
    }

    this->SortMusicList();
}

void MusicSelector::Update()
{
    Super::Update();
    
    this->OnHandleInput();
}

void MusicSelector::InitializeHighlightObject()
{
    m_highlight = std::make_shared<tgon::GameObject>();
    
    auto spriteRendererComponent = m_highlight->AddComponent<tgon::SpriteRendererComponent>();
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    spriteRendererComponent->SetTexture(assetModule->GetTexture(u8"Resource/UI/MusicSelectScene/highlight.png"));
}

void MusicSelector::OnHandleInput()
{
    auto keyboard = m_inputModule->GetKeyboard();
    if (keyboard->IsKeyDown(tgon::KeyCode::LeftArrow))
    {
        if (m_currSelectedCoverImageIndex > -2)
        {
            this->MoveToLeftMusic();
            --m_currSelectedCoverImageIndex;
            this->SortMusicList();
        }
    }
    else if (keyboard->IsKeyDown(tgon::KeyCode::RightArrow))
    {
        if (m_currSelectedCoverImageIndex < static_cast<int32_t>(m_coverImageTextures.size()) - 1)
        {
            this->MoveToRightMusic();
            ++m_currSelectedCoverImageIndex;
            this->SortMusicList();
        }
    }
}

constexpr tgon::Vector3 coverImagePositions[] = {
    tgon::Vector3(-280.0f, 0.0f, 0.0f),
    tgon::Vector3(-180.0f, 0.0f, 0.0f),
    tgon::Vector3(0.0f, 0.0f, 0.0f),
    tgon::Vector3(180.0f, 0.0f, 0.0f),
    tgon::Vector3(280.0f, 0.0f, 0.0f),
};
constexpr tgon::Vector3 coverImageScales[] = {
    tgon::Vector3(0.6f, 0.6f, 1.0f),
    tgon::Vector3(0.75f, 0.75f, 1.0f),
    tgon::Vector3(1.0f, 1.0f, 1.0f),
    tgon::Vector3(0.75f, 0.75f, 1.0f),
    tgon::Vector3(0.6f, 0.6f, 1.0f),
};

void MusicSelector::MoveToLeftMusic()
{
    auto engine = tgon::Application::GetEngine();
    auto timerModule = engine->FindModule<tgon::TimerModule>();
    auto timeModule = engine->FindModule<tgon::TimeModule>();
    m_animationTimer = timerModule->SetTimer([this, timerModule, timeModule]()
    {
//        timerModule->ClearTimer(m_animationTimer);
//        m_animationTimer = {};
    }, 0.0f, true);
}

void MusicSelector::MoveToRightMusic()
{
}

void MusicSelector::SortMusicList()
{
    int j = 0;
    for (int32_t i = m_currSelectedCoverImageIndex; i <= static_cast<int32_t>(m_coverImageObjectPool.size()) - 2; ++i, ++j)
    {
        if (i < 0)
        {
            continue;
        }
        
        auto spriteRendererComponent = m_coverImageObjectPool[i]->FindComponent<tgon::SpriteRendererComponent>();
        spriteRendererComponent->SetTextureSize({230.0f, 230.0f});
        spriteRendererComponent->SetTextureRect({0.0f, 0.0f, 230.0f, 230.0f});
        spriteRendererComponent->SetSortingLayer(4 - std::abs(2 - j));
        
        auto transform = m_coverImageObjectPool[i]->GetTransform();
        transform->SetLocalPosition(coverImagePositions[j]);
        transform->SetLocalScale(coverImageScales[j]);
    }
}
