//
// Created by oleg on 08.01.24.
//

#include "worker.h"


char WORKER_RMQ_HOST[1024];
int WORKER_RMQ_PORT;

int mul_callback(json_t *message)
{
    int a = json_integer_value(json_object_get(message, "a"));
    int b = json_integer_value(json_object_get(message, "b"));
    int result = a * b;
    return result;
}


int factorial(int n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}


int fact_callback(json_t *message)
{
    int n = json_integer_value(json_object_get(message, "var"));
    return factorial(n);
}


void basic_callback(json_t* message, json_t* reply_to, int (call_me(json_t*))) {
    ConnectionProxy *proxy = rmq_connection_open(WORKER_RMQ_HOST, WORKER_RMQ_PORT);
    json_t *result_message = json_loads("{\"status\": 200}", 0, 0);
    int result = call_me(message);
    json_object_set(result_message, "result", json_integer(result));
    char* reply_q = json_string_value(json_object_get(reply_to, "queue"));
    send_json_message(
            proxy->conn,
            "",
            reply_q,
            result_message,
            NULL
    );
    amqp_connection_close(*proxy->conn, 200);
}


int call_me(amqp_envelope_t* envelope)
{
    char* raw_message = malloc(envelope->message.body.len + 1);
    strncpy(raw_message, envelope->message.body.bytes, envelope->message.body.len);
    raw_message[envelope->message.body.len] = '\0'; // Null-terminate the string
    printf("\tNew message: %s\n", raw_message);
    json_t *message = json_loads(raw_message, 0, 0);
    json_t *reply_to = json_loads(envelope->message.properties.reply_to.bytes, 0, 0);
    int action = json_integer_value(json_object_get(message, "action"));
    switch (action) {
        case MULTIPLY:
            basic_callback(message, reply_to, mul_callback);
            break;
        case FACTORIAL:
            basic_callback(message, reply_to, fact_callback);
            break;
        default:
            return 0;
    }
    return 1;
}


void run()
{
    strcpy(WORKER_RMQ_HOST, getenv("rmq_host"));
    WORKER_RMQ_PORT = atoi(getenv("rmq_port"));
    const char* region = getenv("region");
    if (!region) {
        perror("\"region\" must be set!");
        exit(-1);
    }
    ConnectionProxy *proxy = rmq_connection_open(WORKER_RMQ_HOST, WORKER_RMQ_PORT);
    queue_declare(proxy, region, "machines", region, 1);
    consume_messages(*proxy->conn, region, call_me);
}
