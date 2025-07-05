#include "../disney_beacons_app.h"

typedef struct {
    const char* name;
    const uint8_t* data;
    size_t data_len;
} BandMenuOption;

static void disney_beacons_app_scene_menu_band_callback(void* context, uint32_t index) {
    DisneyBeaconsApp* ble_beacon = context;
    view_dispatcher_send_custom_event(ble_beacon->view_dispatcher, index);
}
static const BandMenuOption band_menu_options[] = {
    {
        .name = "Blue-Red-Buzz",
        .data = (const uint8_t[]){0x02, 0x01, 0x04, 0x0e, 0xff, 0x83, 0x01, 0xe9, 0x09, 0x00, 0x12, 0x0f, 0xbc, 0xb5, 0xb5, 0xa4, 0xa4, 0xb9},
        .data_len = 18,
    },
};

static const size_t band_menu_options_count = sizeof(band_menu_options) / sizeof(BandMenuOption);


bool disney_beacons_app_scene_input_band_data_on_event(void* context, SceneManagerEvent event) {
    DisneyBeaconsApp* ble_beacon = context;
    SceneManager* scene_manager = ble_beacon->scene_manager;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event < band_menu_options_count) {
            const BandMenuOption* option = &band_menu_options[event.event];
            memcpy(ble_beacon->beacon_data, option->data, option->data_len);
            disney_beacons_app_update_state(ble_beacon);
            scene_manager_previous_scene(scene_manager);
            consumed = true;
        }
    }

    return consumed;
}

void disney_beacons_app_scene_input_band_data_on_enter(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    Submenu* band = ble_beacon->band;

    for(size_t x = 0; x < band_menu_options_count; x++) {
        submenu_add_item(
            band,
            band_menu_options[x].name,
            x,
            disney_beacons_app_scene_menu_band_callback,
            ble_beacon);
    }

    view_dispatcher_switch_to_view(ble_beacon->view_dispatcher, DisneyBeaconsAppViewBand);
}

void disney_beacons_app_scene_input_band_data_on_exit(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    submenu_reset(ble_beacon->band);
}
