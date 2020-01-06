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
class Keyboard;

} /* namespace tgon */

class Note;
class HoldNote;
class NoteLine;

class MusicEditScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(MusicEditScene)
    
/**@section Type */
private:
    using NoteObjectPair = std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<Note>>;
    using HoldNoteObjectPair = std::pair<std::shared_ptr<tgon::GameObject>, std::shared_ptr<HoldNote>>;

/**@section Constructor */
public:
    explicit MusicEditScene(const MusicInfo& musicInfo);
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void OnHandleInput();
    void InitializeBackgroundObject();
    void InitializeNoteLine();
    void InitializeNoteObjectPool();
    void InitializeHoldNoteObjectPool();
    NoteObjectPair GetNoteObjectFromPool();
    HoldNoteObjectPair GetHoldNoteObjectFromPool();
    void UpdateNotes();
    void UpdateBackgroundObjectPosition();
    void InitializeMusicNameObject();
    void InitializeMusicArtistNameObject();
    void RefreshNotes();

/**@section Variable */
private:
    bool m_isMusicStopped = false;
    bool m_isMusicWaiting = true;
    float m_elapsedTime = 0.0f;
    size_t m_noteInfoIndex = 0;
    MusicInfo m_musicInfo;
    tgon::AudioPlayer m_audioPlayer;
    std::shared_ptr<tgon::GameObject> m_backgroundObject;
    std::shared_ptr<NoteLine> m_noteLine;
    std::vector<NoteObjectPair> m_noteObjectPool;
    std::vector<HoldNoteObjectPair> m_holdNoteObjectPool;
    std::array<std::deque<NoteObjectPair>, 5> m_notes;
    std::shared_ptr<tgon::Keyboard> m_keyboard;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::AudioModule> m_audioModule;
    std::shared_ptr<tgon::UITextRendererComponent> m_comboRendererComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_musicNameRendererComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_musicArtistNameRendererComponent;
};
