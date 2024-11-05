// Made for hyprland

#include "hyprevent_parser.h"

#include <stdio.h>
#include <stdlib.h>

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#ifdef __cplusplus
extern "C"
{
#endif

    const char *KNOWN_EVENT_NAMES[] = {
        "workspace",
        "workspacev2",
        "focusedmon",
        "activewindow",
        "activewindowv2",
        "fullscreen",
        "monitorremoved",
        "monitoradded",
        "monitoraddedv2",
        "createworkspace",
        "createworkspacev2",
        "destroyworkspace",
        "destroyworkspacev2",
        "moveworkspace",
        "moveworkspacev2",
        "renameworkspace",
        "activespecial",
        "activelayout",
        "openwindow",
        "closewindow",
        "movewindow",
        "movewindowv2",
        "openlayer",
        "closelayer",
        "submap",
        "changefloatingmode",
        "urgent",
        "screencast",
        "windowtitle",
        "windowtitlev2",
        "togglegroup",
        "moveintogroup",
        "moveoutofgroup",
        "ignoregrouplock",
        "lockgroups",
        "configreloaded",
        "pin"};

    HyprEvent parseEvent(char *msg, int len)
    {
        int event_name_length = -1;
        for (int i = 0; i < len; i++)
        {
            if (msg[i] == '>')
            {
                event_name_length = i;
                break;
            }
        }
        if (event_name_length == -1)
        {
            return (HyprEvent){.type = pin};
        }

        for (int i = 0; i < COUNT_OF(KNOWN_EVENT_NAMES); i++)
        {
            if (strlen(KNOWN_EVENT_NAMES[i]) != event_name_length)
                continue;

            if (strncmp(KNOWN_EVENT_NAMES[i], msg, event_name_length) == 0)
            {

                int params_lengths[4];
                char *params_starts[4];

                {
                    int start = event_name_length + 2;
                    int current_param = 0;
                    for (int j = event_name_length + 2 + 1; j < len; j++)
                    {
                        bool last = j == len - 1;

                        if (msg[j] == ',' || last)
                        {
                            params_lengths[current_param] = j - start + (last ? 1 : 0);
                            params_starts[current_param] = msg + start;
                            start = j + 1;
                            current_param++;
                            if (current_param >= 3)
                            {
                                // todo
                            }
                        }
                    }
                }

                switch ((HyprEventType)(i))
                {
                case workspace:
                    break; // implement later
                case workspacev2:
                {

                    char *second_buf = (char *)malloc(params_lengths[1] + 1);
                    strncpy(second_buf, params_starts[1], params_lengths[1]);

                    return (HyprEvent){.type = workspacev2,
                                       .first = strtoull(params_starts[0], NULL, 16),
                                       .second = (size_t)second_buf};
                }
                break;
                case focusedmon: // implement later
                    break;
                case activewindow: // implement later
                    break;
                case activewindowv2:
                {
                    return (HyprEvent){.type = activewindowv2,
                                       .first = strtoull(params_starts[0], NULL, 16)};
                }
                break;
                case fullscreen: // implement later
                    break;
                case monitorremoved: // implement later
                    break;
                case monitoradded: // implement later
                    break;
                case monitoraddedv2: // implement later
                    break;
                case createworkspace: // implement later
                    break;
                case createworkspacev2: // implement later
                    break;
                case destroyworkspace: // implement later
                    break;
                case destroyworkspacev2: // implement later
                    break;
                case moveworkspace: // implement later
                    break;
                case moveworkspacev2: // implement later
                    break;
                case renameworkspace: // implement later
                    break;
                case activespecial: // implement later
                    break;
                case openwindow:
                {
                    char *second_buf = (char *)malloc(params_lengths[1] + 1);
                    strncpy(second_buf, params_starts[1], params_lengths[1]);

                    char *third_buf = (char *)malloc(params_lengths[2] + 1);
                    strncpy(third_buf, params_starts[2], params_lengths[2]);

                    char *fourth_buf = (char *)malloc(params_lengths[3] + 1);
                    strncpy(fourth_buf, params_starts[3], params_lengths[3]);

                    return (HyprEvent){.type = openwindow,
                                       .first = strtoull(params_starts[0], NULL, 16),
                                       .second = (size_t)second_buf,
                                       .third = (size_t)third_buf,
                                       .fourth = (size_t)fourth_buf};
                }
                break;
                case closewindow:
                    return (HyprEvent){.type = closewindow,
                                       .first = strtoull(params_starts[0], NULL, 16)};
                    break;
                case movewindow: // implement later
                    break;
                case movewindowv2:
                {
                    char *third_buf = (char *)malloc(params_lengths[1] + 1);
                    strncpy(third_buf, params_starts[1], params_lengths[1]);

                    return (HyprEvent){.type = movewindowv2,
                                       .first = strtoull(params_starts[0], NULL, 16),
                                       .second = strtoull(params_starts[1], NULL, 16),
                                       .third = (size_t)third_buf};
                }
                break;
                case openlayer: // implement later
                    break;
                case closelayer: // implement later
                    break;
                case submap: // implement later
                    break;
                case changefloatingmode: // implement later
                    break;
                case urgent: // implement later
                    break;
                case screencast: // implement later
                    break;
                case windowtitle: // implement later
                    break;
                case windowtitlev2:
                {
                    char *second_buf = (char *)malloc(params_lengths[1] + 1);
                    strncpy(second_buf, params_starts[1], params_lengths[1]);

                    return (HyprEvent){.type = windowtitlev2,
                                       .first = strtoull(params_starts[0], NULL, 16),
                                       .second = (size_t)second_buf};
                }
                break;
                case togglegroup: // implement later
                    break;
                case moveintogroup: // implement later
                    break;
                case moveoutofgroup: // implement later
                    break;
                case ignoregrouplock: // implement later
                    break;
                case lockgroups: // implement later
                    break;
                case configreloaded: // implement later
                    break;
                case pin: // implement later
                    break;

                default:
                    break;
                }
                break;
            }
        }
        return (HyprEvent){.type = pin};
    }

#ifdef __cplusplus
}
#endif