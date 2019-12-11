#include "PrecompiledHeader.h"

#include "TGON.h"
#include "GameDataModule.h"

GameDataModule::GameDataModule() :
    m_assetModule(tgon::Application::GetEngine()->FindModule<tgon::AssetModule>())
{
}

void GameDataModule::Initialize()
{
    Super::Initialize();
    
    this->InitializeMusicInfos();
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
        
        this->PreloadMusicCoverTexture(noteDirectory);
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
        
        auto divisorIndex = fileLineData.find(':');
        if (divisorIndex == std::string::npos)
        {
            continue;
        }
        
        fileLineData[divisorIndex] = '\0';
        float hitTime = (1.0f / bitPerSeconds / 8.0f) * atof(&fileLineData[0]) + sync;

        for (size_t j = 0; j < 4; ++j)
        {
            auto ch = fileLineData[divisorIndex + 1 + j];
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
                size_t holdEndLineDataIndex = i;
                size_t holdEndLineDataDivisorIndex = 0;
                for (size_t k = i + 1; k < fileLineDatas.value().size(); ++k)
                {
                    auto& holdLineData = fileLineDatas.value()[k];
                    size_t divisorIndex2 = holdLineData.find(':');
                    if (holdLineData[divisorIndex2 + 1 + j] != '2')
                    {
                        break;
                    }

                    holdEndLineDataDivisorIndex = divisorIndex2;
                    holdEndLineDataIndex = k;
                }

                if (holdEndLineDataIndex == i)
                {
                    continue;
                }

                auto& holdEndLineData = fileLineDatas.value()[holdEndLineDataIndex];
                holdEndLineData[holdEndLineDataDivisorIndex] = '\0';
                float holdEndTime = (1.0f / bitPerSeconds / 8.0f) * atof(&holdEndLineData[0]) + sync;
                holdEndLineData[holdEndLineDataDivisorIndex] = ':';

                ret.push_back(NoteInfo{static_cast<int32_t>(j), hitTime, holdEndTime - hitTime});
            }
        }
    }
    
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

void GameDataModule::PreloadMusicCoverTexture(const std::string& noteDirectory)
{
    auto pngCoverPath = noteDirectory + "/cover.png";
    if (tgon::File::Exists(pngCoverPath.c_str()))
    {
        m_assetModule->GetTexture(pngCoverPath);
        return;
    }
    
    auto jpgCoverPath = noteDirectory + "/cover.jpg";
    if (tgon::File::Exists(jpgCoverPath.c_str()))
    {
        m_assetModule->GetTexture(jpgCoverPath);
        return;
    }
}
