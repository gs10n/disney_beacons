#include "scenes.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const disney_beacons_app_on_enter_handlers[])(void*) = {
#include "scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const disney_beacons_app_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const disney_beacons_app_on_exit_handlers[])(void* context) = {
#include "scene_config.h"
};
#undef ADD_SCENE

// Initialize scene handlers configuration structure
const SceneManagerHandlers disney_beacons_app_scene_handlers = {
    .on_enter_handlers = disney_beacons_app_on_enter_handlers,
    .on_event_handlers = disney_beacons_app_on_event_handlers,
    .on_exit_handlers = disney_beacons_app_on_exit_handlers,
    .scene_num = DisneyBeaconsAppSceneNum,
};
