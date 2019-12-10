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
    virtual void UpdateInput();
    virtual bool CheckCanHit() const;
    NoteTiming CheckNoteTiming() const noexcept;
    void SetNoteLineIndex(int32_t index) noexcept;
    void SetElapsedTime(float elapsedTime) noexcept;
    void SetHitTime(float hitTime) noexcept;
    float GetHitTime() const noexcept;
    bool IsHitted() const noexcept;

private:
    void InitializeSprite();
    
/**@section Variable */
protected:
    std::shared_ptr<tgon::SpriteRendererComponent> m_noteRendererComponent;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::Keyboard> m_keyboard;
    bool m_isHitted = false;
    float m_elapsedTime = 0.0f;
    float m_hitTime = 0.0f;
    int32_t m_noteLineIndex = 0;
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
    bool CheckCanHit() const override;
    void SetHoldTime(float holdTime) noexcept;
    float GetHoldTime() const noexcept;

private:
    void InitializeSprite();

/**@section Variable */
protected:
    char m_holdingKey = 0;
    float m_holdTime = 0.0f;
    std::shared_ptr<tgon::SpriteRendererComponent> m_longNoteRendererComponent;
};
