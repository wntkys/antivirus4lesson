#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        workspace,
        workspacev2,
        focusedmon,
        activewindow,
        activewindowv2,
        fullscreen,
        monitorremoved,
        monitoradded,
        monitoraddedv2,
        createworkspace,
        createworkspacev2,
        destroyworkspace,
        destroyworkspacev2,
        moveworkspace,
        moveworkspacev2,
        renameworkspace,
        activespecial,
        activelayout,
        openwindow,
        closewindow,
        movewindow,
        movewindowv2,
        openlayer,
        closelayer,
        submap,
        changefloatingmode,
        urgent,
        screencast,
        windowtitle,
        windowtitlev2,
        togglegroup,
        moveintogroup,
        moveoutofgroup,
        ignoregrouplock,
        lockgroups,
        configreloaded,
        pin
    } HyprEventType;

    typedef struct
    {
        HyprEventType type;
        size_t first;
        size_t second;
        size_t third;
        size_t fourth;
    } HyprEvent;

    HyprEvent parseEvent(char *msg, int len);

#ifdef __cplusplus
}
#endif