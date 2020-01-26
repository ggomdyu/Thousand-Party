/**
 * @file    MultipleSceneModule.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include <unordered_map>

#include "Engine/SceneModule.h"

enum class MultipleSceneChangeAnimType
{
    NoAnim,
    LeftToRightAnim,
};

class MultipleSceneModule :
    public tgon::SceneModule
{
public:
    TGON_DECLARE_RTTI(MultipleSceneModule)

/**@section Method */
public:
    void Update() override;
    void ChangeScene(MultipleSceneChangeAnimType sceneChangeAnimType, const std::shared_ptr<tgon::GameObject>& scene);

/**@section Variable */
private:
    MultipleSceneChangeAnimType m_sceneChangeAnimType;
};