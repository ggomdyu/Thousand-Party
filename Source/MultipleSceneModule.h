/**
 * @file    MultipleSceneModule.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once

#pragma once
#include <memory>

#include "Game/Scene.h"
#include "Engine/Module.h"

class MultipleSceneModule :
    public tgon::Module
{
public:
    TGON_DECLARE_RTTI(MultipleSceneModule)

/**@section Method */
public:
    enum class ChangeSceneAnimationType
    {
        NoAnim,
        LeftToRightAnim,
    };

/**@section Method */
public:
    void Update() override;
    template <typename _SceneType, typename... _Args>
    void ChangeScene(ChangeSceneAnimationType animType, _Args&&... args);
    void AddGlobalObject(const std::shared_ptr<tgon::GameObject>& object);
    void AddGlobalObject(std::shared_ptr<tgon::GameObject>&& object);
    std::shared_ptr<tgon::GameObject> FindGlobalObject(const tgon::StringViewHash& objectName);
    bool RemoveGlobalObject(const tgon::StringViewHash& objectName);
    void RemoveAllGlobalObject();

/**@section Variable */
private:
    ChangeSceneAnimationType m_changeSceneAnimType;
    std::unique_ptr<tgon::Scene> m_currScene;
    std::unique_ptr<tgon::Scene> m_nextScene;
    std::vector<std::shared_ptr<tgon::GameObject>> m_globalObjectVector;
    std::unordered_map<tgon::StringHash, std::shared_ptr<tgon::GameObject>> m_globalObjectMap;
};

template <typename _SceneType, typename ..._Args>
inline void MultipleSceneModule::ChangeScene(ChangeSceneAnimationType animType, _Args&&... args)
{
    // If there's no scene, then initialize it immediately.
    if (m_currScene == nullptr)
    {
        m_currScene = std::make_unique<_SceneType>(std::forward<_Args>(args)...);
        m_currScene->Initialize();
        return;
    }
    
    // Otherwise, the scene will be initialized on next frame.
    m_nextScene = std::make_unique<_SceneType>(std::forward<_Args>(args)...);
}