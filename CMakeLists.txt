cmake_minimum_required(VERSION 3.27)
project(untitled1 C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
# rmq
include_directories(rabbitmq-c/include)
include_directories(rabbitmq-c/build/include)
link_directories(rabbitmq-c/build/librabbitmq)
add_library(rabbitmq-c SHARED IMPORTED)
set_target_properties(rabbitmq-c PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/rabbitmq-c/build/librabbitmq/librabbitmq.so"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/rabbitmq-c/build/include"
)

add_executable(
        gateway
        src/gateway.c

        src/server/routes.h
        src/server/routes.c
        src/server/urls.c
        src/server/urls.h
        src/publisher.h
        src/consumer.h
        src/consumer.h
        src/consumer.c
        src/publisher.c
        src/rmq_utils.h
        src/rmq_utils.c
        src/shared.h
)

add_executable(
        worker

        src/worker/worker.h
        src/worker/worker.c
        src/worker_main.c
        src/shared.h

        src/publisher.h
        src/consumer.h
        src/consumer.h
        src/consumer.c
        src/publisher.c
        src/rmq_utils.h
        src/rmq_utils.c
)

# Link the libraries
target_link_libraries(gateway PRIVATE rabbitmq-c jansson ulfius)
target_link_libraries(worker PRIVATE rabbitmq-c jansson)
