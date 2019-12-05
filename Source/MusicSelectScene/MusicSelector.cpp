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
    for (size_t i = 0; i < m_musicCoverImages.size(); ++i)
    {
        m_musicCoverImages[i] = std::make_shared<tgon::GameObject>();
        m_musicCoverImages[i]->Initialize();
        m_musicCoverImages[i]->GetTransform()->SetParent(this->GetTransform());
    }

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

        auto spriteRendererComponent = m_musicCoverImages[i]->AddComponent<tgon::SpriteRendererComponent>();
        spriteRendererComponent->SetTexture(texture);
        spriteRendererComponent->SetTextureSize({230.0f, 230.0f});
        spriteRendererComponent->SetTextureRect({0.0f, 0.0f, 230.0f, 230.0f});
        spriteRendererComponent->SetSortingLayer(2);
    }
}

void MusicSelector::Update()
{
    Super::Update();
    
    this->OnHandleInput();
}

void MusicSelector::OnHandleInput()
{
    auto keyboard = m_inputModule->GetKeyboard();
    if (keyboard->IsKeyDown(tgon::KeyCode::LeftArrow))
    {
        
    }
    else if (keyboard->IsKeyDown(tgon::KeyCode::RightArrow))
    {
        
    }
}
