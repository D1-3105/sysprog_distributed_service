//
// Created by oleg on 07.01.24.
//

#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#define RMQ_PORT 5673
#define RMQ_HOST "localhost"

#include "stdio.h"
#include <stdlib.h>
#include "string.h"

#include "rabbitmq-c/amqp.h"
#include "jansson.h"

#include "rmq_utils.h"


void send_json_message(
        amqp_connection_state_t* conn,
        char const *exchange,
        char const *routing_key,
        const json_t* message,
        amqp_basic_properties_t* props
);



#endif //PUBLISHER_H_
