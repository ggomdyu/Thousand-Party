/**
 * @file    MusicSelectScene.h
 * @author  ggomdyu
 * @since   12/02/2019
 */

#pragma once
#include <memory>

#include "Game/Scene.h"

namespace tgon
{

class GameObject;
class TimeModule;
class TextRendererComponent;

} /* namespace tgon */

class MusicSelectScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(MusicSelectScene)
    
/**@section Constructor */
public:
    MusicSelectScene();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void CreateSpriteObjects();
    void CreateSquareBackgroundObject();
    void CreateMusicSelectorObject();
    void CreateMusicNameObject();
    void CreateMusicComposerObject();
    void OnChangeSelectedMusic();

/**@section Variable */
private:
    std::shared_ptr<tgon::GameObject> m_musicText;
    std::shared_ptr<tgon::GameObject> m_selectText;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::TextRendererComponent> m_musicNameRendererComponent;
    std::shared_ptr<tgon::TextRendererComponent> m_musicComposerRendererComponent;
};
