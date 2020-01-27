#include "PrecompiledHeader.h"

#include "Game/Scene.h"
#include "Engine/AssetModule.h"
#include "Platform/Application.h"
#include "IO/Path.h"
#include "IO/Directory.h"

#include "LogoScene/LogoScene.h"
#include "TitleScene/TitleScene.h"
#include "MusicSelectScene/MusicSelectScene.h"
#include "GameDataModule.h"

void GameDataModule::Initialize()
{
    Super::Initialize();
    
    this->InitializeMusicInfos();
    this->PreloadScene();
}

void GameDataModule::Update()
{
    Super::Update();
}

void GameDataModule::InitializeMusicInfos()
{
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (auto& noteDirectory : tgon::Directory::GetDirectories("Note"))
    {
        auto musicInfo = this->ParseMusicInfo(noteDirectory);
        musicInfo.noteInfos = this->ParseNoteInfo(noteDirectory, musicInfo.bpm, musicInfo.sync);
        m_musicInfos.push_back(std::move(musicInfo));
        
        this->PreloadMusicCoverTexture(assetModule, noteDirectory);
    }
}

const std::vector<MusicInfo>& GameDataModule::GetMusicInfos() const noexcept
{
    return m_musicInfos;
}

std::vector<NoteInfo> GameDataModule::ParseNoteInfo(const std::string& noteDirectory, float bpm, float sync)
{
    auto noteFilePath = noteDirectory + "/note.txt";
    auto fileLineDatas = tgon::File::ReadAllLines(noteFilePath.c_str());
    if (fileLineDatas.has_value() == false)
    {
        return {};
    }
    
    std::vector<NoteInfo> ret;
    double bitPerSeconds = bpm / 60.0f;
    
    for (size_t i = 0; i < fileLineDatas.value().size(); ++i)
    {
        auto& fileLineData = fileLineDatas.value()[i];
        if (fileLineData[0] == '#')
        {
            continue;
        }
        
        float hitTime = (1.0f / bitPerSeconds / 8.0f) * atof(&fileLineData[6]) + sync;

        for (size_t j = 0; j < 5; ++j)
        {
            auto ch = fileLineData[j];
            if (ch == '0')
            {
                continue;
            }
            else if (ch == '1')
            {
                ret.push_back(NoteInfo{static_cast<int32_t>(j), hitTime, 0.0f});
            }
            else if (ch == '2')
            {
                if (i > 0 && fileLineDatas.value()[i - 1][j] == '2')
                {
                    continue;
                }
                
                size_t holdEndLineDataIndex = i;
                for (size_t k = i + 1; k < fileLineDatas.value().size(); ++k)
                {
                    auto& holdLineData = fileLineDatas.value()[k];
                    if (holdLineData[0] == '#')
                    {
                        continue;
                    }

                    if (holdLineData[j] != '2')
                    {
                        break;
                    }

                    holdEndLineDataIndex = k;
                }

                if (holdEndLineDataIndex == i)
                {
                    continue;
                }

                auto& holdEndLineData = fileLineDatas.value()[holdEndLineDataIndex];
                float holdEndTime = (1.0f / bitPerSeconds / 8.0f) * atof(&holdEndLineData[6]) + sync;

                ret.push_back(NoteInfo{static_cast<int32_t>(j), hitTime, holdEndTime - hitTime});
            }
        }
    }
    
    std::sort(ret.begin(), ret.end(), [](const NoteInfo& lhs, const NoteInfo& rhs)
    {
        return lhs.hitTime < rhs.hitTime;
    });
    
    return ret;
}

MusicInfo GameDataModule::ParseMusicInfo(const std::string& noteDirectory)
{
    auto infoFilePath = noteDirectory + "/info.txt";
    auto fileLineDatas = tgon::File::ReadAllLines(infoFilePath.c_str());
    if (fileLineDatas.has_value() == false)
    {
        return {};
    }
    
    std::string musicName;
    std::string musicAuthorName;
    float bpm = 0.0f;
    float sync = 0.0f;
    for (auto& fileLineData : fileLineDatas.value())
    {
        if (fileLineData.find("music") != std::string::npos)
        {
            auto pos = fileLineData.find("=", 5);
            if (pos != std::string::npos)
            {
                musicName = fileLineData.substr(pos + 1);
            }
        }
        else if (fileLineData.find("artist") != std::string::npos)
        {
            auto pos = fileLineData.find("=", 6);
            if (pos != std::string::npos)
            {
                musicAuthorName = fileLineData.substr(pos + 1);
            }
        }
        else if (fileLineData.find("bpm") != std::string::npos)
        {
            auto pos = fileLineData.find("=", 3);
            if (pos != std::string::npos)
            {
                bpm = atof(fileLineData.substr(pos + 1).c_str());
            }
        }
        else if (fileLineData.find("sync") != std::string::npos)
        {
            auto pos = fileLineData.find("=", 4);
            if (pos != std::string::npos)
            {
                sync = atof(fileLineData.substr(pos + 1).c_str());
            }
        }
    }
    
    std::string musicPath = noteDirectory + "/music.wav";
    if (tgon::File::Exists(musicPath.c_str()) == false)
    {
        musicPath = noteDirectory + "/music.ogg";
    }
    if (tgon::File::Exists(musicPath.c_str()) == false)
    {
        musicPath = noteDirectory + "/music.mp3";
    }
    
    return MusicInfo{std::move(musicPath), std::move(musicName), std::move(musicAuthorName), bpm, sync, {}};
}

void GameDataModule::PreloadMusicCoverTexture(const std::shared_ptr<tgon::AssetModule>& assetModule, const std::string& noteDirectory)
{
    auto pngCoverPath = noteDirectory + "/cover.png";
    if (tgon::File::Exists(pngCoverPath.c_str()))
    {
        assetModule->GetResource<tgon::Texture>(pngCoverPath);
        return;
    }
    
    auto jpgCoverPath = noteDirectory + "/cover.jpg";
    if (tgon::File::Exists(jpgCoverPath.c_str()))
    {
        assetModule->GetResource<tgon::Texture>(jpgCoverPath);
        return;
    }
}

void GameDataModule::PreloadScene()
{
    m_cachedLogoScene = tgon::Scene::Create<LogoScene>();
    m_cachedTitleScene = tgon::Scene::Create<TitleScene>();
    m_cachedMusicSelectScene = tgon::Scene::Create<MusicSelectScene>();
}
