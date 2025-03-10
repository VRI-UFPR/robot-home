/**
 * Pioneer-P3DX communication library
 *  Copyright (C) 2024  Visao Robotica e Imagem (VRI)
 *   - Felipe Bombardelli <felipebombardelli@gmail.com>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * * */

// ============================================================================
//  Header
// ============================================================================

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ufr.h>
#include <signal.h>
#include <math.h>

#include "pioneer_p3dx.h"

// ============================================================================
//  Main Pioneer
// ============================================================================

int main(int argc, char *argv[]) {
    link_t timer = ufr_subscriber("@new posix:timer @time 200ms @debug 0");
    link_t vel_cmd = ufr_subscriber("@new ros_humble:topic @msg ros_humble:twist @topic /cmd_vel @debug 0");
    link_t odom = ufr_publisher("@new ros_humble:topic @coder ros_humble:pose @topic /odom @debug 0");

    pioneer_connect("/dev/pioneer", 0);
    pioneer_disable_sonars();
    // pioneer_enable_motors();

    // Main loop
    // signal (SIGINT, my_handler);
    int count = 0;
    float vel, rotvel, pos_th;
    int pos_x, pos_y;
    while( ufr_loop_ok() ) {
        if ( ufr_recv_async(&vel_cmd) == UFR_OK ) {
            ufr_get(&vel_cmd, "ff", &vel, &rotvel);

            int16_t i16_vel = (int16_t) vel;
            int16_t i16_rotvel = (int16_t) rotvel;
            pioneer_vel( i16_vel );
            pioneer_rotvel( i16_rotvel );
            printf("[LOG]: %d %d\n", i16_vel, i16_rotvel);
            count = 0;
        }

        if ( ufr_recv_async(&timer) == UFR_OK ) {
            if ( count >= 3 ) {
                // vel = 0;
                // rotvel = 0;
                pioneer_vel(vel);
                pioneer_rotvel(rotvel);
                count = 0;
                // printf("[LOG]: Timeout\n");
            }
            count += 1;
        }

        // publishes the odometry
        pioneer_read(&pos_x, &pos_y, &pos_th);
        // printf("%d %d %f %f %f\n", pos_x, pos_y, pos_th, vel, rotvel);
        ufr_put(&odom, "iifff\n", pos_x, pos_y, pos_th*M_PI/180.0, vel, rotvel);

        // wait 50ms
        usleep(50000);
    }

    // End
    pioneer_close();
    return 0;
}