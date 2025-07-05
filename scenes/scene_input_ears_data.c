#include "../disney_beacons_app.h"

typedef struct {
    const char* name;
    const uint8_t* data;
    size_t data_len;
} EarsMenuOption;

static void disney_beacons_app_scene_menu_ears_callback(void* context, uint32_t index) {
    DisneyBeaconsApp* ble_beacon = context;
    view_dispatcher_send_custom_event(ble_beacon->view_dispatcher, index);
}

static const EarsMenuOption ears_menu_options[] = {
    {
        .name = "Green",
        .data =
            (const uint8_t[]){0x02, 0x01, 0x04, 0x08, 0xff, 0x83, 0x01, 0x09,
                              0x0c, 0x01, 0xff, 0x62},
        .data_len = 12,
    },
    {
        .name = "Blue-Red",
        .data =
            (const uint8_t[]){0x02, 0x01, 0x04, 0x08, 0xff, 0x83, 0x01, 0x09,
                              0x0c, 0x01, 0xff, 0x61, 0x6c},
        .data_len = 13,
    },
    {
        .name = "Red-Blue",
        .data =
            (const uint8_t[]){0x02, 0x01, 0x04, 0x08, 0xff, 0x83, 0x01, 0x09,
                              0x0c, 0x01, 0xff, 0x64, 0x6c},
        .data_len = 13,
    },
};

static const size_t ears_menu_options_count = sizeof(ears_menu_options) / sizeof(EarsMenuOption);

bool disney_beacons_app_scene_input_ears_data_on_event(void* context, SceneManagerEvent event) {
    DisneyBeaconsApp* ble_beacon = context;
    SceneManager* scene_manager = ble_beacon->scene_manager;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event < ears_menu_options_count) {
            const EarsMenuOption* option = &ears_menu_options[event.event];
            memcpy(ble_beacon->beacon_data, option->data, option->data_len);
            disney_beacons_app_update_state(ble_beacon);
            scene_manager_previous_scene(scene_manager);
            consumed = true;
        }
    }

    return consumed;
}

void disney_beacons_app_scene_input_ears_data_on_enter(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    Submenu* ears = ble_beacon->ears;

    for(size_t x = 0; x < ears_menu_options_count; x++) {
        submenu_add_item(
            ears,
            ears_menu_options[x].name,
            x,
            disney_beacons_app_scene_menu_ears_callback,
            ble_beacon);
    }

    view_dispatcher_switch_to_view(ble_beacon->view_dispatcher, DisneyBeaconsAppViewEars);
}

void disney_beacons_app_scene_input_ears_data_on_exit(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    submenu_reset(ble_beacon->ears);
}
