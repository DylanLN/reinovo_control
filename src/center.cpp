#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "XmlRpc.h"

#include <ros/ros.h>
#include <std_srvs/SetBool.h>

//oryx
#include "actionlib/server/simple_action_server.h"
#include <arm_controller/PickPlace.h>
#include <arm_controller/move.h>
#include "oryxbot_msgs/centerAction.h"
#include <oryxbot_msgs/nav_goal.h>
#include <oryxbot_msgs/set_zero.h>
#include <oryxbot_msgs/SetCharge.h>
#include <oryxbot_msgs/SetRelativeMove.h>
#include <oryxbot_msgs/track.h>

#include <reinovo_control/action.h>
#include <reinovo_control/path.h>
#include <reinovo_control/nav_goal.h>
#include <reinovo_control/task.h>

typedef actionlib::SimpleActionServer<oryxbot_msgs::centerAction> center_server;

using namespace std;

//调度数据
typedef struct {
    reinovo_control::nav_goal navgoal;
    reinovo_control::task task;
}NavGoal;


typedef struct {
    string path_name;
    uint16_t number;
    vector<NavGoal> navgoal;
}Path;

vector<reinovo_control::nav_goal> navgoal;
vector<reinovo_control::task> task;
vector<reinovo_control::path> path;


/**************************************************************
//类
**************************************************************/
class CENTRALCONTROLLING
{
public:
    CENTRALCONTROLLING();
    ros::NodeHandle nh;
    ros::NodeHandle nh_;
    ros::ServiceClient client_nav,cilent_pick,client_charging,client_relativemove,client_gotopos,client_track;
    ros::ServiceServer server_suspend;
    center_server centerserver;
    uint8_t suspend;
    

    bool readgoal();
    bool readtask();
    bool readpath();
    bool load_data(string path_name);

    Path cur_path;

    void delay_s(float time);
    int action(string actionname,vector<float> parameter);
    void execute(const oryxbot_msgs::centerGoalConstPtr &goal, center_server *as);
    bool suspend_callback(std_srvs::SetBool::Request &req,
                          std_srvs::SetBool::Response &res);
private:
    /*data*/
};



/***********************************************************

    读取导航点

***********************************************************/
bool CENTRALCONTROLLING::readgoal()
{
    XmlRpc::XmlRpcValue scanner_params;
    nh.getParam("/center/navgoal", scanner_params);
    if (scanner_params.size() == 0)
    {
        ROS_INFO_STREAM("未找到goal");
        return false;
    }
    
    for (size_t i = 0; i < scanner_params.size(); i++)
    {
        reinovo_control::nav_goal data;
        data.name = (string)scanner_params[i]["name"];
        data.pose.x = scanner_params[i]["pose.x"];
        data.pose.y = scanner_params[i]["pose.y"];
        data.pose.theta = scanner_params[i]["pose.th"];
        navgoal.push_back(data);
    }
    // for (size_t i = 0; i < navgoal.size() ; i++)
    // {
    //     cout << " name :" << navgoal[i].name << endl;
    //     cout << "pose x:" << navgoal[i].pose.x << endl;
    //     cout << "pose y:" << navgoal[i].pose.y << endl;
    //     cout << "pose th:" << navgoal[i].pose.theta << endl;
    // }
    return true;
}

