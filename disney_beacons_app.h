/**
 * @file disney_beacons_app.h
 * @brief DIS beacons.
 */
#pragma once

#include "extra_beacon.h"
#include <furi.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/widget.h>
#include <gui/modules/submenu.h>
#include <gui/modules/byte_input.h>
#include <gui/modules/dialog_ex.h>

#include <rpc/rpc_app.h>
#include <notification/notification_messages.h>

#include <furi_hal_bt.h>

#include "scenes/scenes.h"
#include <stdint.h>

typedef struct {
    Gui* gui;
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;

    Submenu* submenu;
    ByteInput* byte_input;
    DialogEx* dialog_ex;
    Submenu* ears;
    Submenu* band;

    FuriString* status_string;

    GapExtraBeaconConfig beacon_config;
    uint8_t beacon_data[EXTRA_BEACON_MAX_DATA_SIZE];
    uint8_t beacon_data_len;
    bool is_beacon_active;
} DisneyBeaconsApp;

typedef enum {
    DisneyBeaconsAppViewSubmenu,
    DisneyBeaconsAppViewByteInput,
    DisneyBeaconsAppViewDialog,
    DisneyBeaconsAppViewEars,
    DisneyBeaconsAppViewBand,
} DisneyBeaconsAppView;

typedef enum {
    DisneyBeaconsAppCustomEventDataEditResult = 100,
} DisneyBeaconsAppCustomEvent;

void disney_beacons_app_update_state(DisneyBeaconsApp* app, uint8_t beacon_data_len);
