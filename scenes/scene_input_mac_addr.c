#include "../disney_beacons_app.h"

static void disney_beacons_app_scene_add_type_byte_input_callback(void* context) {
    BleBeaconApp* ble_beacon = context;
    view_dispatcher_send_custom_event(
        ble_beacon->view_dispatcher, BleBeaconAppCustomEventDataEditResult);
}

void disney_beacons_app_scene_input_mac_addr_on_enter(void* context) {
    BleBeaconApp* ble_beacon = context;
    byte_input_set_header_text(ble_beacon->byte_input, "Enter MAC (reversed)");

    byte_input_set_result_callback(
        ble_beacon->byte_input,
        disney_beacons_app_scene_add_type_byte_input_callback,
        NULL,
        context,
        ble_beacon->beacon_config.address,
        sizeof(ble_beacon->beacon_config.address));

    view_dispatcher_switch_to_view(ble_beacon->view_dispatcher, BleBeaconAppViewByteInput);
}

bool disney_beacons_app_scene_input_mac_addr_on_event(void* context, SceneManagerEvent event) {
    BleBeaconApp* ble_beacon = context;
    SceneManager* scene_manager = ble_beacon->scene_manager;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == BleBeaconAppCustomEventDataEditResult) {
            disney_beacons_app_update_state(ble_beacon);
            scene_manager_previous_scene(scene_manager);
            return true;
        }
    }

    return false;
}

void disney_beacons_app_scene_input_mac_addr_on_exit(void* context) {
    BleBeaconApp* ble_beacon = context;

    byte_input_set_result_callback(ble_beacon->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(ble_beacon->byte_input, NULL);
}
