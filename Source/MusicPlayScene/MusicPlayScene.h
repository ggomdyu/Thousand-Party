/**
 * @file    MusicPlayScene.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include <memory>
#include <deque>

#include "Audio/AudioPlayer.h"
#include "Game/Scene.h"

#include "../GameDataModule.h"

namespace tgon
{

class GameObject;
class AudioModule;
class TimeModule;
class UITextRendererComponent;
class Material;

} /* namespace tgon */

class Note;
class HoldNote;
class NoteHitInfo;
class NoteLineUI;
class MusicLeftTimeUI;

class MusicPlayScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(MusicPlayScene)

/**@section Type */
private:
    using NoteObjectPair = std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<Note>>;
    using HoldNoteObjectPair = std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<HoldNote>>;

/**@section Constructor */
public:
    MusicPlayScene();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    void OnActivate() override;
    void SetMusicInfo(const MusicInfo& musicInfo);
    
private:
    void InitializeBackgroundObject();
    void InitializeNoteHitInfo();
    void InitializeNoteLineBoxUI();
    void InitializeMusicLeftTimeUI();
    void InitializeNoteObjectPool();
    void InitializeHoldNoteObjectPool();
    NoteObjectPair GetNoteObjectFromPool();
    HoldNoteObjectPair GetHoldNoteObjectFromPool();
    void UpdateNotes();
    void UpdateBackgroundObjectPosition();
    void InitializeMusicNameObject();
    void InitializeMusicArtistNameObject();

/**@section Variable */
private:
    bool m_isMusicWaiting = true;
    float m_elapsedTime = 0.0f;
    size_t m_noteInfoIndex = 0;
    MusicInfo m_musicInfo;
    tgon::AudioPlayer m_audioPlayer;
    std::shared_ptr<tgon::GameObject> m_backgroundObject;
    std::shared_ptr<NoteHitInfo> m_noteHitInfo;
    std::shared_ptr<NoteLineUI> m_noteLine;
    std::shared_ptr<MusicLeftTimeUI> m_musicLeftTime;
    std::vector<NoteObjectPair> m_noteObjectPool;
    std::vector<HoldNoteObjectPair> m_holdNoteObjectPool;
    std::array<std::deque<NoteObjectPair>, 5> m_notes;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::AudioModule> m_audioModule;
    std::shared_ptr<tgon::Material> m_noteLineEdgeMaterial;
    std::shared_ptr<tgon::UITextRendererComponent> m_comboRendererComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_musicNameRendererComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_musicArtistNameRendererComponent;
};
