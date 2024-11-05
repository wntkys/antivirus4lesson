#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

// Function to connect to Hyprland's IPC socket
int connect_to_hyprland()
{
    struct sockaddr_un addr;
    int sockfd;
    char socket_path[256];

    // Get the XDG_RUNTIME_DIR environment variable
    const char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == NULL)
    {
        fprintf(stderr, "XDG_RUNTIME_DIR not set.\n");
        return -1;
    }

    // Get the HYPRLAND_INSTANCE_SIGNATURE environment variable
    const char *instance_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
    if (instance_signature == NULL)
    {
        fprintf(stderr, "HYPRLAND_INSTANCE_SIGNATURE not set.\n");
        return -1;
    }

    // Construct the socket path
    snprintf(socket_path, sizeof(socket_path), "%s/hypr/%s/.socket2.sock", xdg_runtime_dir, instance_signature);

    // Create a Unix socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        return -1;
    }

    // Set up the address structure
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // Connect to the Hyprland IPC socket
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect error");
        close(sockfd);
        return -1;
    }

    // Set the socket to non-blocking mode
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    return sockfd;
}

// Function to continuously receive updates from the socket
void listen_for_updates()
{
    int sockfd = connect_to_hyprland();
    if (sockfd == -1)
    {
        return;
    }

    char buffer[BUFFER_SIZE];
    while (1)
    {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Attempt to read from the socket
        ssize_t num_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (num_bytes > 0)
        {
            // Successfully received some data, process it
            printf("Received update: %s\n", buffer);
        }
        else if (num_bytes == -1)
        {
            // Non-blocking, so we can safely continue even if there's no data yet
            usleep(100000); // Sleep for 100ms to avoid CPU exhaustion
        }
    }

    // Close the socket when done (though this won't be reached in this example)
    close(sockfd);
}

int main()
{
    // Start listening for updates
    listen_for_updates();

    return 0;
}
