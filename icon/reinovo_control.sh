#! /bin/bash
source ~/.bashrc
source /opt/ros/kinetic/setup.bash
path=$(dirname $(dirname $(dirname $(readlink -f "$(dirname "$0")"))))
source $path/devel/setup.bash
export ROS_MASTER_URI=http://192.168.8.100:11311
roslaunch $path/src/reinovo_control/launch/reinovo_control.launch
