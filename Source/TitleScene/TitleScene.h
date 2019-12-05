/**
 * @file    TitleScene.h
 * @author  ggomdyu
 * @since   09/29/2018
 */

#pragma once
#include <memory>

#include "Game/Scene.h"
#include "Engine/TimerHandle.h"

namespace tgon
{

class InputModule;
class TimeModule;
class SpriteRendererComponent;

} /* namespace tgon */

class TitleScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(TitleScene)

/**@section Constructor */
public:
    TitleScene();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void InitializeGraphics();
    void CreateNightSkyObject();
    void CreateSpriteObjects();
    void CreateTextObjects();
    void CreateFireFlyObjects();
    void OnHandleInput();

/**@section Variable */
private:
    std::shared_ptr<tgon::InputModule> m_inputModule;
    std::shared_ptr<tgon::SpriteRendererComponent> m_fadeInSpriteRendererComponent;
    tgon::TimerHandle m_fadeInImageTimerHandle;
};
