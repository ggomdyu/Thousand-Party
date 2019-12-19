/**
 * @file    NoteHitInfo.h
 * @author  ggomdyu
 * @since   12/19/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

namespace tgon
{

class TextRendererComponent;

} /* namespace tgon */

class NoteHitInfo :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(NoteHitInfo)

/**@section Method */
public:
    void Initialize() override;
    void OnHitNote();
    void OnMissNote();
    
private:
    void InitializeSprite();
    void InitializeCurrentComboTextObject();
    void InitializeMaxComboTextObject();
    void RefreshComboText();
    
/**@section Variable */
protected:
    int32_t m_currCombo = 0;
    int32_t m_maxCombo = 0;
    std::shared_ptr<tgon::TextRendererComponent> m_currComboTextRendererComponent;
    std::shared_ptr<tgon::TextRendererComponent> m_maxComboTextRendererComponent;
};
