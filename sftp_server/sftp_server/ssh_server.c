#include <cstdio>
#include <libssh/server.h>

struct winsize
{
    int ws_row;
    int ws_col;
    int ws_xpixel;
    int ws_ypixel;
};

static void handle_session(ssh_event event, ssh_session session) {
   
}

static void* session_thread(void* arg) {
    ssh_session session = arg;
    ssh_event event;

    event = ssh_event_new();
    if (event != NULL) {
        /* Blocks until the SSH session ends by either
         * child thread exiting, or client disconnecting. */
        handle_session(event, session);
        ssh_event_free(event);
    }
    else {
        fprintf(stderr, "Could not create polling context\n");
    }
    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
}

int main2(int argc, char** argv) {
    char ip = "127.0.0.1";
    char port = "22";
    ssh_init();
    ssh_event event;/* Create and configure the ssh session. */
    auto sshbind = ssh_bind_new();
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, ip);
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &port);
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, "C:\\Users\\user\\Desktop\\priv_key.txt");

    /* Listen on `port' for connections. */
    if (ssh_bind_listen(sshbind) < 0) {
        printf("Error listening to socket: %s\n", ssh_get_error(sshbind));
        return -1;
    }
    printf("Listening on port %d.\n", port);
    /* Loop forever, waiting for and handling connection attempts. */
    while (1) {
        ssh_session session = ssh_new();
        if (ssh_bind_accept(sshbind, session) == SSH_ERROR) {
            fprintf(stderr, "Error accepting a connection: %s'.\n",
                ssh_get_error(sshbind));
            return -1;
        }
        printf("Accepted a connection.\n");
        //event = ssh_event_new();
        CreateThread(NULL, 0, session_thread, session, 0, NULL);
        //std::thread(sessionHandler, event, session).detach();
    return 0;
}
