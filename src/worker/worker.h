//
// Created by oleg on 08.01.24.
//

#ifndef WORKER_H
#define WORKER_H

#include "string.h"

#include "rabbitmq-c/amqp.h"

#include "jansson.h"

#include "../publisher.h"
#include "../consumer.h"
#include "../rmq_utils.h"
#include "../shared.h"

void run();

#endif //WORKER_H
