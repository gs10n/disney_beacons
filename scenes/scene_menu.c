#include "../disney_beacons_app.h"

enum SubmenuIndex {
    SubmenuIndexSetMac,
    SubmenuIndexSetData,
    SubmenuIndexSetEars,
};

static void disney_beacons_app_scene_menu_submenu_callback(void* context, uint32_t index) {
    DisneyBeaconsApp* ble_beacon = context;
    view_dispatcher_send_custom_event(ble_beacon->view_dispatcher, index);
}

void disney_beacons_app_scene_menu_on_enter(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    Submenu* submenu = ble_beacon->submenu;

    submenu_add_item(
        submenu,
        "Set MAC",
        SubmenuIndexSetMac,
        disney_beacons_app_scene_menu_submenu_callback,
        ble_beacon);
    submenu_add_item(
        submenu,
        "Set Data",
        SubmenuIndexSetData,
        disney_beacons_app_scene_menu_submenu_callback,
        ble_beacon);
    submenu_add_item(
        submenu,
        "Ears",
        SubmenuIndexSetEars,
        disney_beacons_app_scene_menu_submenu_callback,
        ble_beacon);

    view_dispatcher_switch_to_view(ble_beacon->view_dispatcher, DisneyBeaconsAppViewSubmenu);
}

bool disney_beacons_app_scene_menu_on_event(void* context, SceneManagerEvent event) {
    DisneyBeaconsApp* ble_beacon = context;
    SceneManager* scene_manager = ble_beacon->scene_manager;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        const uint32_t submenu_index = event.event;
        if(submenu_index == SubmenuIndexSetMac) {
            scene_manager_next_scene(scene_manager, DisneyBeaconsAppSceneInputMacAddress);
            consumed = true;
        } else if(submenu_index == SubmenuIndexSetData) {
            scene_manager_next_scene(scene_manager, DisneyBeaconsAppSceneInputBeaconData);
            consumed = true;
        } else if(submenu_index == SubmenuIndexSetEars) {
            scene_manager_next_scene(scene_manager, DisneyBeaconsAppSceneInputEarsData);
            consumed = true;
        }
    }

    return consumed;
}

void disney_beacons_app_scene_menu_on_exit(void* context) {
    DisneyBeaconsApp* ble_beacon = context;
    submenu_reset(ble_beacon->submenu);
}
