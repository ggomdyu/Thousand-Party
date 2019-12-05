/**
 * @file    MusicSelector.h
 * @author  ggomdyu
 * @since   12/03/2019
 */

#pragma once
#include <memory>
#include <array>

#include "Game/GameObject.h"

namespace tgon
{

class TimeModule;
class InputModule;
class GameObject;

} /* namespace tgon */

class MusicSelector :
    public tgon::GameObject
{
public:
    TGON_DECLARE_RTTI(MusicSelector)

/**@section Constructor */
public:
    MusicSelector();
    
/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    
private:
    void OnHandleInput();
    
private:

/**@section Variable */
private:
    std::shared_ptr<tgon::TimeModule> m_timeModule;
    std::shared_ptr<tgon::InputModule> m_inputModule;
    std::array<std::shared_ptr<tgon::GameObject>, 5> m_musicCoverImages;
};
