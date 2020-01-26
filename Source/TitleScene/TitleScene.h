/**
 * @file    TitleScene.h
 * @author  ggomdyu
 * @since   09/29/2018
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"
#include "Engine/TimerHandle.h"

namespace tgon
{

class InputModule;
class TimeModule;
class UISpriteRendererComponent;

} /* namespace tgon */

class TitleScene :
    public tgon::GameObject
{
public:
    TGON_DECLARE_RTTI(TitleScene)

/**@section Constructor */
public:
    TitleScene();

/**@section Destructor */
public:
    ~TitleScene() override;

/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void CreateNightSkyObject();
    void CreateSpriteObjects();
    void CreateFireFlyObjects();
    void OnHandleInput();
    void DestroyTimer();

/**@section Variable */
private:
    std::shared_ptr<tgon::InputModule> m_inputModule;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_fadeInSpriteRendererComponent;
    std::shared_ptr<tgon::GameObject> m_girl;
    tgon::TimerHandle m_fadeInTimerHandle;
    tgon::TimerHandle m_girlMoveTimerHandler;
};
