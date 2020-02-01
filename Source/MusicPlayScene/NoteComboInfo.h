/**
 * @file    NoteComboInfo.h
 * @author  ggomdyu
 * @since   12/19/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

#include "Note.h"

namespace tgon
{

class UITextRendererComponent;

} /* namespace tgon */

class NoteComboInfo :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(NoteComboInfo)

/**@section Method */
public:
    void Initialize() override;
    void OnHitNote(NoteTiming noteTiming);
    void OnMissNote();
    void Reset();

private:
    void InitializePosition();
    void InitializeLine();
    void InitializeCurrentComboText();
    void InitializeMaxComboText();
    void InitializePerfectText();
    void InitializeEarlyText();
    void InitializeLazyText();
    void InitializeMissText();
    void RefreshText();
    void RefreshCurrentComboText();
    void RefreshMaxComboText();
    void RefreshPerfectText();
    void RefreshEarlyText();
    void RefreshLazyText();
    void RefreshMissText();

/**@section Variable */
protected:
    int32_t m_currCombo = 0;
    int32_t m_maxCombo = 0;
    int32_t m_perfectCount = 0;
    int32_t m_earlyCount = 0;
    int32_t m_lazyCount = 0;
    int32_t m_missCount = 0;
    std::shared_ptr<tgon::UITextRendererComponent> m_currComboTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_maxComboTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_perfectTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_earlyTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_lazyTextComponent;
    std::shared_ptr<tgon::UITextRendererComponent> m_missTextComponent;
};