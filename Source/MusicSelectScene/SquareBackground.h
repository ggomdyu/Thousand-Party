/**
 * @file    SquareBackground.h
 * @author  ggomdyu
 * @since   12/03/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

namespace tgon
{

class TimeModule;

} /* namespace tgon */

class SquareBackground :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(SquareBackground)

/**@section Constructor */
public:
    SquareBackground();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void InitializeSpriteComponent();

/**@section Variable */
private:
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::Material> m_material;
    float m_backgroundXOffset = 0.0f;
};
