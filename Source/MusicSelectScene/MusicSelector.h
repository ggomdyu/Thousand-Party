/**
 * @file    MusicSelector.h
 * @author  ggomdyu
 * @since   12/03/2019
 */

#pragma once
#include <memory>
#include <array>

#include "Game/GameObject.h"
#include "Engine/TimerHandle.h"
#include "Core/Delegate.h"

namespace tgon
{

class TimeModule;
class InputModule;

} /* namespace tgon */

class GameDataModule;
struct MusicInfo;

class MusicSelector :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(MusicSelector)

/**@section Constructor */
public:
    MusicSelector();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void InitializeMusicCoverObjects();
    void InitializeHighlightObject();
    void OnHandleInput();
    void AnimateMusicCoverObject();
    void RefreshMusicCoverHighlight();
    void SortMusicList();
    void SortMusicLayer();

/**@section Variable */
public:
    tgon::Delegate<void(const MusicInfo&)> OnChangeSelectedMusic;
    
/**@section Variable */
private:
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::InputModule> m_inputModule;
    std::shared_ptr<GameDataModule> m_gameDataModule;
    std::shared_ptr<tgon::GameObject> m_highlight;
    std::vector<std::shared_ptr<tgon::GameObject>> m_coverImageObjects;
    tgon::TimerHandle m_animationTimer;
    int32_t m_currSelectedCoverImageIndex = -3;
};
