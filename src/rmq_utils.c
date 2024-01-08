//
// Created by oleg on 07.01.24.
//
#include <bits/types/struct_timeval.h>
#include <string.h>
#include "rmq_utils.h"
#include "rabbitmq-c/tcp_socket.h"

ConnectionProxy* rmq_connection_open(const char* rmq_host, const int rmq_port) {
    amqp_connection_state_t* conn = malloc(sizeof(amqp_connection_state_t));
    *conn = amqp_new_connection();
    amqp_socket_t* socket = amqp_tcp_socket_new(*conn);
    struct timeval tval = {10, 0};
    amqp_socket_open_noblock(socket, rmq_host, rmq_port, &tval);
    amqp_login(*conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
               "guest", "guest");
    amqp_channel_open(*conn, 1);
    ConnectionProxy* proxy = malloc(sizeof(ConnectionProxy));
    proxy->conn = conn;
    return proxy;
}

void queue_declare(ConnectionProxy* conn, const char *queue, const char *bind_to, const char *routing_key, int durable)
{
    if (!conn || !conn->conn) {
        fprintf(stderr, "Invalid connection\n");
        return;
    }

    amqp_queue_declare(
            *conn->conn,
            1,
            amqp_cstring_bytes(queue),
            0,
            durable,
            0,
            1,
            amqp_empty_table
    );
    if (strlen(bind_to)) {
        amqp_exchange_declare(
                *conn->conn,
                1,
                amqp_cstring_bytes(bind_to),
                amqp_cstring_bytes("direct"),
                0, 1, 0, 0,
                amqp_empty_table
        );
        amqp_queue_bind(
                *conn->conn,
                1,
                amqp_cstring_bytes(queue),
                amqp_cstring_bytes(bind_to),
                amqp_cstring_bytes(routing_key),
                amqp_empty_table
        );
    }
}


void die_on_error(int x, char const *context) {
    if (x < 0) {
        fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x));
        exit(1);
    }
}

