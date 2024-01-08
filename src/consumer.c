//
// Created by oleg on 07.01.24.
//

#include "consumer.h"


void consume_messages(amqp_connection_state_t conn, char const *queue_name, int (*routine)(amqp_envelope_t*))
{
    amqp_envelope_t envelope;
    amqp_channel_t channel = 1; // Assuming you're using channel 1
    amqp_basic_consume(conn, channel, amqp_cstring_bytes(queue_name), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
    amqp_rpc_reply_t res;

    while (1) {
        amqp_maybe_release_buffers(conn);
        res = amqp_consume_message(conn, &envelope, NULL, 0);
        if (AMQP_RESPONSE_NORMAL == res.reply_type) {
            int result = routine(&envelope);
            if (!result) {
                amqp_basic_reject(conn, channel, envelope.delivery_tag, 1);
            } else {
                amqp_basic_ack(conn, channel, envelope.delivery_tag, 0);
            }
        } else {
            printf("{%i} - {%s}", res.library_error, amqp_error_string2(res.library_error));
            break;
        }
    }
}

amqp_envelope_t* consume_N_messages(amqp_connection_state_t conn, char const *queue_name, int N, int channel)
{
    amqp_channel_open(conn, channel);
    amqp_basic_consume(conn, channel, amqp_cstring_bytes(queue_name), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
    amqp_rpc_reply_t res;

    amqp_envelope_t *envelopes = (amqp_envelope_t*) malloc(sizeof(amqp_envelope_t) * N);
    if (envelopes == NULL) {
        fprintf(stderr, "Failed to allocate memory for envelopes\n");
        exit(-1);
    }


    for (int i = 0; i < N; i++) {
        amqp_maybe_release_buffers(conn);
        res = amqp_consume_message(conn, &envelopes[i], NULL, 0);
        if (AMQP_RESPONSE_NORMAL == res.reply_type) {
            amqp_basic_ack(conn, channel, envelopes[i].delivery_tag, 0);
        } else {
            printf("{%i} - {%s}", res.library_error, amqp_error_string2(res.library_error));
            fflush(stdout);
            break;
        }
        if (envelopes[i].message.body.bytes == NULL) {
            perror("Message body is null\n");
            exit(-1);
        }
    }


    return envelopes;
}
