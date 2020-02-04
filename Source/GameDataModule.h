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

class AudioSource;
class AssetModule;
class Scene;

} /* namespace tgon */

class LogoScene;
class TitleScene;
class MusicSelectScene;
class MusicPlayScene;
class MusicResultScene;

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
    template <typename _SceneType>
    std::shared_ptr<_SceneType> GetCachedScene() noexcept;

private:
    void InitializeMusicInfos();
    MusicInfo ParseMusicInfo(const std::string& noteDirectory);
    std::vector<NoteInfo> ParseNoteInfo(const std::string& noteDirectory, float bpm, float sync);
    void PreloadMusicCoverTexture(const std::shared_ptr<tgon::AssetModule>& assetModule, const std::string& noteDirectory);
    void PreloadScene();
    
/**@section Variable */
private:
    std::vector<MusicInfo> m_musicInfos;
    std::shared_ptr<LogoScene> m_cachedLogoScene;
    std::shared_ptr<TitleScene> m_cachedTitleScene;
    std::shared_ptr<MusicSelectScene> m_cachedMusicSelectScene;
    std::shared_ptr<MusicPlayScene> m_cachedMusicPlayScene;
    std::shared_ptr<MusicResultScene> m_cachedMusicResultScene;
};

template <>
inline std::shared_ptr<LogoScene> GameDataModule::GetCachedScene() noexcept
{
    return m_cachedLogoScene;
}

template <>
inline std::shared_ptr<TitleScene> GameDataModule::GetCachedScene() noexcept
{
    return m_cachedTitleScene;
}

template <>
inline std::shared_ptr<MusicSelectScene> GameDataModule::GetCachedScene() noexcept
{
    return m_cachedMusicSelectScene;
}

template <>
inline std::shared_ptr<MusicPlayScene> GameDataModule::GetCachedScene() noexcept
{
    return m_cachedMusicPlayScene;
}

template <>
inline std::shared_ptr<MusicResultScene> GameDataModule::GetCachedScene() noexcept
{
    return m_cachedMusicResultScene;
}
