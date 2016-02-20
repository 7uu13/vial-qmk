/*
The MIT License (MIT)

Copyright (c) 2016 Fred Sundvik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include "protocol/byte_stuffer.c"
#include "protocol/frame_validator.c"
#include "protocol/frame_router.c"

static uint8_t received_data[256];
static uint16_t received_data_size;

typedef struct {
    uint8_t received_data[256];
    uint16_t sent_data_size;
} receive_buffer_t;

typedef struct {
    receive_buffer_t send_buffers[2];
} router_buffer_t;

router_buffer_t router_buffers[8];

router_buffer_t* current_router_buffer;


Describe(ByteStuffer);
BeforeEach(ByteStuffer) {
    init_byte_stuffer();
    memset(router_buffers, 0, sizeof(router_buffers));
    current_router_buffer = 0;
}
AfterEach(ByteStuffer) {}

typedef struct {
    uint32_t data;
    uint8_t extra[16];
} frame_buffer_t;


void send_data(uint8_t link, const uint8_t* data, uint16_t size) {
    receive_buffer_t* buffer = &current_router_buffer->send_buffers[link];
    memcpy(buffer->received_data + buffer->sent_data_size, data, size);
    buffer->sent_data_size += size;
}

static void receive_data(uint8_t link, uint8_t* data, uint16_t size) {
    int i;
    for(i=0;i<size;i++) {
        byte_stuffer_recv_byte(link, data[i]);
    }
}


Ensure(ByteStuffer, master_broadcast_is_received_by_everyone) {
    frame_buffer_t data;
    data.data = 0xAB7055BB;
    current_router_buffer = router_buffers + 0;
    router_send_frame(0xFF, (uint8_t*)&data, 4);
    assert_that(router_buffers[0].send_buffers[DOWN_LINK].sent_data_size, is_greater_than(0));
    assert_that(router_buffers[0].send_buffers[UP_LINK].sent_data_size, is_equal_to(0));

    current_router_buffer = router_buffers + 1;
    receive_data(UP_LINK, router_buffers[0].send_buffers[DOWN_LINK].received_data,
            router_buffers[0].send_buffers[DOWN_LINK].sent_data_size);
    assert_that(router_buffers[1].send_buffers[DOWN_LINK].sent_data_size, is_greater_than(0));
    assert_that(router_buffers[1].send_buffers[UP_LINK].sent_data_size, is_equal_to(0));
}
