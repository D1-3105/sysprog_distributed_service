#include <stdio.h>
#include <string.h>
#include "rmq_utils.h"
#include "publisher.h"
#include "consumer.h"

/*int main()
{
    ConnectionProxy conn = rmq_connection_open("localhost", 5673);
    cJSON* message = cJSON_CreateObject();
    cJSON_AddItemToObject(message, "check", cJSON_CreateString("valid"));
    send_json_message(conn.conn, "", "shitty_queue", message);
    free(conn.conn);
    return 0;
}*/
/*
int consumer_routine(amqp_envelope_t* envelope)
{
    char *message_body = malloc(envelope->message.body.len + 1);
    memcpy(message_body, envelope->message.body.bytes, envelope->message.body.len);
    message_body[envelope->message.body.len] = '\0';
    printf("\tNew message: %s \n", message_body);
    fflush(stdout); // Flush the buffer
    return 1;
}

int main()
{
    ConnectionProxy conn = rmq_connection_open("localhost", 5673);
    queue_declare(&conn, "new_queue", "", "new_queue");
    consume_messages(*conn.conn, "new_queue", consumer_routine);
    free(conn.conn);
}
*/