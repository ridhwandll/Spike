set(CROSS_PLATFORM_FILES
    "include/GLFW/glfw3.h"
    "include/GLFW/glfw3native.h"
    "src/glfw_config.h"
    "src/context.c"
    "src/init.c"
    "src/input.c"
    "src/monitor.c"
    "src/vulkan.c"
    "src/window.c"
)

if(WIN32)
    set(PLATFORM_FILES
        "src/win32_init.c"
        "src/win32_joystick.c"
        "src/win32_monitor.c"
        "src/win32_time.c"
        "src/win32_thread.c"
        "src/win32_window.c"
        "src/wgl_context.c"
        "src/egl_context.c"
        "src/osmesa_context.c"
    )
elseif(UNIX)
    set(PLATFORM_FILES
        "src/x11_init.c"
        "src/x11_monitor.c"
        "src/x11_window.c"
        "src/xkb_unicode.c"
        "src/posix_time.c"
        "src/posix_thread.c"
        "src/glx_context.c"
        "src/egl_context.c"
        "src/osmesa_context.c"
        "src/linux_joystick.c"
    )
endif()

set(SOURCE_FILES ${CROSS_PLATFORM_FILES} ${PLATFORM_FILES})
add_library(GLFW STATIC ${SOURCE_FILES})

if(WIN32)
    target_compile_definitions(GLFW PRIVATE "_CRT_SECURE_NO_WARNINGS" "_GLFW_WIN32")
elseif(UNIX)
    target_compile_definitions(GLFW PRIVATE "_GLFW_X11")
endif()

