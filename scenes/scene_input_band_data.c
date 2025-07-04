#include "../disney_beacons_app.h"

static void disney_beacons_app_scene_menu_band_callback(void* context, uint32_t index) {
    DisneyBeaconsApp* ble_beacon = context;
    view_dispatcher_send_custom_event(ble_beacon->view_dispatcher, index);
}

bool disney_beacons_app_scene_input_band_data_on_event(void* context, SceneManagerEvent event) {
    DisneyBeaconsApp* ble_beacon = context;
    SceneManager* scene_manager = ble_beacon->scene_manager;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) 
    {

        switch(event.event)
        {
            case 1:
                memcpy(
                    ble_beacon->beacon_data,
                    ((uint8_t[]){0x02, 0x01, 0x04, 0x08, 0xff, 0x83, 0x02, 0x09, 0x0c, 0x02, 0xff, 0x63}),
                    12);
                break;
            default:
                break;
        }

        disney_beacons_app_update_state(ble_beacon);
        scene_manager_previous_scene(scene_manager);
        consumed = true;
    }

    return consumed;
}

void disney_beacons_app_scene_input_band_data_on_enter(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    Submenu* band = ble_beacon->band;

    submenu_add_item(
        band,
        "Blue",
        1,
        disney_beacons_app_scene_menu_band_callback,
        ble_beacon);

    view_dispatcher_switch_to_view(ble_beacon->view_dispatcher, DisneyBeaconsAppViewBand);
}

void disney_beacons_app_scene_input_band_data_on_exit(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    submenu_reset(ble_beacon->band);
}
