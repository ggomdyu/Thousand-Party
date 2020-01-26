/**
 * @file    NightSky.h
 * @author  ggomdyu
 * @since   10/20/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

namespace tgon
{

class TimeModule;
class Material;

} /* namespace tgon */

class NightSky :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(NightSky)

/**@section Constructor */
public:
    NightSky();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void InitializeSpriteComponent();
    void InitializePosition();
    
/**@section Variable */
private:
    std::shared_ptr<tgon::Transform> m_transform;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::Material> m_material;
    float m_nightSkyXOffset = 0.0f;
};
