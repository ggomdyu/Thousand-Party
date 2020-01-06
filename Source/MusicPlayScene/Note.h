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
class UISpriteRendererComponent;

} /* namespace tgon */

enum class NoteTiming
{
    Perfect,
    Great,
    Early,
    Late,
    Miss
};

class NoteLine;
class Note :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(Note)

/**@section Constructor */
public:
    explicit Note(const std::shared_ptr<NoteLine>& noteLine);
    
/**@section Method */
public:
    virtual void Reset();
    void Initialize() override;
    void Update() override;
    virtual void UpdateInput();
    virtual bool CheckCanHit() const;
    NoteTiming CheckNoteTiming(float timingOffset = 0.0f) const noexcept;
    void SetNoteLineIndex(int32_t index) noexcept;
    void SetElapsedTime(float elapsedTime) noexcept;
    void SetHitTime(float hitTime) noexcept;
    float GetHitTime() const noexcept;
    bool IsHitted() const noexcept;
    bool IsHolding() const noexcept;
    
protected:
    virtual void OnHitNote(tgon::KeyCode keyCode, NoteTiming noteTiming);
    void PlayHitSound();
    
private:
    void InitializeSprite();
    
/**@section Variable */
protected:
    bool m_autoHitted = false;
    std::shared_ptr<NoteLine> m_noteLine;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_noteRendererComponent;
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::Keyboard> m_keyboard;
    std::shared_ptr<tgon::Transform> m_transform;
    bool m_isHitted = false;
    bool m_isHolding = false;
    tgon::KeyCode m_hittedKeyCode;
    float m_elapsedTime = 0.0f;
    float m_hitTime = 0.0f;
    int32_t m_noteLineIndex = 0;
};

class HoldNote :
    public Note
{
public:
    TGON_DECLARE_RTTI(HoldNote)

/**@section Constructor */
public:
    using Note::Note;
    
/**@section Method */
public:
    void Reset() override;
    void Initialize() override;
    void Update() override;
    void UpdateInput() override;
    bool CheckCanHit() const override;
    void SetHoldTime(float holdTime) noexcept;
    float GetHoldTime() const noexcept;

protected:
    void OnHitNote(tgon::KeyCode keyCode, NoteTiming noteTiming) override;
        
private:
    void InitializeSprite();

/**@section Variable */
protected:
    float m_holdTime = 0.0f;
    std::shared_ptr<tgon::GameObject> m_ringObject;
    std::shared_ptr<tgon::UISpriteRendererComponent> m_holdNoteRendererComponent;
};
