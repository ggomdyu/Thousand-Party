/**
 * @file    MusicEditScene.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include <memory>
#include <deque>

#include "Audio/AudioSource.h"
#include "Game/Scene.h"

#include "../GameDataModule.h"

namespace tgon
{

class GameObject;
class AudioModule;
class TimeModule;
class UITextRendererComponent;
class Material;
class Keyboard;

} /* namespace tgon */

class Note;
class HoldNote;
class NoteComboInfo;
class NoteLineUI;
class MusicLeftTimeUI;
enum class NoteTiming;

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
    MusicEditScene();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    void OnActivate() override;
    void SetMusicInfo(const MusicInfo& musicInfo);
    
private:
    void OnHandleInput();
    void RefreshNotes();
    void InitializeBackgroundObject();
    void InitializeNoteComboInfo();
    void InitializeCoverImageUI();
    void InitializeNoteLineBoxUI();
    void InitializeMusicLeftTimeUI();
    void InitializeFadeOutUI();
    void InitializeNoteObjectPool();
    void InitializeHoldNoteObjectPool();
    NoteObjectPair GetNoteObjectFromPool();
    HoldNoteObjectPair GetHoldNoteObjectFromPool();
    void InitializeMusicNameObject();
    void InitializeMusicArtistNameObject();
    void OnHitNote(NoteTiming noteTiming);
    void UpdateNotes();
    void UpdateNoteLineEdgeOffset();
    void PlayMusicFinishFadeOut(tgon::Delegate<void()>&& onFinishFadeOut);

/**@section Variable */
private:
    bool m_isMusicStopped = false;
    bool m_isMusicWaiting = true;
    float m_elapsedTime = 0.0f;
    float m_noteLineEdgeOffset = 0.0f;
    size_t m_noteInfoIndex = 0;
    MusicInfo m_musicInfo;
    tgon::AudioSource m_audioPlayer;
    std::shared_ptr<NoteComboInfo> m_noteComboInfo;
    std::shared_ptr<NoteLineUI> m_noteLine;
    std::shared_ptr<MusicLeftTimeUI> m_musicLeftTime;
    std::vector<NoteObjectPair> m_noteObjectPool;
    std::vector<HoldNoteObjectPair> m_holdNoteObjectPool;
    std::array<std::deque<NoteObjectPair>, 5> m_notes;
    std::shared_ptr<tgon::Keyboard> m_keyboard;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::AudioModule> m_audioModule;
    std::shared_ptr<tgon::Material> m_noteLineEdgeMaterial;
    std::shared_ptr<tgon::UITextRendererComponent> m_musicNameTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_musicArtistNameTextComponent;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_backgroundSpriteComponent;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_coverImageSpriteComponent;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_fadeOutSpriteComponent;
};
