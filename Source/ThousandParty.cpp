#include "PrecompiledHeader.h"

#include "LogoScene/LogoScene.h"

#include "TGON.h"
#include "ThousandParty.h"

TGON_DECLARE_ENGINE(ThousandParty);

using namespace tgon;

void ThousandParty::Initialize()
{
    Super::Initialize();
    
    this->InitializeModule();
}

void ThousandParty::Update()
{
    Super::Update();
}

void ThousandParty::InitializeModule()
{
    this->AddModule<AssetModule>();
    this->AddModule<TimeModule>();
    this->AddModule<TimerModule>();
    this->AddModule<TaskModule>();
    
    decltype(auto) rootWindow = Application::GetInstance().GetRootWindow();
    InputMode inputMode;
    {
        inputMode.isUseMouse = false;
        inputMode.isUseKeyboard = true;
        inputMode.isUseGamepad = false;
    }
    this->AddModule<InputModule>(*rootWindow, inputMode);
    
    VideoMode videoMode;
    {
        videoMode.clearColor = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
        videoMode.enableHardwareAccelerate = true;
        videoMode.enableTripleBuffer = false;
        videoMode.enableVerticalSync = false;
        videoMode.enableMultiSampling = false;
    };
    this->AddModule<GraphicsModule>(*rootWindow, videoMode)->GetUIRenderer().SetMaxSortingLayer(10);
    
    this->AddModule<SceneModule>()->ChangeScene<LogoScene>();
}
