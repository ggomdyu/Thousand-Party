/**
 * @file    LogoScene.h
 * @author  ggomdyu
 * @since   10/06/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

namespace tgon
{

class InputModule;
class TimeModule;
class UISpriteRendererComponent;

} /* namespace tgon */

class LogoScene :
    public tgon::GameObject
{
public:
    TGON_DECLARE_RTTI(LogoScene)

        LogoScene()
    {
        int n = 3;
    }

/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void CreateGameObjects();
    void CreateSpriteObject();
    void OnHandleInput();

/**@section Variable */
private:
    int64_t m_beginTime = 0;
    std::shared_ptr<tgon::InputModule> m_inputModule;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_logoSpriteRendererComponents[4];
};
