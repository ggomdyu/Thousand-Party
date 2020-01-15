/**
 * @file    MusicLeftTimeUI.h
 * @author  ggomdyu
 * @since   01/15/2020
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"
#include "Engine/TimerHandle.h"

namespace tgon
{

class UISpriteRendererComponent;

}; /* namespace tgon */

class MusicLeftTimeUI :
    public tgon::Component
{
/**@section Method */
public:
    void Initialize() override;
    void Update() override;

private:
    void InitializeLeftTimeBGImage();
    void InitializeLeftTimeCircle();

private:
    float m_leftTimeCircleStartXPos = 0.0f;
    float m_leftTimeCircleEndXPos = 0.0f;
    std::shared_ptr<tgon::GameObject> m_leftTimeBG;
    std::shared_ptr<tgon::GameObject> m_leftTimeCircle;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_leftTimeBGRendererComponent;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_leftTimeCircleRendererComponent;
};
