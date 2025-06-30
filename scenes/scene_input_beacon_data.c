#include "../disney_beacons_app.h"

static void disney_beacons_app_scene_add_type_byte_input_callback(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    view_dispatcher_send_custom_event(
        ble_beacon->view_dispatcher, DisneyBeaconsAppCustomEventDataEditResult);
}

void disney_beacons_app_scene_input_beacon_data_on_enter(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    byte_input_set_header_text(ble_beacon->byte_input, "Enter beacon data");

    byte_input_set_result_callback(
        ble_beacon->byte_input,
        disney_beacons_app_scene_add_type_byte_input_callback,
        NULL,
        context,
        ble_beacon->beacon_data,
        sizeof(ble_beacon->beacon_data));

    view_dispatcher_switch_to_view(ble_beacon->view_dispatcher, DisneyBeaconsAppViewByteInput);
}

bool disney_beacons_app_scene_input_beacon_data_on_event(void* context, SceneManagerEvent event) {
    DisneyBeaconsApp* ble_beacon = context;
    SceneManager* scene_manager = ble_beacon->scene_manager;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == DisneyBeaconsAppCustomEventDataEditResult) {
            disney_beacons_app_update_state(ble_beacon);
            scene_manager_previous_scene(scene_manager);
            return true;
        }
    }

    return false;
}

void disney_beacons_app_scene_input_beacon_data_on_exit(void* context) {
    DisneyBeaconsApp* ble_beacon = context;

    byte_input_set_result_callback(ble_beacon->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(ble_beacon->byte_input, NULL);
}
