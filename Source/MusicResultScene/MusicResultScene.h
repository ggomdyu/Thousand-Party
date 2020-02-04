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

} /* namespace tgon */

class ThousandPartyCircle;

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
    tgon::Vector3 m_girlInitialPos;
    tgon::Vector3 m_resultTextInitialPos;
    tgon::Vector3 m_resultInfoInitialPos;
    std::vector<std::shared_ptr<ThousandPartyCircle>> m_circles;
};
