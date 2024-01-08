//
// Created by oleg on 08.01.24.
//

#ifndef ROUTES_H
#define ROUTES_H

#include "ulfius.h"

#include "jansson.h"

#include "string.h"

#include "../publisher.h"
#include "../rmq_utils.h"
#include "../consumer.h"
#include "../shared.h"

#define RMQ_HOST "localhost"
#define RMQ_PORT 5673


int distributed_action(const struct _u_request *request, struct _u_response *response, void *user_data);
int root(const struct _u_request *request, struct _u_response *response, void *user_data);

#endif //ROUTES_H
