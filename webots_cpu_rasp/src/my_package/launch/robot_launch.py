# =============================================================================
#  Header
# =============================================================================

import os
import launch
from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from webots_ros2_driver.webots_launcher import WebotsLauncher
from webots_ros2_driver.webots_controller import WebotsController

# =============================================================================
#  Launch Description
# =============================================================================

def generate_launch_description():
    package_dir = get_package_share_directory('my_package')
    robot_description_path = os.path.join(package_dir, 'resource', 'my_robot.urdf')

    # Webots Simulator
    webots = WebotsLauncher(
        world=os.path.join(package_dir, 'worlds', 'world_pioneer.wbt')
    )

    # Webots Controller
    my_robot_driver = WebotsController(
        robot_name='pioneer',
        parameters=[
            {'robot_description': robot_description_path},
        ],
        remappings=[
            ('/pioneer/kinect_color/image_color', '/webcam_image')
        ]
    )

    return LaunchDescription([
        webots,
        my_robot_driver,
        launch.actions.RegisterEventHandler(
            event_handler=launch.event_handlers.OnProcessExit(
                target_action=webots,
                on_exit=[launch.actions.EmitEvent(event=launch.events.Shutdown())],
            )
        )
    ])
