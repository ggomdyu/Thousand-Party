#include "PrecompiledHeader.h"

#include "Platform/Application.h"
#include "Engine/TimerModule.h"

#include "MultipleSceneModule.h"

void MultipleSceneModule::Update()
{
    if (m_currScene != nullptr)
    {
        m_currScene->Update();
    }
 
    if (m_nextScene != nullptr)
    {
        m_nextScene->Update();
    }
}

void MultipleSceneModule::ChangeScene(MultipleSceneChangeAnimType sceneChangeAnimType, const std::shared_ptr<tgon::GameObject>& scene)
{
    if (sceneChangeAnimType == MultipleSceneChangeAnimType::NoAnim)
    {
        m_nextScene = nullptr;
        m_currScene = scene;
    }
    else if (sceneChangeAnimType == MultipleSceneChangeAnimType::RightToLeftAnim)
    {
        m_nextScene = scene;

        auto engine = tgon::Application::GetEngine();
        auto rootWindow = tgon::Application::GetRootWindow();
        m_nextScene->GetTransform()->SetLocalPosition({rootWindow->GetClientSize().width * 0.5f - 200.0f, 0.0f, 0.0f});

        auto timerModule = engine->FindModule<tgon::TimerModule>();
        timerModule->SetTimer([&](tgon::TimerHandle timerHandle)
        {
            
        }, 0.0f, true);
    }
}
