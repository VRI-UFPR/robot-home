/* BSD 2-Clause License
 * Copyright (c) 2024, Visao Robotica e Imagem (VRI)
 *  - Felipe Bombardelli <felipebombardelli@gmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// ============================================================================
//  Header
// ============================================================================

// #include <errno.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <ufr.h>
#include <unistd.h>
#include <vector>

using namespace cv;

typedef struct {
    float pos_x, pos_y, pos_th;
    float vel, rotvel;

    time_t last;
} pioneer_t;

typedef struct {
    float angle_min, angle_max, angle_increment;
    float time_increment, scan_time;
    float range_min, range_max;
    std::vector<float> ranges;
    std::vector<float> intensities;
    time_t last;
} lidar_t;

lidar_t g_lidar;
pioneer_t g_base;

uint32_t g_cam1_size = 0;
uint8_t g_cam1_data[1024*1024];

uint32_t g_cam2_size = 0;
uint8_t g_cam2_data[1024*1024];


// ============================================================================
//  Private Functions
// ============================================================================





// ============================================================================
//  Main
// ============================================================================

int main() {
    printf("Starting Node\n");

    // Links
    // link_t timer = ufr_subscriber("@new posix:timer @time 1s");
    link_t server = ufr_server_st("@new zmq:socket @coder msgpack @debug 0");

    link_t motors = ufr_publisher("@new ros_humble:topic @msg twist @topic /cmd_vel @debug 0");
    link_t scan = ufr_subscriber("@new ros_humble:topic @msg laserscan @topic /scan @debug 4");
    link_t odom = ufr_subscriber("@new ros_humble:topic @msg pose @topic /odom @debug 0");
    link_t cam1 = ufr_subscriber("@new ros_humble:topic @msg image @topic /camera1");
    link_t cam2 = ufr_subscriber("@new ros_humble:topic @msg image @topic /camera2");

    // link_t cam1 = ufr_subscriber("@new zmq:topic @coder msgpack @port 5001 @host 10.0.0.3");
    // link_t cam2 = ufr_subscriber("@new zmq:topic @coder msgpack @port 5002 @host 10.0.0.3");

    // Robot Variables
    float pos_x=0, pos_y=0, pos_th=0;
    Mat map(320, 320, CV_8UC1);

    // Buffers
    std::vector<uchar> buf;

    // Main loop
    while ( ufr_loop_ok() ) {
        usleep(50000);

        // int topic_id = ufr_recv_2a(&server, &encoders, 100);
        // ufr_get(&encoders, "^ff", &left, &right);

        if ( ufr_recv_async(&cam1) == UFR_OK ) {
            g_cam1_size = ufr_get_nbytes(&cam1);
            const uint8_t* ros_image = ufr_get_rawptr(&cam1);
            // ufr_get_raw(&cam1, g_cam1_data, g_cam1_size);
            // memcpy(g_cam1_data, ros_image, g_cam1_size);
            // printf("opa1 %d %p\n", g_cam1_size, g_cam1_data);
        }

        if ( ufr_recv_async(&cam2) == UFR_OK ) {
            g_cam2_size = ufr_get_nbytes(&cam2);
            const uint8_t* ros_image = ufr_get_rawptr(&cam2);
            // g_cam2_data = ufr_get_raw_ptr(&cam2);
            // ufr_get_raw(&cam2, g_cam2_data, g_cam2_size);
            // memcpy(g_cam2_data, ros_image, g_cam2_size);
            // printf("opa2 %d %p\n", g_cam2_size, g_cam2_data);
        }

        if ( ufr_recv_async(&scan) == UFR_OK ) {
            float dummy;
            ufr_get(&scan, "fffffff", &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy);

            // update global variable
            const size_t size = ufr_get_nbytes(&scan);
            g_lidar.ranges.resize(size);
            // ufr_get_af32(&scan, g_lidar.ranges.data(), size);
            g_lidar.last = time(0);
            // printf("lidar\n");
        }

        if ( ufr_recv_async(&odom) == UFR_OK ) {
            ufr_get(&odom, "fffff", 
                &g_base.pos_x, &g_base.pos_y, &g_base.pos_th, 
                &g_base.vel, &g_base.rotvel);
            g_base.last = time(0);
            // printf("odom\n");
        }

        // SERVIDOR ZMQ
        if ( ufr_recv_async(&server) == UFR_OK ) {
            char command[1024];
            ufr_get(&server, "s", command);
            printf("[INFO:Server]: %s ", command);

            if ( strcmp(command, "odom") == 0 ) {
                ufr_get_eof(&server);
                ufr_put(&server, "fffff\n\n", g_base.pos_x, g_base.pos_y, g_base.pos_th, g_base.vel, g_base.rotvel);
                printf("- OK\n");

            } else if ( strcmp(command, "scan") == 0 ) {
                ufr_get_eof(&server);
                // ufr_put(&server, "af\n", lidar_size, lidar_ranges);
                ufr_put(&server, "\n");
                // printf("%ld - ERROR\n", lidar_size);

            } else if ( strcmp(command, "camera1.jpg") == 0 ) {
                ufr_get_eof(&server);
                ufr_put_raw(&server, g_cam1_data, g_cam1_size);
                ufr_put(&server, "\n");
                ufr_put_eof(&server);
                printf(" - OK\n");

            } else if ( strcmp(command, "camera2.jpg") == 0 ) {
                ufr_get_eof(&server);
                ufr_put_raw(&server, g_cam2_data, g_cam2_size);
                ufr_put(&server, "\n");
                ufr_put_eof(&server);
                printf(" - OK\n");

            } else if ( strcmp(command, "scan.jpg") == 0 ) {
                ufr_get_eof(&server);
                map = 0;
                map.at<uint8_t>(160,160) = 200; 

                const size_t lidar_size = g_lidar.ranges.size();
                float angle = M_PI/2.0;
                float delta_angle = M_PI*2.0 / lidar_size;
                for (size_t i=0; i<lidar_size; i++, angle+=delta_angle) {
                    const float dist = g_lidar.ranges[i];
                    if ( dist == INFINITY ) {
                        continue;
                    }

                    uint32_t y = 160 - ( dist * sin(angle) * 35.0 );
                    uint32_t x = 160 + ( dist * cos(angle) * 35.0 );
                    if ( x < 320 && y < 320 ) {
                        map.at<uint8_t>(y,x) = 255;
                    }
                }

                imencode(".jpg", map, buf);
                ufr_put_raw(&server, buf.data(), buf.size());
                ufr_put(&server, "\n");
                ufr_put_eof(&server);
                printf(" - OK\n");

            } else if ( strcmp(command, "cmd_vel") == 0 ) {
                float vel, rotvel;
                ufr_get(&server, "ff\n", &vel, &rotvel);
                ufr_put(&motors, "ff\n", vel, rotvel);
                ufr_put(&server, "is\n\n", 0, "OK");

                printf("%f %f - OK\n", vel, rotvel);

            } else {
                printf(" - ERROR\n");
            }
        }
        // END SERVIDOR
    }

    return 0;
}
