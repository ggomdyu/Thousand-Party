/**
 * @file    MusicScoreScene.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include <memory>

#include "Math/Vector3.h"
#include "Game/Scene.h"

namespace tgon
{

class TimerModule;
class TimeModule;
class InputModule;
class UITextRendererComponent;

} /* namespace tgon */

class ThousandPartyCircle;

struct MusicResultInfo
{
    int32_t perfectCount;
    int32_t greatCount;
    int32_t goodCount;
    int32_t missCount;
    int32_t maxCombo;
};

class MusicResultScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(MusicResultScene)
/**@section Constructor */
public:
    MusicResultScene();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    void OnActivate() override;
    void SetMusicResultInfo(const MusicResultInfo& musicResultInfo);
    
/**@section Variable */
private:
    void InitializeResultText();
    void InitializeResultInfo();
    void InitializeCircles();
    void InitializeGirl();
    void InitializeGirlAnimTimer();
    void InitializeResultTextAnimTimer();
    void InitializeResultInfoAnimTimer();
    void ResetAllCircles();

/**@section Variable */
private:
    float m_elapsedAnimTime = 0.0f;
    std::shared_ptr<tgon::TimerModule> m_timerModule;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::InputModule> m_inputModule;
    std::shared_ptr<tgon::GameObject> m_girl;
    std::shared_ptr<tgon::GameObject> m_resultText;
    std::shared_ptr<tgon::GameObject> m_resultInfo;
    std::shared_ptr<tgon::UITextRendererComponent> m_perfectTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_greatTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_goodTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_missTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_maxComboTextComponent;
    tgon::Vector3 m_girlInitialPos;
    tgon::Vector3 m_resultTextInitialPos;
    tgon::Vector3 m_resultInfoInitialPos;
    std::vector<std::shared_ptr<ThousandPartyCircle>> m_circles;
};
