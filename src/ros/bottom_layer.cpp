#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sstream>

using namespace std;


#include "ros/ros.h"
#include "tf/tf.h"
#include <tf/transform_listener.h>
#include "reinovo_control/ask.h"
#include "reinovo_control/get_navgoal.h"
#include "reinovo_control/goto_navgoal.h"


#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
MoveBaseClient* navclient;


/*首页
打开、关闭驱动
开始、停止建图
open、close all
其他模块1

*/

/*导航
开启、关闭导航

*/

/*调度
开启、关闭调度

*/

class BootomLayer
{
public:
    BootomLayer();
    //virtual ~BootomLayer();

public:
    //data
    ros::NodeHandle nh;
    ros::NodeHandle n;

    //首页
	ros::ServiceServer key_server;
    ros::ServiceClient shutdown_slam;
    ros::ServiceClient shutdown_bringup;
    ros::ServiceClient shutdown_all;
    ros::ServiceClient shutdown_other1,shutdown_other2,shutdown_other3;

    ros::ServiceClient shutdown_nav;
    ros::ServiceClient shutdown_arm;
    ros::ServiceClient shutdown_dispatch;

	ros::ServiceServer getpose_server;
	ros::ServiceServer gotopose_server;


    move_base_msgs::MoveBaseGoal goal;  //目标点

    tf::TransformListener listener;
	
    bool key_callback(reinovo_control::ask::Request &req,
                        reinovo_control::ask::Response &res);

    bool getpose_callback(reinovo_control::get_navgoal::Request &req,
                        reinovo_control::get_navgoal::Response &res);

    bool gotopose_callback(reinovo_control::goto_navgoal::Request &req,
                            reinovo_control::goto_navgoal::Response &res);



    //底层控制节点
    uint8_t flag_bringup;
    //slam地图构建
    uint8_t flag_slam;
    //开启所有外设
    uint8_t flag_all;
    //其他三功能
    uint8_t flag_other[3];
    //导航
    uint8_t flag_nav;
    //arm控制开启标志位
    uint8_t flag_arm;
    //调度节点
    uint8_t flag_dispatch;


private:
    /* data */

};

