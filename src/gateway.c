//
// Created by oleg on 08.01.24.
//

#include <netinet/in.h>
#include "server/urls.h"
#include "ulfius.h"


int main() {
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, 8000, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return (1);
    }

    // Endpoint list declaration
    add_urls(&instance);
    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);

        // Wait for the user to press <enter> on the console to quit the application
        getchar();
    } else {
        fprintf(stderr, "Error starting framework\n");
    }
    printf("End framework\n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
}
