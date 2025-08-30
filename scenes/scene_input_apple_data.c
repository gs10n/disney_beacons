#include "../disney_beacons_app.h"

typedef struct {
  const char *name;
  const uint8_t *data;
  size_t data_len;
} AppleMenuOption;

static void disney_beacons_app_scene_menu_apple_callback(void *context,
                                                         uint32_t index) {
  DisneyBeaconsApp *ble_beacon = context;
  view_dispatcher_send_custom_event(ble_beacon->view_dispatcher, index);
}

static const AppleMenuOption apple_menu_options[] = {
    {
        .name = "Airtag",
        .data =
            (const uint8_t[]){
                0x1E, 0xFF, 0x4C, 0x00, 0x07, 0x19, 0x05, 0x00,
                0x55, 0x10, 0x00, 0x00, 0x01, 0x4E, 0xCA, 0x91,
                0x89, 0xF2, 0xE6, 0x8A, 0xA8, 0xC7, 0xDC, 0x1E,
                0x4A, 0xC7, 0xD3, 0x31, 0xEF, 0x52, 0xD3,
            },
        .data_len = 31,
    },
};

static const size_t apple_menu_options_count =
    sizeof(apple_menu_options) / sizeof(AppleMenuOption);

bool disney_beacons_app_scene_input_apple_data_on_event(
    void *context, SceneManagerEvent event) {
  DisneyBeaconsApp *ble_beacon = context;
  SceneManager *scene_manager = ble_beacon->scene_manager;

  bool consumed = false;

  if (event.type == SceneManagerEventTypeCustom) {
    if (event.event < apple_menu_options_count) {
      const AppleMenuOption *option = &apple_menu_options[event.event];
      memcpy(ble_beacon->beacon_data, option->data, option->data_len);
      disney_beacons_app_update_state(ble_beacon, option->data_len);
      scene_manager_previous_scene(scene_manager);
      consumed = true;
    }
  }

  return consumed;
}

void disney_beacons_app_scene_input_apple_data_on_enter(void *context) {
  DisneyBeaconsApp *ble_beacon = context;
  Submenu *apple = ble_beacon->apple;

  for (size_t x = 0; x < apple_menu_options_count; x++) {
    submenu_add_item(apple, apple_menu_options[x].name, x,
                     disney_beacons_app_scene_menu_apple_callback, ble_beacon);
  }

  view_dispatcher_switch_to_view(ble_beacon->view_dispatcher,
                                 DisneyBeaconsAppViewApple);
}

void disney_beacons_app_scene_input_apple_data_on_exit(void *context) {
  DisneyBeaconsApp *ble_beacon = context;
  submenu_reset(ble_beacon->apple);
}
