#include "disney_beacons_app.h"

#include <extra_beacon.h>
#include <furi_hal_version.h>

#include <string.h>

#define TAG "DisneyBeaconsApp"

static bool disney_beacons_app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    DisneyBeaconsApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool disney_beacons_app_back_event_callback(void* context) {
    furi_assert(context);
    DisneyBeaconsApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void disney_beacons_app_tick_event_callback(void* context) {
    furi_assert(context);
    DisneyBeaconsApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

static void disney_beacons_app_restore_beacon_state(DisneyBeaconsApp* app) {
    // Restore beacon data from service
    GapExtraBeaconConfig* local_config = &app->beacon_config;
    const GapExtraBeaconConfig* config = furi_hal_bt_extra_beacon_get_config();
    if(config) {
        // We have a config, copy it
        memcpy(local_config, config, sizeof(app->beacon_config));
    } else {
        // No config, set up default values - they will stay until overriden or device is reset
        local_config->min_adv_interval_ms = 50;
        local_config->max_adv_interval_ms = 150;

        local_config->adv_channel_map = GapAdvChannelMapAll;
        local_config->adv_power_level = GapAdvPowerLevel_0dBm;

        local_config->address_type = GapAddressTypePublic;
        memcpy(
            local_config->address, furi_hal_version_get_ble_mac(), sizeof(local_config->address));
        // Modify MAC address to make it different from the one used by the main app
        local_config->address[0] ^= 0xFF;
        local_config->address[3] ^= 0xFF;

        furi_check(furi_hal_bt_extra_beacon_set_config(local_config));
    }

    // Get beacon state
    app->is_beacon_active = furi_hal_bt_extra_beacon_is_active();

    // Restore last beacon data
    app->beacon_data_len = furi_hal_bt_extra_beacon_get_data(app->beacon_data);
}

static DisneyBeaconsApp* disney_beacons_app_alloc(void) {
    DisneyBeaconsApp* app = malloc(sizeof(DisneyBeaconsApp));

    app->gui = furi_record_open(RECORD_GUI);

    app->scene_manager = scene_manager_alloc(&disney_beacons_app_scene_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();

    app->status_string = furi_string_alloc();

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, disney_beacons_app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, disney_beacons_app_back_event_callback);
    view_dispatcher_set_tick_event_callback(
        app->view_dispatcher, disney_beacons_app_tick_event_callback, 100);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    app->submenu = submenu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DisneyBeaconsAppViewSubmenu, submenu_get_view(app->submenu));

    app->dialog_ex = dialog_ex_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DisneyBeaconsAppViewDialog, dialog_ex_get_view(app->dialog_ex));

    app->byte_input = byte_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DisneyBeaconsAppViewByteInput, byte_input_get_view(app->byte_input));

    app->ears = submenu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DisneyBeaconsAppViewEars, submenu_get_view(app->ears));

    app->band = submenu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, DisneyBeaconsAppViewBand, submenu_get_view(app->band));

    disney_beacons_app_restore_beacon_state(app);

    return app;
}

static void disney_beacons_app_free(DisneyBeaconsApp* app) {
    view_dispatcher_remove_view(app->view_dispatcher, DisneyBeaconsAppViewByteInput);
    view_dispatcher_remove_view(app->view_dispatcher, DisneyBeaconsAppViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, DisneyBeaconsAppViewDialog);
    view_dispatcher_remove_view(app->view_dispatcher, DisneyBeaconsAppViewEars);
    view_dispatcher_remove_view(app->view_dispatcher, DisneyBeaconsAppViewBand);

    free(app->byte_input);
    free(app->submenu);
    free(app->dialog_ex);
    free(app->ears);
    free(app->band);

    free(app->scene_manager);
    free(app->view_dispatcher);

    free(app->status_string);

    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);
    app->gui = NULL;

    free(app);
}

int32_t disney_beacons_app(void* args) {
    UNUSED(args);

    DisneyBeaconsApp* app = disney_beacons_app_alloc();

    scene_manager_next_scene(app->scene_manager, DisneyBeaconsAppSceneRunBeacon);

    view_dispatcher_run(app->view_dispatcher);

    disney_beacons_app_free(app);
    return 0;
}

void disney_beacons_app_update_state(DisneyBeaconsApp* app, uint8_t beacon_data_len) {
    furi_hal_bt_extra_beacon_stop();

    furi_check(furi_hal_bt_extra_beacon_set_config(&app->beacon_config));

    app->beacon_data_len = beacon_data_len;

    FURI_LOG_I(TAG, "beacon_data_len: %d", app->beacon_data_len);

    furi_check(furi_hal_bt_extra_beacon_set_data(app->beacon_data, app->beacon_data_len));

    if(app->is_beacon_active) {
        furi_check(furi_hal_bt_extra_beacon_start());
    }
}