//读取任务
bool CENTRALCONTROLLING::readtask()
{
    XmlRpc::XmlRpcValue scanner_params;
    nh.getParam("/center/task", scanner_params);
    if (scanner_params.size() == 0)
    {
        ROS_INFO_STREAM("未找到task");
        return false;
    }
    for(size_t i = 0; i<scanner_params.size(); ++i)
    {
        reinovo_control::task data;
        string name = (string)scanner_params[i]["name"];
        data.name = name;
        int step = (int)scanner_params[i]["step"];
        data.step = (uint8_t)step;
        
        for (size_t j = 0; j < scanner_params[i]["action"].size(); j++)
        {
            reinovo_control::action action;
            data.action.push_back(action);
            string action_name = (string)scanner_params[i]["action"][j]["name"];
            data.action[j].name = action_name;
            int number = (int)scanner_params[i]["action"][j]["number"];
            data.action[j].number = (uint8_t)number;

            if (scanner_params[i]["action"][j]["param_name"].size() > 0)
            {
                for (size_t k = 0; k < scanner_params[i]["action"][j]["param_name"].size(); k++)
                {
                    string param_name = (string)scanner_params[i]["action"][j]["param_name"][k];
                    data.action[j].param_name.push_back(param_name);
                }
            }
            if (scanner_params[i]["action"][j]["param"].size() > 0)
            {
                for (size_t k = 0; k < scanner_params[i]["action"][j]["param"].size(); k++)
                {
                    double param = (double)scanner_params[i]["action"][j]["param"][k];
                    data.action[j].param.push_back(param);
                }
            }
        }
        task.push_back(data);
    }

    //debug
    // for (size_t i = 0; i < task.size(); i++)
    // {
    //     cout << "---------------------------------------------" << endl;
    //     cout << "task name :" << task[i].name << endl;
    //     ROS_INFO("STEP : %d" , task[i].step);

    //     for (size_t j = 0; j < task[i].action.size(); j++)
    //     {
    //         cout << "+++++++++++++++++++++++++++++" << endl;
    //         cout << task[i].action[j].name << endl;
    //         ROS_INFO("number : %d" ,task[i].action[j].number);

    //         if (task[i].action[j].param_name.size() > 0)
    //         {
    //             for (size_t k = 0; k < task[i].action[j].param_name.size(); k++)
    //             {
    //                 cout << task[i].action[j].param_name[k] << " || ";
    //             }
    //             cout << " " <<endl;
    //         }
    //         if (task[i].action[j].param.size() > 0)
    //         {
    //             for (size_t k = 0; k < task[i].action[j].param.size(); k++)
    //             {
    //                 cout << task[i].action[j].param[k] << " || ";
    //             }
    //             cout << " " <<endl;
    //         }
    //     }
    // }
}

//读取路径
bool CENTRALCONTROLLING::readpath()
{
    XmlRpc::XmlRpcValue scanner_params;
    nh.getParam("/center/path", scanner_params);
    if (scanner_params.size() == 0)
    {
        ROS_INFO_STREAM("未找到goal");
        return false;
    }
    for(size_t i = 0; i<scanner_params.size(); ++i)
    {
        reinovo_control::path data;
        string name = (string)scanner_params[i]["name"];
        data.path_name = name;
        int number = (int)scanner_params[i]["number"];
        data.number = (uint8_t)number;

        for (size_t j = 0; j < scanner_params[i]["nav_name"].size(); j++)
        {
             string nav_name = (string)scanner_params[i]["nav_name"][j];
             data.nav_name.push_back(nav_name);
        }
        for (size_t k = 0; k < scanner_params[i]["task_name"].size(); k++)
        {
             string task_name = (string)scanner_params[i]["task_name"][k];
             data.task_name.push_back(task_name);
        }
        path.push_back(data);
    }
    // for (size_t i = 0; i < path.size(); i++)
    // {
    //     cout << path[i].path_name << endl ;
    //     cout << path[i].number << endl ;
    //     for (size_t j = 0; j < path[i].nav_name.size(); j++)
    //     {
    //         cout << path[i].nav_name[j] << "  ";
    //     }
    //     cout << " " << endl;
        
    //     for (size_t j = 0; j < path[i].task_name.size(); j++)
    //     {
    //         cout << path[i].task_name[j] << "  ";
    //     }
    //     cout << " " << endl;
    // }
    return true;    
}


