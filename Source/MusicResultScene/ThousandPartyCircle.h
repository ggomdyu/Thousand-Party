/**
 * @file    ThousandPartyCircle.h
 * @author  ggomdyu
 * @since   10/20/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

namespace tgon
{

class TimeModule;
class UISpriteRendererComponent;

} /* namespace tgon */

class ThousandPartyCircle :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(ThousandPartyCircle)

/**@section Constructor */
public:
    ThousandPartyCircle();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    void Reset();

/**@section Variable */
private:
    float m_moveSpeed;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_bgSpriteComponent;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_textSpriteComponent;
};
