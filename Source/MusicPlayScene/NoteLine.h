/**
 * @file    NoteLine.h
 * @author  ggomdyu
 * @since   12/13/2019
 */

#pragma once
#include <memory>

#include "Game/GameObject.h"

class NoteLine :
    public tgon::GameObject
{
public:
    TGON_DECLARE_RTTI(NoteLine)

/**@section Constructor */
public:
    NoteLine();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    tgon::Vector3 GetNoteStartPosition(int32_t index) const;
    tgon::Vector3 GetNoteHitPosition(int32_t index) const;
    
private:
    void InitializeLines();
    
/**@section Variable */
protected:
    std::array<std::shared_ptr<tgon::GameObject>, 5> m_lines;
};
