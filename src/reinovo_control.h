#ifndef REINOVO_CONTROL_H
#define REINOVO_CONTROL_H

/*******************    qt     **********************/
#include "ui_mainwindow.h"
#ifndef Q_MOC_RUN
#include "rviz/panel.h"
#include "QTimer"
#include "QMessageBox"
#include "QDialog"
#endif

/*******************    cpp    **********************/
#include <pthread.h>
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>  
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "vector"

//自定义file操作
#include "file.h"

/*******************    ros    **********************/
#include "ros/ros.h"
#include "tf/tf.h"
#include "std_msgs/String.h"
#include<geometry_msgs/Twist.h>
#include<reinovo_control/nav_goal.h>
#include<reinovo_control/path.h>


#include<reinovo_control/task.h>

#include <std_srvs/SetBool.h>

#include<reinovo_control/pathsrv.h>
#include<reinovo_control/pathserver.h>
#include "reinovo_control/get_navgoal.h"
#include "reinovo_control/ask.h"
#include "reinovo_control/navgoalsrv.h"
#include "reinovo_control/navgoalserver.h"
#include "reinovo_control/goto_navgoal.h"
#include "reinovo_control/actionsrv.h"
#include "reinovo_control/tasksrv.h"
#include "reinovo_control/taskserver.h"

#include "arm_controller/control.h"
#include "arm_controller/move.h"
#include "arm_controller/PickPlace.h"

#include "oryxbot_msgs/centerAction.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

typedef actionlib::SimpleActionClient<oryxbot_msgs::centerAction> center_client;

struct ARMGOAL{
    string goalname;
    arm_controller::control pose;
};
const string teachname[3] = {"goto","argoto","pump"};

using namespace std;

namespace Ui
{
class Form;
}

class ReinovoControl:public rviz::Panel
{
    Q_OBJECT
public:
    ReinovoControl(QWidget* parent=0);
    virtual ~ReinovoControl();
    Ui::Form* ui;
    QTimer* timer_;
    

    ros::NodeHandle nh_;
    //参数等用
    ros::NodeHandle n;
    ros::ServiceClient driver_client;
    ros::ServiceClient slam_client;
    ros::ServiceClient map_client;

    ros::Publisher pub;

    

//其他函数
public:
    string get_time();

//output

    // File ofile;
    // File* pfile;

//按键槽函数
public slots:
    //
    void tabwidget(int index);
    //首页
    bool home_init();
    void freinovo_control();
    void fopen_driver();    //打开驱动
    void fopen_slam();  //打开建图
    void fsave_map();   //保存地图
    void fopen_all();   //open all
    void fother1();   //other1
    void fother2();   //other2
    void fother3();   //other3

    //Teleop
    bool teleop_init();
    void fspeed_enable();   //速度使能
    void fpub_vxplus();    //发布+vx指令
    void fpub_vxminus();    //发布-vx指令
    void fpub_vyplus();    //发布+vy指令
    void fpub_vyminus();    //发布-vy指令
    void fpub_vthplus();    //发布+vth指令
    void fpub_vthminus();    //发布-vth指令
    void fvel_stop();    //发布stop指令

    //导航
    bool nav_init();
    void frefresh_map();       //地图列表
    void fswitch_map();     //切换地图
    void fdelete_map();     //删除地图
    void fopen_nav();       //开启、关闭导航
    void frefresh_target();    //导航点列表
    void fgoto_target();    //goto
    void fdelete_target();  //删除导航点
    void fget_gesture();    //获取姿态
    void fsave_target();    //保存目标点

    //arm
    bool arm_init();//初始化
    bool arm_refresh();
    void fopen_arm();   //开启关闭手臂
    void fpump();   //气泵
    void funlock_arm();     //电机锁
    void fopen_cam();       //打开相机
    void fplusx();          //x移动
    void fplusy();          //y移动
    void fplusz();          //z
    void fredx();
    void fredy();
    void fredz();
    void fmicro();          //微调
    void farmsave_target(); //保存目标点
    void farmdelete_target();//删除目标点
    void farmtarget();//目标点勾选
    void far();//ar勾选
    void farm_goto();//goto
    void fenther_teach();//进入示教
    void fsave_armteach();//保存示教
    void position_callback(const arm_controller::control& msg); //回调函数
    //示教所用
    void ftpump();   //气泵
    void ftarm_goto();//goto


