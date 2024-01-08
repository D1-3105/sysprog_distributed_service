#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "rabbitmq-c/amqp.h"

#include <unistd.h> // For sleep function
#include <stdio.h>
#include <malloc.h>

#include <bits/types/struct_timeval.h>

#include "rmq_utils.h"

void consume_messages
        (
                amqp_connection_state_t conn,
                char const *queue_name,
                int (routine (amqp_envelope_t*))
        );

amqp_envelope_t* consume_N_messages(amqp_connection_state_t conn, char const *queue_name, int N, int channel);

#endif