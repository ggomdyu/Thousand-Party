/**
 * @file    FireFly.h
 * @author  ggomdyu
 * @since   10/20/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

namespace tgon
{

class SpriteRendererComponent;

} /* namespace tgon */

class FireFly :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(FireFly)

/**@section Constructor */
public:
    FireFly();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    void Reset();

/**@section Variable */
private:
    bool m_canAdjustOpacity;
    bool m_needToIncreaseOpacity;
    float m_opacityAdjustSpeed;
    float m_moveSpeed;
    std::shared_ptr<tgon::SpriteRendererComponent> m_spriteRendererComponent;
};
