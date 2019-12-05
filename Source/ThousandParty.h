/**
 * @file    ThousandParty.h
 * @author  ggomdyu
 * @since   09/29/2018
 */

#pragma once
#include <memory>

#include "Engine/Engine.h"

class ThousandParty final :
    public tgon::Engine
{
public:
    TGON_DECLARE_RTTI(ThousandParty)

/**@section Constructor */
public:
    ThousandParty() {}

/**@section Method */
public:
    void Initialize() override;
    void Update() override;
    void InitializeModule();
};
