#include<ros/ros.h>
#include "reinovo_control/ask.h"
//空白节点，主要用来关闭launch文件
bool flag=0;
bool shutdown(reinovo_control::ask::Request &req,
                reinovo_control::ask::Response &res)
{
    flag=1;
    res.success=true;
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "blank");
    ros::NodeHandle nh;
    ros::ServiceServer srv_relmove = nh.advertiseService("shutdown", shutdown);
    ros::Rate loop_rate(500);
    while (ros::ok())
    {
        loop_rate.sleep();
        ros::spinOnce();
        if (flag==1)
        {
            ros::shutdown();
        }
        
    }
    return 0;    
}