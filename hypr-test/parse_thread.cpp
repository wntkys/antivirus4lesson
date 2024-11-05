#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <functional>

#include "readerwriterqueue.h"
#include "hyprevent_parser.h"
#include <thread>
#include <iostream>

using namespace moodycamel;

#define BUFFER_SIZE 4096

int connect_to_hyprland(bool socket_second)
{
    struct sockaddr_un addr;
    int sockfd;
    char socket_path[256];

    const char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == nullptr)
    {
        fprintf(stderr, "XDG_RUNTIME_DIR not set.\n");
        return -1;
    }

    const char *instance_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
    if (instance_signature == nullptr)
    {
        fprintf(stderr, "HYPRLAND_INSTANCE_SIGNATURE not set.\n");
        return -1;
    }

    snprintf(socket_path, sizeof(socket_path), "%s/hypr/%s/.socket%s.sock", xdg_runtime_dir, instance_signature, socket_second ? "2" : "");

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect error");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void hyprland_sock2listen(std::function<void(HyprEvent)> enqueue_event)
{
    int sockfd = connect_to_hyprland(true);
    if (sockfd == -1)
    {
        return;
    }

    static char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    while (true)
    {
        ssize_t num_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (num_bytes == -1)
        {
            perror("recv error");
            close(sockfd);
            return;
        }

        char *start = buffer;
        for (int i = 0; i < num_bytes; i++)
        {

            if (buffer[i] == '\n')
            {
                HyprEvent event = parseEvent(start, buffer - start + i);

                if (event.type != pin)
                    enqueue_event(event);

                start = buffer + i + 1;
            }
        }
    }
    close(sockfd);

    return;
}

// Main function to retrieve the list of applications

int main()
{

    ReaderWriterQueue<HyprEvent> rwq;

    auto enqueue_event = [&rwq](HyprEvent event)
    {
        rwq.enqueue(event);
    };

    // Launch the socket listening in a separate thread
    std::thread listener_thread([&]()
                                { hyprland_sock2listen(enqueue_event); });

    // Now you can process the events in the main thread (or another thread)
    while (true)
    {
        HyprEvent event;
        event.type = pin;
        if (rwq.try_dequeue(event))
        {
            // Process the event here
            switch (event.type)
            {
            case openwindow:
                std::cout << "window open:" << std::endl;
                break;
            case closewindow:
                std::cout << "window close:" << std::endl;
                break;
            case movewindowv2:
                std::cout << "window move:" << std::endl;
                break;
            case workspacev2:
                std::cout << "workspace change:" << std::endl;
                break;

            default:
                break;
            }
        }
    }

    // Make sure to join the thread if the program ends
    listener_thread.join();

    return 0;
}
