#include "PrecompiledHeader.h"

#include "Engine/GraphicsModule.h"
#include "Engine/SceneModule.h"
#include "LogoScene/LogoScene.h"

#include "ThousandParty.h"
#include "GameDataModule.h"

TGON_DECLARE_ENGINE(ThousandParty);

ThousandParty::ThousandParty() :
    tgon::Engine({
        tgon::WindowStyle{u8"Thousand Party", 0,0,838,441, true, true, false, false, false},
        tgon::InputMode{true, false, false},
        tgon::VideoMode{tgon::Color4f(1.0f, 1.0f, 1.0f, 1.0f), true, false, false, false, 16, 32, 24, 8}
    })
{
}

void ThousandParty::Initialize()
{
    Super::Initialize();

    this->FindModule<tgon::GraphicsModule>()->GetUIRenderer().SetMaxSortingLayer(10);
    this->FindModule<tgon::SceneModule>()->ChangeScene<LogoScene>();
}

void ThousandParty::InitializeModule()
{
    Super::Initialize();

    this->AddModule<GameDataModule>();
}
