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

/**@section Destructor */
public:
    ~ThousandParty() override = default;

/**@section Event Handler */
public:
    void OnLaunch() override;
    void OnTerminate() override;

/**@section Method */
public:
    void Update() override;

    void Initialize();
    void InitializeModule();
};
