#pragma once
#include <cstdint>

//object id always 1
namespace wl_display {
    enum class Request : uint16_t {
        SYNC = 0, GET_REGISTRY = 1
    };
    enum class Event : uint16_t {
        ERROR = 0, DELETE_ID = 1
    };
}

namespace wl_registry {
    enum class Request : uint16_t {
        BIND = 0
    };
    enum class Event : uint16_t {
        GLOBAL = 0, GLOBAL_REMOVE = 1
    };
}

namespace wl_callback {
    enum class Event : uint16_t {
        DONE = 0
    };
}

namespace wl_compositor {
    enum class Request : uint16_t {
        CREATE_SURFACE = 0, CREATE_REGION = 1, RELEASE = 2
    };
}

namespace wl_surface {
    enum class Request : uint16_t {
        DESTROY = 0,
        ATTACH = 1,
        DAMAGE = 2,
        FRAME = 3,
        SET_OPAQUE_REGION = 4,
        SET_INPUT_REGION = 5,
        COMMIT = 6,
        SET_BUFFER_TRANSFORM = 7,
        SET_BUFFER_SCALE = 8,
        DAMAGE_BUFFER = 9,
        OFFSET = 10,
        GET_RELEASE = 11
    };

    enum class Event : uint16_t {
        ENTER = 0,
        LEAVE = 1,
        PREFERRED_BUFFER_SCALE = 2,
        PREFERRED_BUFFER_TRANSFORM = 3,
    };
}

namespace xdg_wm_base {
    enum class Request : uint16_t {
        DESTROY = 0,
        CREATE_POSITIONER = 1,
        GET_XDG_SURFACE = 2,
        PONG = 3
    };
    enum class Event : uint16_t {
        PING = 0,

    };
}

namespace xdg_surface {
    enum class Request : uint16_t {
        DESTROY = 0,
        GET_TOPLEVEL = 1,
        GET_POPUP = 2,
        SET_WINDOW_GEOMETRY = 3,
        ACK_CONFIGURE = 4
    };
    enum class Event : uint16_t {
        CONFIGURE = 0
    };
}

namespace xdg_toplevel {
    enum class Request : uint16_t {
        DESTROY = 0,
        SET_PARENT = 1,
        SET_TITLE = 2,
        SET_APP_ID = 3,
        SHOW_WINDOW_MENU = 4,
        MOVE = 5,
        RESIZE = 6,
        SET_MAX_SIZE = 7,
        SET_MIN_SIZE = 8,
        SET_MAXIMIZED = 9,
        UNSET_MAXIMIZED = 10,
        SET_FULLSCREEN = 11,
        UNSET_FULLSCREEN = 12,
        SET_MINIMIZED = 13,
    };

    enum class Event : uint16_t {
        CONFIGURE = 0,
        CLOSE = 1,
        CONFIGURE_BOUNDS = 2,
        WM_CAPABILITIES = 3,
    };
}

namespace wl_shm {
    enum class Request : uint16_t {
        CREATE_POOL = 0,
        RELEASE = 1,
    };

    enum class Event : uint16_t {
        FORMAT = 0,
    };

    enum class Format : uint32_t {
        ARGB8888 = 0,
        XRGB8888 = 1
    };
}

namespace wl_shm_pool {
    enum class Request : uint16_t {
        CREATE_BUFFER = 0,
        DESTROY = 1,
        RESIZE = 2,
    };
}

namespace zxdg_decoration_manager_v1 {
    enum class Request : uint16_t {
        DESTROY = 0,
        GET_TOPLEVEL_DECORATION = 1
    };
}

namespace zxdg_toplevel_decoration_v1{
    enum class Request : uint16_t {
        DESTROY = 0,
        SET_MODE = 1,
        UNSET_MODE = 2
    };
    enum class Event : uint16_t {
        CONFIGURE = 0
    };
    enum class Mode : uint32_t {
        CLIENT_SIDE = 1, SERVER_SIDE = 2
    };
}

namespace wl_buffer {
    enum class Event : uint16_t {
        RELEASE = 0,
    };

    enum class Request : uint16_t {
        DESTROY = 0,
    };
}

namespace wl_seat {
    enum class Event : uint16_t {
        CAPABILITIES = 0,
        NAME = 1,
    };

    enum class Request : uint16_t {
        GET_POINTER = 0,
        GET_KEYBOARD = 1,
        GET_TOUCH = 2,
        RELEASE = 3,
    };

    enum Capability : uint32_t {
        POINTER = 1,
        KEYBOARD = 2,
        TOUCH = 4
    };
}

namespace wl_pointer {
    enum class Event : uint16_t {
        ENTER = 0,
        LEAVE = 1,
        MOTION = 2,
        BUTTON = 3,
        AXIS = 4,
        FRAME = 5,
        AXIS_SOURCE = 6,
        AXIS_STOP = 7,
        AXIS_DISCRETE = 8,
        AXIS_VALUE120 = 9,
        AXIS_RELATIVE_DIRECTION = 10
    };

    enum class Request : uint16_t {
        SET_CURSOR = 0,
        RELEASE = 1
    };
    enum class Axis : uint32_t {
        VERTICAL_SCROLL = 0,
        HORIZONTAL_SCROLL = 1
    };
}

namespace wl_keyboard {
    enum class Event : uint16_t {
        KEYMAP = 0,
        ENTER = 1,
        LEAVE = 2,
        KEY = 3,
        MODIFIERS = 4,
        REPEAT_INFO = 5
    };

    enum class Request : uint16_t {
        RELEASE = 0
    };

    enum class KeyState : uint32_t {
        RELEASED = 0,
        PRESSED = 1
    };

    enum class KeymapFormat : uint32_t {
        NO_KEYMAP = 0,
        XKB_V1 = 1
    };
}