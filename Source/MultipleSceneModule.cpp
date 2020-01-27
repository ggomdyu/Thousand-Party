#include "PrecompiledHeader.h"

#include "Platform/Application.h"
#include "Engine/TimeModule.h"
#include "Engine/TimerModule.h"
#include "Math/Mathematics.h"

#include "MultipleSceneModule.h"

namespace
{

double EaseOutExpo(float t)
{
    return 1 - std::pow(2, -8.0f * t);
}

double EaseOutBack(double t)
{
    return 1 + (--t) * t * (2.70158 * t + 1.70158);
}

double EaseOutCirc(double t)
{
    return sqrt(t);
}


} /* namespace */

void MultipleSceneModule::Update()
{
    if (m_scene1 != nullptr)
    {
        m_scene1->Update();
    }
 
    if (m_scene2 != nullptr)
    {
        m_scene2->Update();
    }
}

void MultipleSceneModule::ChangeScene(MultipleSceneChangeAnimType sceneChangeAnimType, const std::shared_ptr<tgon::Scene>& scene)
{
    m_sceneChangeAnimType = sceneChangeAnimType;

    scene->OnStart();

    if (sceneChangeAnimType == MultipleSceneChangeAnimType::NoAnim)
    {
        if (m_scene1 != nullptr)
        {
            m_scene1->OnFinish();
        }
        if (m_scene2 != nullptr)
        {
            m_scene2->OnFinish();
        }

        m_scene1 = scene;
        m_scene2 = nullptr;
        return;
    }

    if (m_scene1 == nullptr)
    {
        m_scene1 = scene;
    }
    else if (m_scene2 == nullptr)
    {
        m_scene2 = scene;
    }
    
    if (sceneChangeAnimType == MultipleSceneChangeAnimType::RightToLeftAnim)
    {
        auto engine = tgon::Application::GetEngine();
        auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();

        auto timerModule = engine->FindModule<tgon::TimerModule>();
        auto timeModule = engine->FindModule<tgon::TimeModule>();
        timerModule->SetTimer([&, timeModule, timerModule, clientSize](tgon::TimerHandle timerHandle)
        {
            if (m_scene1 == nullptr || m_scene2 == nullptr)
            {
                return;
            }

            m_animElapsedTime += timeModule->GetTickTime();
            m_animElapsedTime = std::clamp(m_animElapsedTime, 0.0f, 1.0f);

            m_scene1->GetTransform()->SetLocalPosition(tgon::Lerp(tgon::Vector3(0.0f, 0.0f, 0.0f), tgon::Vector3(-clientSize.width, 0.0f, 0.0f), EaseOutExpo(m_animElapsedTime)));
            m_scene2->GetTransform()->SetLocalPosition(tgon::Lerp(tgon::Vector3(clientSize.width, 0.0f, 0.0f), tgon::Vector3(0.0f, 0.0f, 0.0f), EaseOutExpo(m_animElapsedTime)));

            if (m_animElapsedTime >= 1.0f)
            {
                if (m_scene1 != nullptr)
                {
                    m_scene1->OnFinish();
                }

                m_scene1 = std::move(m_scene2);
                m_scene1->GetTransform()->SetLocalPosition({0.0f, 0.0f, 0.0f});

                timerModule->ClearTimer(timerHandle);
            }
        }, 0.0f, true);
    }
}