bool BootomLayer::key_callback(reinovo_control::ask::Request &req,
                                reinovo_control::ask::Response &res)
{
    //打开关闭slam
    if (req.message == "slam")
    {
        if (flag_slam == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_slam.launch&")<0){
                flag_slam = 10;
                res.success = false;
                res.message = "system(slam) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_slam = 1;
                res.message = "slam 正在开启";
                res.success = true;
            }
        }else if(flag_slam == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_slam.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_slam = 0;
                    res.message = "slam 正在关闭";
                    res.success = true;
                }else{
                    flag_slam = 10;
                    res.message = "slam 关闭失败";
                    res.success = false;
                }
            }else{
                flag_slam = 0;
                res.message = "slam 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message == "bringup"){
        if (flag_bringup == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_bringup.launch&")<0){
                flag_bringup = 10;
                res.success = false;
                res.message = "system(bringup) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_bringup = 1;
                res.message = "bringup 正在开启";
                res.success = true;
            }
        }else if(flag_bringup == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_bringup.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_bringup = 0;
                    res.message = "bringup 正在关闭";
                    res.success = true;
                }else{
                    flag_bringup = 10;
                    res.message = "bringup 关闭失败";
                    res.success = false;
                }
            }else{
                flag_bringup = 0;
                res.message = "bringup 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message == "all"){
        if (flag_all == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_all.launch&")<0){
                flag_all = 10;
                res.success = false;
                res.message = "system(all) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_all = 1;
                res.message = "all 正在开启";
                res.success = true;
            }
        }else if(flag_all == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_all.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_all = 0;
                    res.message = "all 正在关闭";
                    res.success = true;
                }else{
                    flag_all = 10;
                    res.message = "all 关闭失败";
                    res.success = false;
                }
            }else{
                flag_all = 0;
                res.message = "all 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message == "other1"){
        if (flag_other[0] == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_other1.launch&")<0){
                flag_other[0] = 10;
                res.success = false;
                res.message = "system(其他模块1) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_other[0] = 1;
                res.message = "其他模块1 正在开启";
                res.success = true;
            }
        }else if(flag_other[0] == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_other1.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_other[0] = 0;
                    res.message = "其他模块1 正在关闭";
                    res.success = true;
                }else{
                    flag_other[0] = 10;
                    res.message = "其他模块1 关闭失败";
                    res.success = false;
                }
            }else{
                flag_other[0] = 0;
                res.message = "其他模块1 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message == "other2"){
        if (flag_other[1] == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_other2.launch&")<0){
                flag_other[1] = 10;
                res.success = false;
                res.message = "system(其他模块2) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_other[1] = 1;
                res.message = "其他模块2 正在开启";
                res.success = true;
            }
        }else if(flag_other[1] == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_other2.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_other[1] = 0;
                    res.message = "其他模块2 正在关闭";
                    res.success = true;
                }else{
                    flag_other[1] = 10;
                    res.message = "其他模块2 关闭失败";
                    res.success = false;
                }
            }else{
                flag_other[1] = 0;
                res.message = "其他模块2 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message == "other3"){
        if (flag_other[2] == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_other3.launch&")<0){
                flag_other[2] = 10;
                res.success = false;
                res.message = "system(其他模块3) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_other[2] = 1;
                res.message = "其他模块3 正在开启";
                res.success = true;
            }
        }else if(flag_other[2] == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_other3.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_other[2] = 0;
                    res.message = "其他模块3 正在关闭";
                    res.success = true;
                }else{
                    flag_other[2] = 10;
                    res.message = "其他模块3 关闭失败";
                    res.success = false;
                }
            }else{
                flag_other[2] = 0;
                res.message = "其他模块3 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message.find("nav") >= 0&&req.message.find("nav") < 100){
        if (flag_nav == 0 && req.mode == 1){
            string str2 = req.message.substr(req.message.find(":")+1);
            string str1 = "roslaunch reinovo_control robot_nav.launch map_name:=" + str2 + "&";
            cout << str1 <<endl;
            if(system((const char*)str1.c_str())<0){
                flag_nav = 10;
                res.success = false;
                res.message = "system(导航) errorr 开启失败";
            }else{
                flag_nav = 1;
                res.message = "导航 正在开启";
                res.success = true;
            }
        }else if(flag_nav == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_nav.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_nav = 0;
                    res.message = "导航 正在关闭";
                    res.success = true;
                }else{
                    flag_nav = 10;
                    res.message = "导航 关闭失败";
                    res.success = false;
                }
            }else{
                flag_nav = 0;
                res.message = "导航 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else if (req.message == "arm"){
        if (flag_arm == 0 && req.mode == 1){
            if(system("roslaunch reinovo_control robot_arm.launch&")<0){
                flag_arm = 10;
                res.success = false;
                res.message = "system(arm) errorr 开启失败";
                //ROS_ERROR("system() errorr");
            }else{
                flag_arm = 1;
                res.message = "arm 正在开启";
                res.success = true;
            }
        }else if(flag_arm == 1 && req.mode == 0){
            reinovo_control::ask srv;
            srv.request.mode = true;
            if (shutdown_arm.call(srv))
            {
                if (srv.response.success == true)
                {
                    flag_arm = 0;
                    res.message = "arm 正在关闭";
                    res.success = true;
                }else{
                    flag_arm = 10;
                    res.message = "arm 关闭失败";
                    res.success = false;
                }
            }else{
                flag_arm = 0;
                res.message = "arm 未连接，可能已关闭";
                res.success = true;
            }
        }
    }else{
        res.message = "未找到该功能";
        res.success = false;
    }

    return true;
}

