#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "hyprevent_parser.h"

#define BUFFER_SIZE 4096

int connect_to_hyprland(bool socket_second)
{
    struct sockaddr_un addr;
    int sockfd;
    char socket_path[256];

    const char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == NULL)
    {
        fprintf(stderr, "XDG_RUNTIME_DIR not set.\n");
        return -1;
    }

    const char *instance_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
    if (instance_signature == NULL)
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

// Function to send a command to Hyprland and receive the response
char *hyprland_ipc_request(const char *command)
{
    int sockfd = connect_to_hyprland(true);
    if (sockfd == -1)
    {
        return NULL;
    }

    // Send the command to Hyprland
    send(sockfd, command, strlen(command), 0);

    // Buffer for response
    static char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // Receive the response
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t num_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (num_bytes == -1)
        {
            perror("recv error");
            close(sockfd);
            return NULL;
        }

        char *start = buffer;
        // printf("#>>>>\n%s\n#<<<<\n", buffer);
        for (int i = 0; i < num_bytes; i++)
        {

            if (buffer[i] == '\n')
            {
                HyprEvent a = parseEvent(start, buffer - start + i);

                if (a.type == openwindow)
                    printf("openwindow: %s\n", (char *)a.third);
                else if (a.type == closewindow)
                    printf("closewindow with id:%i\n", a.first);
                else if (a.type == workspacev2)
                    printf("workspacev2: %i\n", a.first);
                else if (a.type == movewindowv2)
                    printf("movewindowv2 with id:%i, workspace:%i\n", a.first, a.second);

                start = buffer + i + 1;
            }
        }
    }

    // Close the socket
    close(sockfd);

    return buffer;
}

// Main function to retrieve the list of applications
void list_apps()
{
    char *response = hyprland_ipc_request("clients");
    if (response)
    {
        printf("Response from Hyprland IPC:\n%s\n", response);
    }
    else
    {
        printf("Failed to communicate with Hyprland.\n");
    }
}

int main()
{
    // Retrieve and display apps
    list_apps();

    return 0;
}
