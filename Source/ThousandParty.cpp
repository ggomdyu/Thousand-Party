#include "Engine/UIRendererModule.h"
#include "Engine/SceneModule.h"
#include "LogoScene/LogoScene.h"
#include "MusicResultScene/MusicResultScene.h"

#include "ThousandParty.h"
#include "GameDataModule.h"
#include "MultipleSceneModule.h"

TGON_DECLARE_ENGINE(ThousandParty);

ThousandParty::ThousandParty() :
    tgon::Engine({
        tgon::WindowStyle{u8"Thousand Party", 0, 0, 838, 441, true, true, false, false, false},
        tgon::InputMode{true, false, false},
        tgon::VideoMode{tgon::Color4f(1.0f, 1.0f, 1.0f, 1.0f), true, false, false, false, 16, 32, 24, 8}
    })
{
}

void ThousandParty::Initialize()
{
    Super::Initialize();
    
    this->RemoveModule<tgon::SceneModule>();

    auto uiRendererModule = this->FindModule<tgon::UIRendererModule>();
    uiRendererModule->SetMaxSortingLayer(10);

    auto gameDataModule = this->AddModule<GameDataModule>();
    this->AddModule<MultipleSceneModule>()->ChangeScene(MultipleSceneChangeAnimType::NoAnim, gameDataModule->GetCachedScene<LogoScene>());
}