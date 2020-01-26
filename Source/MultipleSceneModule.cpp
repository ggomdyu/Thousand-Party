#include "PrecompiledHeader.h"

#include "MultipleSceneModule.h"

void MultipleSceneModule::Update()
{
    if (m_nextScene != nullptr)
    {
        m_currScene = std::move(m_nextScene);
    }

    if (m_currScene != nullptr)
    {
        m_currScene->Update();
    }
}

void MultipleSceneModule::ChangeScene(MultipleSceneChangeAnimType sceneChangeAnimType, const std::shared_ptr<tgon::GameObject>& scene)
{
    // If there's no scene, then initialize it immediately.
    if (m_currScene == nullptr)
    {
        m_currScene = scene;
        return;
    }

    // Otherwise, the scene will be initialized on next frame.
    m_nextScene = scene;
}
