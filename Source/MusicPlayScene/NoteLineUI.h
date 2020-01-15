/**
 * @file    NoteLineUI.h
 * @author  ggomdyu
 * @since   12/13/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

class NoteLineUI :
    public tgon::Component
{
public:
    TGON_DECLARE_RTTI(NoteLineUI)

/**@section Method */
public:
    void Initialize() override;
    tgon::Vector3 GetNoteStartPosition(int32_t index) const;
    tgon::Vector3 GetNoteHitPosition(int32_t index) const;
    
private:
    void InitializeLines();
    
/**@section Variable */
protected:
    std::array<std::shared_ptr<tgon::GameObject>, 5> m_lines;
};
