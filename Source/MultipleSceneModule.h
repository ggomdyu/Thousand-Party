/**
 * @file    MultipleSceneModule.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include "Game/Scene.h"
#include "Engine/Module.h"

enum class MultipleSceneChangeAnimType
{
    NoAnim,
    RightToLeftAnim,
};

class MultipleSceneModule :
    public tgon::Module
{
public:
    TGON_DECLARE_RTTI(MultipleSceneModule)

/**@section Method */
public:
    void Update() override;
    void ChangeScene(MultipleSceneChangeAnimType sceneChangeAnimType, const std::shared_ptr<tgon::Scene>& scene);
    bool IsEndChangeSceneAnimation() const noexcept;

/**@section Variable */
private:
    std::shared_ptr<tgon::Scene> m_scene1;
    std::shared_ptr<tgon::Scene> m_scene2;
    float m_animElapsedTime = 1.0f;
    MultipleSceneChangeAnimType m_sceneChangeAnimType;
};