bool CENTRALCONTROLLING::load_data(string path_name)
{
    int flag = 0;
    size_t pos;
    for (size_t i = 0; i < path.size(); i++)
    {
        if (path_name == path[i].path_name)
        {
            pos = i;
            flag = 1;
            break;
        }
    }
    if (flag == 1)
    {
        cur_path.path_name = path[pos].path_name;
        cur_path.number = path[pos].number;
        for (size_t i = 0; i < path[pos].nav_name.size(); i++)
        {
            NavGoal Navgoal;
            cur_path.navgoal.push_back(Navgoal);
            for (size_t j = 0; j < navgoal.size() ; j++)
            {
                if (path[pos].nav_name[i] == navgoal[j].name)
                {
                    cur_path.navgoal[i].navgoal = navgoal[j];
                    break;
                }
            }
            for (size_t j = 0; j < task.size(); j++)
            {
                if (path[pos].task_name[i] == task[j].name)
                {
                    cur_path.navgoal[i].task = task[j];
                    break;
                }
            }
        }

        //debug
        cout << " path name :" << cur_path.path_name<<endl;
        for (size_t i = 0; i < cur_path.navgoal.size(); i++)
        {
            cout << "---------" << endl;
            cout << "nav name  :" << cur_path.navgoal[i].navgoal.name <<endl;
            cout << "pose x  :" << cur_path.navgoal[i].navgoal.pose.x << endl;
            cout << "pose y  :" << cur_path.navgoal[i].navgoal.pose.y << endl;
            cout << "pose th  :" << cur_path.navgoal[i].navgoal.pose.theta << endl;

            cout << "task name  :" << cur_path.navgoal[i].task.name <<endl;
            ROS_INFO("STEP : %d  :",cur_path.navgoal[i].task.step);

            if (cur_path.navgoal[i].task.action.size()>0)
            {
                for (size_t j = 0; j < cur_path.navgoal[i].task.action.size();j++)
                {
                    cout << "    action name :" << cur_path.navgoal[i].task.action[j].name<<endl;
                    ROS_INFO("    number : %d",cur_path.navgoal[i].task.action[j].number);
                    for (size_t k = 0; k < cur_path.navgoal[i].task.action[j].param_name.size(); k++)
                    {
                        cout << "    paramname" << j << " : " << cur_path.navgoal[i].task.action[j].param_name[k] <<endl;
                        cout << "    param" << j << " : " << cur_path.navgoal[i].task.action[j].param[k] <<endl;
                    }
                }
            }
        }
    }else{
        ROS_INFO_STREAM("未找到路径");
    }
}




CENTRALCONTROLLING::CENTRALCONTROLLING():centerserver(nh,"center_server",boost::bind(&CENTRALCONTROLLING::execute,this,_1,&centerserver),false)
{
    string stra="";  
    nh.param<string>("/center/path_name",stra,"");
    readgoal();
    readtask();
    readpath();
    load_data(stra);

    centerserver.start();
    suspend=0;
    server_suspend=nh.advertiseService("center_suspend",&CENTRALCONTROLLING::suspend_callback,this);

    client_nav  =nh.serviceClient<oryxbot_msgs::nav_goal>("oryxbot_navgoali");//导航
    cilent_pick =nh.serviceClient<arm_controller::PickPlace>("pick_ar");//1.抓取放置	5
    client_charging =nh.serviceClient<oryxbot_msgs::SetCharge>("goto_charge");//2.自动回冲	1
    client_relativemove =nh.serviceClient<oryxbot_msgs::SetRelativeMove>("relative_move");//3.定向移动	2
    client_gotopos  =nh.serviceClient<arm_controller::move>("goto_position");//4.手臂goto	4
    client_track    =nh.serviceClient<oryxbot_msgs::track>("Track");//6.ar码追踪 2

}


//秒级延时函数
void CENTRALCONTROLLING::delay_s(float time)
{
    usleep((int)(time*1000.0*1000.0));
}
//程序暂停回调函数
bool CENTRALCONTROLLING::suspend_callback(std_srvs::SetBool::Request &req,
                      std_srvs::SetBool::Response &res)
{
    suspend=req.data;
    return true;
}

const string function_name[8] = {"sleep","pick_ar","goto_charge","relative_move","goto_position","set_zero","Track"};
int CENTRALCONTROLLING::action(string actionname,vector<float> parameter)
{
    int value= 0;
    int fun = -1;
    
    for (size_t i = 0; i < 8; i++)
    {
        if (actionname == function_name[i])
        {
            fun = i;
            break;
        }
    }
    if (fun == -1)
    {
        ROS_INFO(" Please check the error here. It should be fun parameter error.");
        return false;
    }else if (fun == 0){  //0.延时
        //延时 parameter[0] s;
        delay_s(parameter[0]);
	    value=true;
    }else if (fun == 1)
    {
        /* code for True */
        arm_controller::PickPlace   srv;
        srv.request.number=parameter[0];
        srv.request.mode=parameter[1];
        srv.request.pose.position.x=parameter[2];
        srv.request.pose.position.y=parameter[3];
        srv.request.pose.position.z=parameter[4];
        if(cilent_pick.call(srv))
        {
            value=true;
        }else{
            ROS_ERROR(" pick_ar service not found !");
            value=false;   
        }
    }else if (fun==2){      //2.自动回冲
        /* code for True */
        oryxbot_msgs::SetCharge srv;
        srv.request.charge=parameter[0];
        if(client_charging.call(srv))
        {
            value=srv.response.success;
        }else{
            ROS_ERROR(" charging service not found !");
            value=false;   
        }
    }else if (fun==3){      //3.定向移动
        /* code for True */
        oryxbot_msgs::SetRelativeMove srv;
        srv.request.mode=parameter[0];
        srv.request.relative_move=parameter[1];
         cout << "mode " << srv.request.mode << "move " <<srv.request.relative_move<<endl;
        if(client_relativemove.call(srv))
        {
            value=srv.response.success;
        }else{
            ROS_ERROR(" relativemove service not found !");
            value=false;   
        }
    }else if(fun==4){       //4.手臂goto
        arm_controller::move srv;
        srv.request.pose.position.x=parameter[0];
        srv.request.pose.position.y=parameter[1];
        srv.request.pose.position.z=parameter[2];
        if(client_gotopos.call(srv))
        {
            value=srv.response.success;
        }else{
            ROS_ERROR(" gotopos service not found !");
            value=false;   
        }
    }else if (fun==6){      //6.ar码追踪
        /* code for True */
        delay_s(2);
        oryxbot_msgs::track srv;
        srv.request.track=parameter[0];
        if(client_track.call(srv))
        {
            value=srv.response.success;
        }else{
            ROS_ERROR(" track ar service not found !");
            value=false;   
        }
    }
    
    return (value);
}

