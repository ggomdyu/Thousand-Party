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
class Material;

}; /* namespace tgon */

class MusicLeftTimeUI :
    public tgon::Component
{
/**@section Method */
public:
    void Initialize() override;
    void SetProgress(float progress) noexcept;
    float GetProgress() const noexcept;

private:
    void InitialziePosition();
    void InitializeBGImage();
    void InitializeCircleImage();

private:
    float m_progress = {};
    float m_leftTimeBGWidth = {};
    std::shared_ptr<tgon::Material> m_leftTimeBGMaterial;
    std::shared_ptr<tgon::GameObject> m_leftTimeCircle;
};
