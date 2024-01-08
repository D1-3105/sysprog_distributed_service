//
// Created by oleg on 07.01.24.
//
#include "publisher.h"


void send_message(
        amqp_connection_state_t* conn, char const *exchange, char const *routing_key, char* message_bytes,
        amqp_basic_properties_t* props
)
{
    amqp_bytes_t body = amqp_cstring_bytes(message_bytes);
    body.len = strlen(message_bytes);
    die_on_error(
        amqp_basic_publish(
                *conn, 1, amqp_cstring_bytes(exchange),
                amqp_cstring_bytes(routing_key), 0, 0, props,
                body
        ),
        "Publish failed"
    );
}

void send_json_message(
        amqp_connection_state_t* conn, char const *exchange, char const *routing_key, const json_t* message,
        amqp_basic_properties_t* props
)
{
    char* msg_str = json_dumps(message, 0);
    send_message(conn, exchange, routing_key, msg_str, props);
}