//中心调度函数
void CENTRALCONTROLLING::execute(const oryxbot_msgs::centerGoalConstPtr &goal, center_server *as)
{
    oryxbot_msgs::centerFeedback feedback;
    oryxbot_msgs::centerResult result_;
    for (size_t i = 0; i < cur_path.navgoal.size()&&ros::ok(); i++)
    {
        feedback.message="即将要去导航点 ：" + cur_path.navgoal[i].navgoal.name;
        as->publishFeedback(feedback);//导航成功返回
        oryxbot_msgs::nav_goal srv;
        srv.request.pose.x  =   cur_path.navgoal[i].navgoal.pose.x;
        srv.request.pose.y  =   cur_path.navgoal[i].navgoal.pose.y;
        srv.request.pose.theta  =   cur_path.navgoal[i].navgoal.pose.theta;
        if(client_nav.call(srv))
        {
            suspend=~srv.response.success;
        }else{
            ROS_ERROR(" nav service not found !");
            suspend=true;            
        }
        feedback.message=  cur_path.navgoal[i].navgoal.name + "  已到达！";
        as->publishFeedback(feedback);//导航成功返回

        if (cur_path.navgoal[i].task.name == "" )
        {
            feedback.message= "此处无task";
            as->publishFeedback(feedback);//导航成功返回
        }else{
            feedback.message= "task : " + cur_path.navgoal[i].task.name + "开始运行" ;
            as->publishFeedback(feedback);//开始运行人物

            if (cur_path.navgoal[i].task.action.size()>0)
            {
                for (size_t j = 0; j < cur_path.navgoal[i].task.action.size(); j++)
                {
                    feedback.message= "action : " + cur_path.navgoal[i].task.action[j].name + "开始运行" ;
                    as->publishFeedback(feedback);//开始执行action
                    
                    //暂停
                    if(suspend==true){
                        feedback.message= "暂停调度" ;
                        as->publishFeedback(feedback);//导航成功返回
                        cout << "zanting" <<endl;
                        while(suspend==true&&ros::ok())
                        {
                            sleep(1);
                        }
                        feedback.message= "开始调度" ;
                        as->publishFeedback(feedback);//导航成功返回                        
                    }
                    vector<float> parameter;
                    for (size_t k = 0; k < cur_path.navgoal[i].task.action[j].param.size(); k++)
                    {
                        parameter.push_back(cur_path.navgoal[i].task.action[j].param[k]);
                    }
                    
                    if (!action(cur_path.navgoal[i].task.action[j].name,parameter)){
                        ROS_ERROR(" is error !");
                        suspend=true;
                    }else{
                        suspend = false;
                    }
                }
            }
        }
        //在导航前暂停
        while(suspend==true&&ros::ok())
        {
	        cout << "zanting " << endl;
            sleep(1);
        }
    }

    feedback.message= "本次调度完毕" ;
    as->publishFeedback(feedback);
	cout << "end for ------" <<endl;
    as->setSucceeded(result_);
	cout << "end end end "<< endl;

}

//主函数
int main(int argc,char** argv)
{
    ros::init(argc,argv,"center");

    CENTRALCONTROLLING central;

    ROS_INFO(" init ok");
    //开启多线程接收
    ros::AsyncSpinner spinner(3); 
    spinner.start();
    ros::waitForShutdown();
}
