//
// Created by oleg on 07.01.24.
//

#ifndef RMQ_UTILS_H
#define RMQ_UTILS_H

#include <stdlib.h>
#include "rabbitmq-c/amqp.h"
#include "malloc.h"

typedef struct {
    amqp_connection_state_t* conn;
} ConnectionProxy;

ConnectionProxy* rmq_connection_open(const char* rmq_host, int rmq_port);
void queue_declare(ConnectionProxy* conn, const char *queue, const char *bind_to, const char *routing_key, int durable);
void die_on_error(int x, char const *context);


#endif //RMQ_UTILS_H
