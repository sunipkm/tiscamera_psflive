#ifndef __PSFLIVE_H__
#define __PSFLIVE_H__

#pragma once

#include "psflive_device_Imager.h"
#include "psflive_Connection.h"
#include "psflive_Configuration.h"
// ====================================================================================================================










// ====================================================================================================================
namespace psflive {
  struct AppData {
    psflive::device::Imager* ptrImager = nullptr;
    psflive::Configuration* ptrConfiguration = nullptr;
    psflive::Connection* ptrConnection = nullptr;
  };
};
// ====================================================================================================================
#endif //__PSFLIVE_H__