    //示教
    bool teach_init();
    void frefresh_teach();  //刷新示教
    void fteach_list(QListWidgetItem* aItem);     //示教列表
    void fteach_rea();      //示教重现
    void fdelete_teach();   //删除示教
    void fcreate_teach();   //创建示教
    void fteach_info(QListWidgetItem* aItem);    //teach_info
    void fadd_action();     //添加
    void fdelete_action();  //删除action
    void fparam_info(int currentRow, int currentColumn);    //参数被修改
    void fmakefile2();      //生成文件
    //组合
    bool combin_init();
    void frefresh_path();   //刷新路径
    void fpath_list(QString text);      //路径列表
    void fdelete_path();    //删除路径
    void fcreate_path();    //创建路径
    void fpath_info();      //路径信息
    void fdelete_target1(); //删除导航点
    void fadd_target();     //添加导航点
    void fmount();          //挂载
    void fmakefile();      //生成文件
    //调度
    bool dispatch_init();
    void fopen_dispatch();  //开启、关闭调度
    void fstart_dispatch(); //开始调度
    void fpause_dispatch(); //暂停调度
    void frecover_dispatch();   //恢复调度
    void frefresh_path1();      //刷新路径
    void fload_path1();         //加载路径
    void fauto_charging();      //自动充电
    void fSlotAppendText(const QString &text);
signals:
    void AppendText(const QString &text);




//数据
public:
    //首页
    uint8_t flag_driver;
    uint8_t flag_slam;
    uint8_t flag_openall;
    uint8_t flag_other[3];
    ros::ServiceClient save_map;
    string strother[3];

    //Teleop
    uint8_t flag_speed;


    //调度
    uint8_t flag_dispatch;
    uint8_t flag_charging;
    float fvoltage_threshold,fcharging_time;

public:
    //Telop
    geometry_msgs::Twist vel;
    ros::Publisher vel_pub;
    //导航

    uint8_t flag_nav;

    ros::ServiceClient get_navgoal;
    ros::ServiceClient save_navgoal;
    ros::ServiceClient get_path;
    ros::ServiceClient pub_path;
    ros::ServiceClient key_client;
    ros::ServiceClient get_pose;
    ros::ServiceClient get_map;
    ros::ServiceClient switch_map;
    ros::ServiceClient delete_map;
    ros::ServiceClient goto_pose;
    ros::ServiceClient open_dispatch;

    //arm
    ros::Publisher armvel_pub;
	ros::Subscriber arm_sub;
    ros::ServiceClient pump_client;
    ros::ServiceClient unlock_client;
    ros::ServiceClient armgoto_client;
    ros::ServiceClient argoto_client;
    arm_controller::control arm_pos;
    vector<reinovo_control::task> v_taskarm;
    reinovo_control::task task_arm;

    QMetaObject::Connection Connection_pump,Connection_goto;
    uint8_t flag_arm;
    uint8_t flag_cam;
    uint8_t flag_teach;
    float arm_cmd;
    Yaml arm_yaml;
    vector<ARMGOAL> arm_goal;

    //示教
    ros::ServiceClient get_actiontem;
    ros::ServiceClient get_task;
    ros::ServiceClient save_task;
    ros::ServiceClient suspend_client;
    center_client client;
    vector<reinovo_control::task> v_task;
    vector<reinovo_control::action> v_actiontem;
    boost::thread actionthread;
    uint8_t dispatch_status;

/********************************************************************
    1-goto
********************************************************************/
    string map_file;
    uint8_t ui_state;
    //所线程
    boost::thread uithread;

    void doneCb(const actionlib::SimpleClientGoalState& state, const oryxbot_msgs::centerResultConstPtr& result);
    void activeCb();
    void feedbackCb(const oryxbot_msgs::centerFeedbackConstPtr& feedback);
    void actionclient();

    void ui_thread();


    vector<string> v_map;
    vector<reinovo_control::path> v_path;
    vector<reinovo_control::nav_goal> v_navgoal;
};



#endif
