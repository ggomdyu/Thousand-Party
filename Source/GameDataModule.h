/**
 * @file    GameDataModule.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include <memory>
#include <vector>

#include "Engine/Module.h"

namespace tgon
{

class AudioPlayer;
class AssetModule;

} /* namespace tgon */

struct NoteInfo
{
    int32_t noteIndex = 0;
    float hitTime = 0.0f;
    float holdTime = 0.0f;
};

struct MusicInfo
{
    std::string musicPath;
    std::string musicName;
    std::string musicAuthorName;
    float bpm = 0.0f;
    float sync = 0.0f;
    std::vector<NoteInfo> noteInfos;
    bool isNoteDataWrong = false;
};

class GameDataModule :
    public tgon::Module
{
public:
    TGON_DECLARE_RTTI(GameDataModule)

/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    const std::vector<MusicInfo>& GetMusicInfos() const noexcept;
    
private:
    void InitializeMusicInfos();
    MusicInfo ParseMusicInfo(const std::string& noteDirectory);
    std::vector<NoteInfo> ParseNoteInfo(const std::string& noteDirectory, float bpm, float sync);
    void PreloadMusicCoverTexture(const std::shared_ptr<tgon::AssetModule>& assetModule, const std::string& noteDirectory);
    
/**@section Variable */
private:
    std::vector<MusicInfo> m_musicInfos;
};
