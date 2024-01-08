//
// Created by oleg on 08.01.24.
//

#include "routes.h"


json_t* reply_to() {
    json_t* message = json_object(); // Create an empty JSON object
    pid_t my_pid = getpid();
    char reverse_queue[50]; // Buffer for the queue name
    sprintf(reverse_queue, "reply_to__%i", my_pid);
    json_object_set(message, "queue", json_string(reverse_queue));
    return message;
}


json_t* form_factorial_remote_message(int on_fact)
{
    json_t* message = json_object();
    json_object_set(message, "var", json_integer(on_fact));
    json_object_set(message, "action", json_integer(FACTORIAL));
    return message;
}

json_t* form_multiply_remote_message(int a, int b)
{
    json_t* message = json_object();
    json_object_set(message, "a", json_integer(a));
    json_object_set(message, "b", json_integer(b));
    json_object_set(message, "action", json_integer(MULTIPLY));

    return message;
}


AvailableActions map_distributed_action(char action[])
{
    if (strcmp(action, "fact") == 0) {
        return FACTORIAL;
    } else if (strcmp(action, "mul") == 0) {
        return MULTIPLY;
    }
    return -1;
};


json_t* remote_task_result(json_t* remote_message, const char* region, int* status)
{
    ConnectionProxy* conn = rmq_connection_open(RMQ_HOST, RMQ_PORT);
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG |
                   AMQP_BASIC_DELIVERY_MODE_FLAG |
                   AMQP_BASIC_REPLY_TO_FLAG;
    props.content_type = amqp_cstring_bytes("application/json");
    props.delivery_mode = 1;
    json_t* reply_queue = reply_to();
    props.reply_to = amqp_cstring_bytes(
            json_dumps(
                    reply_queue,
                    0
                    )
            );
    char* reverse_queue = (char*)json_string_value(json_object_get(reply_queue, "queue"));
    queue_declare(conn, reverse_queue, "",  reverse_queue, 0);
    send_json_message(conn->conn, "machines", region, remote_message, &props);
    amqp_envelope_t *envelopes;
    envelopes = consume_N_messages(*conn->conn, reverse_queue, 1, 2);
    amqp_connection_close(*conn->conn, AMQP_STATUS_OK);
    free(conn->conn);
    json_t* message = json_loads(envelopes[0].message.body.bytes, 0, 0);
    *status = (int) json_integer_value(json_object_get(message, "status"));
    json_object_del(message, "status");
    return message;
}


int distributed_action(const struct _u_request *request, struct _u_response *response, void *user_data)
{
    json_t *body;
    body = ulfius_get_json_body_request(request, 0);
    if (!body) {
        perror("Json decoding error occurred");
        ulfius_set_json_body_response(
                response,
                422,
                json_loads("{\"detail\": \"JSON decoding error!\"}", 0, 0)
        );
        return U_OK;
    }
    char *action;
    strcpy(action, (char*) u_map_get(request->map_url, "action"));
    json_t* message;
    switch (map_distributed_action(action)) {
        case 0:
            message = form_factorial_remote_message(
                    (int) json_integer_value((json_t*) json_object_get(body, "on_fact"))
            );
            break;
        case 1:
            message = form_multiply_remote_message(
                    (int) json_integer_value((json_t*) json_object_get(body, "a")),
                    (int) json_integer_value((json_t*) json_object_get(body, "b"))
            );
            break;
        default:
            ulfius_set_json_body_response(
                    response,
                    422,
                    json_loads("{\"detail\": \"Unexpected action!\"}", 0, 0)
            );
            return U_OK;
    }
    int* status_holder = (int*) malloc(sizeof(int));
    json_t* result = remote_task_result(
            message,
            json_string_value(json_object_get(body, "region")),
            status_holder
    );
    ulfius_set_json_body_response(
            response,
            *status_holder,
            result
    );
    return U_CALLBACK_COMPLETE;
}


int root(const struct _u_request *request, struct _u_response *response, void *user_data)
{
    json_t* body = json_loads("{\"status\": \"ULFIUS IS ALIVE\"}", 0, 0);
    ulfius_set_json_body_response(response, 200, body);
    ulfius_add_header_to_response(response, "Content-Type", "application/json");

    return U_CALLBACK_COMPLETE;
}

