/**
 * @file    RenderTestScene.h
 * @author  ggomdyu
 * @since   11/08/2019
 */

#pragma once
#include "Game/Scene.h"

class RenderTestScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(RenderTestScene)

/**@section Method */
public:
    void Initialize() override;
    
private:
    void CreateCameraObject();
    void CreateFontObjects();
};
