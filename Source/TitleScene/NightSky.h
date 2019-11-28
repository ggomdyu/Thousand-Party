/**
 * @file    NightSky.h
 * @author  ggomdyu
 * @since   10/20/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"
#include "Engine/TimeModule.h"

class NightSky :
    public tgon::GameObject
{
public:
    TGON_DECLARE_RTTI(NightSky)

/**@section Constructor */
public:
    NightSky();
    
/**@section Method */
public:
    void Initialize();
    void Update() override;
    
private:
    void InitializeSpriteComponent();
    void InitializePosition();
    
/**@section Variable */
private:
    std::shared_ptr<tgon::TimeModule> m_timeModule;
};
