#include "Platform/Application.h"
#include "Engine/TimeModule.h"
#include "Engine/TimerModule.h"
#include "Engine/AssetModule.h"
#include "Engine/InputModule.h"
#include "Math/Mathematics.h"
#include "IO/Path.h"
#include "Component/UISpriteRendererComponent.h"

#include "TitleScene.h"
#include "FireFly.h"
#include "NightSky.h"

#include "../MusicSelectScene/MusicSelectScene.h"
#include "../MusicPlayScene/Note.h"
#include "../MultipleSceneModule.h"

TitleScene::TitleScene()
{
}

void TitleScene::Update()
{
    Super::Update();

    this->OnHandleInput();
}

TitleScene::~TitleScene()
{
    this->DestroyTimer();
}

void TitleScene::Initialize()
{
    this->CreateNightSkyObject();
    this->CreateSpriteObjects();
    this->CreateFireFlyObjects();
    
    auto engine = tgon::Application::GetEngine();
    auto timerModule = engine->FindModule<tgon::TimerModule>();
    auto timeModule = engine->FindModule<tgon::TimeModule>();
    
    m_inputModule = engine->FindModule<tgon::InputModule>();
    m_fadeInTimerHandle = timerModule->SetTimer([this, timeModule, weakTimerModule = std::weak_ptr<tgon::TimerModule>(timerModule)](tgon::TimerHandle timerHandle)
    {
        auto blendColor = m_fadeInSpriteRendererComponent->GetBlendColor();
        if (blendColor.a <= 0.0f)
        {
            if (auto timerModule = weakTimerModule.lock(); timerModule != nullptr)
            {
                timerModule->ClearTimer(timerHandle);
            }

            m_fadeInTimerHandle = {};
            return;
        }
        
        blendColor.a = std::max(0.0f, blendColor.a - 1.0f * timeModule->GetTickTime());
        m_fadeInSpriteRendererComponent->SetBlendColor(blendColor);
    }, 0.0f, true);
    
    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    m_girlMoveTimerHandler = timerModule->SetTimer([this, clientSize, timeModule, weakTimerModule = std::weak_ptr<tgon::TimerModule>(timerModule)](tgon::TimerHandle timerHandle)
    {
        auto destXPos = -clientSize.width / 2 + 640.0f;
        auto position = m_girl->GetTransform()->GetLocalPosition();
        if (std::abs(position.x - destXPos) <= 0.0001f)
        {
            position.x = destXPos;
            m_girl->GetTransform()->SetLocalPosition(position);
            
            if (auto timerModule = weakTimerModule.lock(); timerModule != nullptr)
            {
                timerModule->ClearTimer(timerHandle);
            }
            return;
        }

        auto newXPos = tgon::Lerp(position.x, destXPos, 0.1f);
        position.x = newXPos;
        m_girl->GetTransform()->SetLocalPosition(position);
    }, 0.0f, true);
}

void TitleScene::CreateNightSkyObject()
{
    auto nightSky = tgon::GameObject::Create();
    nightSky->AddComponent<NightSky>();
    
    this->AddChild(std::move(nightSky));
}

void TitleScene::CreateSpriteObjects()
{
    std::string_view texturePathList[] =
    {
        u8"Resource/Background/TitleScene/star.png",
        u8"Resource/Background/TitleScene/earth.png",
        u8"Resource/Background/TitleScene/girl.png",
        u8"Resource/UI/TitleScene/gameLogo.png",
        u8"Resource/UI/TitleScene/pressDesc.png",
        u8"Resource/UI/TitleScene/creatorBtn.png",
        u8"Resource/UI/Common/FadeInOut.png",
    };

    auto clientSize = tgon::Application::GetRootWindow()->GetClientSize();
    float halfWindowWidth = clientSize.width * 0.5f;
    float halfWindowHeight = clientSize.height * 0.5f;
    tgon::Vector3 texturePosList[] =
    {
        tgon::Vector3(-halfWindowWidth + 640.0f, halfWindowHeight - 360.0f, 0.0f),
        tgon::Vector3(-halfWindowWidth + 612.0f, halfWindowHeight - 388.0f, 0.0f),
        tgon::Vector3(halfWindowWidth + 250.0f, halfWindowHeight - 221.0f, 0.0f),
        tgon::Vector3(-halfWindowWidth + 172.5f, halfWindowHeight - 92.5f, 0.0f),
        tgon::Vector3(-halfWindowWidth + 18.0f, -halfWindowHeight + 67.0f, 0.0f),
        tgon::Vector3(-halfWindowWidth, -halfWindowHeight, 0.0f),
        tgon::Vector3(0.0f, 0.0f, 0.0f),
    };
    
    tgon::Vector2 pivotList[] =
    {
        tgon::Vector2(0.5f, 0.5f),
        tgon::Vector2(0.5f, 0.5f),
        tgon::Vector2(0.5f, 0.5f),
        tgon::Vector2(0.5f, 0.5f),
        tgon::Vector2(0.0f, 1.0f),
        tgon::Vector2(0.0f, 1.0f),
        tgon::Vector2(0.5f, 0.5f),
    };
    
    int32_t sortingLayerList[] = {0, 0, 2, 2, 3, 3, 4};
    auto assetModule = tgon::Application::GetEngine()->FindModule<tgon::AssetModule>();
    for (int i = 0; i < std::extent_v<decltype(texturePathList)>; ++i)
    {
        auto object = tgon::GameObject::Create(tgon::Path::GetFileNameWithoutExtension(texturePathList[i]));
        object->GetTransform()->SetLocalPosition(texturePosList[i]);
        
        auto spriteRendererComponent = object->AddComponent<tgon::UISpriteRendererComponent>();
        spriteRendererComponent->SetTexture(assetModule->GetResource<tgon::Texture>(texturePathList[i]));
        spriteRendererComponent->SetSortingLayer(sortingLayerList[i]);
        spriteRendererComponent->SetPivot(pivotList[i]);
        this->AddChild(object);
    }

    m_girl = this->FindObject(u8"girl");
    m_fadeInSpriteRendererComponent = this->FindObject(u8"FadeInOut")->FindComponent<tgon::UISpriteRendererComponent>();
}

void TitleScene::CreateFireFlyObjects()
{
    for (int i = 0; i < 15; ++i)
    {
        auto fireflyObject = tgon::GameObject::Create();
        fireflyObject->AddComponent<FireFly>();

        this->AddChild(std::move(fireflyObject));
    }
}

void TitleScene::OnHandleInput()
{
    auto keyboard = m_inputModule->GetKeyboard();
    if (m_fadeInTimerHandle == tgon::TimerHandle() && (keyboard->IsKeyUp(tgon::KeyCode::Space) || keyboard->IsKeyUp(tgon::KeyCode::Return)))
    {
        auto sceneModule = tgon::Application::GetEngine()->FindModule<MultipleSceneModule>();
        sceneModule->ChangeScene<MusicSelectScene>(MultipleSceneModule::ChangeSceneAnimationType::NoAnim);
    }
}

void TitleScene::DestroyTimer()
{
    auto timerModule = tgon::Application::GetEngine()->FindModule<tgon::TimerModule>();
    timerModule->ClearTimer(m_fadeInTimerHandle);
    timerModule->ClearTimer(m_girlMoveTimerHandler);
}