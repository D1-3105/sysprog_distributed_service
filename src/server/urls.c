//
// Created by oleg on 08.01.24.
//

#include "urls.h"

void add_urls(struct _u_instance* ulfius_app)
{
    ulfius_add_endpoint_by_val(ulfius_app, "GET", "/", NULL, 0, &root, NULL);
    ulfius_add_endpoint_by_val(
            ulfius_app, // Your Ulfius instance
            "POST",     // HTTP method
            NULL, // URL prefix
            "/distributed/action/:action", // URL format
            0,         // Priority
            &distributed_action, // Callback function
            NULL       // User data
    );
}