bool BootomLayer::getpose_callback(reinovo_control::get_navgoal::Request &req,
                                    reinovo_control::get_navgoal::Response &res)
{
    string stra="";
    n.param<string>("/amcl/base_frame_id",stra,"base_footprint");
    const string &straa=stra;
    string strb="";
    n.param<string>("/amcl/global_frame_id",strb,"map");
    const string &strbb=strb;

    //监听tf
    tf::StampedTransform transform;
    try
    {
        listener.waitForTransform(strbb,straa, ros::Time(0), ros::Duration(3.0));
        listener.lookupTransform(strbb,straa, ros::Time(0), transform);
        res.success = true;
    }
    catch (tf::TransformException &ex) 
    {
        ROS_ERROR("TF may have some problems.\n %s",ex.what());
        ros::Duration(1.0).sleep();
        res.success = false;
        return true;
    }

    res.pose.x = transform.getOrigin().x();
    res.pose.y = transform.getOrigin().y();
    double fdata1,fdata2,fdata3;
    transform.getBasis().getRPY(fdata1,fdata2,fdata3);
    res.pose.theta = fdata3;
    return true;
}

bool BootomLayer::gotopose_callback(reinovo_control::goto_navgoal::Request &req,
                                    reinovo_control::goto_navgoal::Response &res)
{
    while(!navclient->waitForServer(ros::Duration(3.0))){
  		ROS_INFO("Waiting for the move_base action server to come up");
	}

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = req.nav_goal.pose.x;
    goal.target_pose.pose.position.y = req.nav_goal.pose.y;

    geometry_msgs::Quaternion orientation = tf::createQuaternionMsgFromYaw(req.nav_goal.pose.theta);
    goal.target_pose.pose.orientation.x = orientation.x;
    goal.target_pose.pose.orientation.y = orientation.y;
    goal.target_pose.pose.orientation.z = orientation.z;
    goal.target_pose.pose.orientation.w = orientation.w;

	ROS_INFO("Sending goal");
    navclient->sendGoal(goal);
  	navclient->waitForResult();
    if(navclient->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      ROS_INFO("move");
      res.success=true;
    }
    else
    {
      ROS_INFO("failed");
      res.success=false;
    }
    return true;
}


BootomLayer::BootomLayer()
{
    flag_slam=0;
    flag_bringup=0;
    flag_nav=0;
    flag_dispatch=0;
    flag_all = 0;
    flag_other[0]=0;
    flag_other[1]=0;
    flag_other[2]=0;
    key_server=nh.advertiseService("key_server",&BootomLayer::key_callback,this);
    shutdown_slam = nh.serviceClient<reinovo_control::ask>("/robot_slam/shutdown");
    shutdown_bringup = nh.serviceClient<reinovo_control::ask>("/robot_bringup/shutdown");
    shutdown_all = nh.serviceClient<reinovo_control::ask>("/robot_all/shutdown");
    shutdown_other1 = nh.serviceClient<reinovo_control::ask>("/robot_other1/shutdown");
    shutdown_other2 = nh.serviceClient<reinovo_control::ask>("/robot_other2/shutdown");
    shutdown_other3 = nh.serviceClient<reinovo_control::ask>("/robot_other3/shutdown");
    shutdown_nav = nh.serviceClient<reinovo_control::ask>("/robot_nav/shutdown");
    shutdown_arm = nh.serviceClient<reinovo_control::ask>("/robot_arm/shutdown");
    shutdown_dispatch = nh.serviceClient<reinovo_control::ask>("/robot_dispatch/shutdown");
    getpose_server  =   nh.advertiseService("get_pose",&BootomLayer::getpose_callback,this);
    gotopose_server = nh.advertiseService("goto_pose",&BootomLayer::gotopose_callback,this);

    navclient = new MoveBaseClient ("move_base", true);

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "bottom_layer");
    BootomLayer bootomlayer;
    ros::AsyncSpinner spinner(3); 
    spinner.start();
    ros::waitForShutdown();
    return 0;    
}
