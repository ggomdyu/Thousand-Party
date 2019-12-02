/**
 * @file    RenderTestScene.h
 * @author  ggomdyu
 * @since   11/08/2019
 */

#pragma once
#include <memory>

#include "Game/Scene.h"
#include "Component/SpriteRendererComponent.h"
#include "Engine/InputModule.h"
#include "Engine/TimeModule.h"

class RenderTestScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(RenderTestScene)

/**@section Constructor */
public:
    RenderTestScene();
    
/**@section Method */
public:
    void Update() override;
    void Initialize() override;
    
private:
    void CreateCameraObject();
    void CreateFontObjects();
};
