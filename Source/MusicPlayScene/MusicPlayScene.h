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
class TextRendererComponent;

} /* namespace tgon */

class Note;
class LongNote;

class MusicPlayScene :
    public tgon::Scene
{
public:
    TGON_DECLARE_RTTI(MusicPlayScene)
    
/**@section Constructor */
public:
    explicit MusicPlayScene(const MusicInfo& musicInfo);
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void InitializeBackgroundObject();
    void InitializeNoteObjectPool();
    void InitializeLongNoteObjectPool();
    void InitializeHitRingObject();
    std::shared_ptr<Note> GetNoteObjectFromPool();
    std::shared_ptr<LongNote> GetLongNoteObjectFromPool();
    void UpdateNoteLine();
    void UpdateBackgroundObjectPosition();
    
/**@section Variable */
private:
    bool m_isMusicWaiting;
    float m_elapsedTime;
    size_t m_noteInfoIndex;
    MusicInfo m_musicInfo;
    tgon::AudioPlayer m_audioPlayer;
    std::vector<std::shared_ptr<Note>> m_noteObjectPool;
    std::vector<std::shared_ptr<LongNote>> m_longNoteObjectPool;
    std::array<std::deque<std::shared_ptr<Note>>, 4> m_noteLine;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::AudioModule> m_audioModule;
    std::shared_ptr<tgon::TextRendererComponent> m_comboRendererComponent;
    std::shared_ptr<tgon::TextRendererComponent> m_musicNameRendererComponent;
    std::shared_ptr<tgon::TextRendererComponent> m_musicArtistNameRendererComponent;
    std::shared_ptr<tgon::GameObject> m_backgroundObject;
};
