/**
 * @file    Note.h
 * @author  ggomdyu
 * @since   12/09/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"
#include "Engine/TimerHandle.h"

namespace tgon
{

class TimeModule;
class InputModule;
class SpriteRendererComponent;

} /* namespace tgon */

enum NoteTiming
{
    Perfect,
    Great,
    Early,
    Late,
    Miss
};

class Note :
    public tgon::GameObject
{
public:
    TGON_DECLARE_RTTI(Note)

/**@section Constructor */
public:
    Note();
    
/**@section Method */
public:
    void Reset();
    void Initialize() override;
    void Update() override;
    void UpdateInput();
//    virtual bool CheckHit() const;
    NoteTiming CheckNoteTiming() const noexcept;
    void SetNoteLineIndex(int32_t index) noexcept;
    void SetElapsedTime(float elapsedTime) noexcept;
    void SetHitTime(float hitTime) noexcept;
    float GetHitTime() const noexcept;
    bool IsHitted() const noexcept;

protected:
    virtual void InitializeSprite();
    
/**@section Variable */
protected:
    bool m_isHitted;
    std::shared_ptr<tgon::SpriteRendererComponent> m_noteRendererComponent;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::Keyboard> m_keyboard;
    float m_elapsedTime = 0.0f;
    float m_hitTime = 0.0f;
    int32_t m_noteLineIndex;
};

class LongNote :
    public Note
{
public:
    TGON_DECLARE_RTTI(LongNote)

/**@section Constructor */
public:
    using Note::Note;
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
protected:
    void InitializeSprite() override;
    
/**@section Variable */
protected:
    std::shared_ptr<tgon::SpriteRendererComponent> m_longNoteRendererComponent;
};
