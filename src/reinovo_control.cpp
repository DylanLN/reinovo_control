#include "reinovo_control.h"

//获取系统时间
string ReinovoControl::get_time()
{
    //获取系统时间  
    time_t now_time=time(NULL);  
    //获取本地时间  
    tm*  t_tm = localtime(&now_time);  
    //转换为年月日星期时分秒结果 
    string now = (string)asctime(t_tm);
    now[now.size()-1] = ':';
    return now;
}

//ui 的回调函数
//主要是机器人导航所用
void ReinovoControl::ui_thread()
{
    ros::Rate loop(5);
    while(ros::ok()){
        //机器人goto
        if (ui_state == 1)
        {
            reinovo_control::goto_navgoal srv;
            srv.request.nav_goal.name = ui->target_list->currentText().toStdString();
            for (size_t i = 0; i < v_navgoal.size(); i++)
            {
                if(srv.request.nav_goal.name == v_navgoal[i].name){
                    srv.request.nav_goal.pose.x = v_navgoal[i].pose.x;
                    srv.request.nav_goal.pose.y = v_navgoal[i].pose.y;
                    srv.request.nav_goal.pose.theta = v_navgoal[i].pose.theta;
                    if(goto_pose.call(srv)){
                        if(srv.response.success == true){
                            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已到达");   
                        }else{
                            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"导航失败");   
                        }
                    }else{
                        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到底层节点！");   
                    }
                    break;
                }
            }
            ui_state=0;
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"goto 已到达");   
        }
        //机械臂goto
        else if (ui_state == 2)
        {
            if (ui->armtarget->isChecked()){
                arm_controller::move srv;

                srv.request.pose = arm_goal[ui->armtarget_list->currentIndex()].pose;
                if (armgoto_client.call(srv))
                {
                    if (srv.response.success)
                    {
                        ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "已到达");
                    }else{
                        ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "失败");
                    }
                }else{
                    ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "未连接到机械臂");
                }
            }
            ui_state=0;
        }
        //追踪ar
        else if (ui_state == 3)
        {
            arm_controller::PickPlace srv;
            srv.request.number = ui->ar_value->value();
            if (argoto_client.call(srv))
            {
                if (srv.response.success)
                {
                    ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "已到达");
                }else{
                    ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "失败");
                }
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "未连接到机械臂");
            }
            ui_state=0;
        }
        loop.sleep();
    }
}

void ReinovoControl::tabwidget(int index)
{
    if (index == 0)
    {
        /* code for True */
    }else if (index == 3)
    {
        // arm_refresh();
    }
}



//构造函数
ReinovoControl::ReinovoControl(QWidget* parent):rviz::Panel(parent),ui(new Ui::Form),timer_(new QTimer),client("center_server",true)
{
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
        ros::console::notifyLoggerLevelsChanged();
    }
    ui->setupUi(this);
    ROS_INFO("ReinovoControl");
    pub = nh_.advertise<std_msgs::String>("ui", 10);

    //tab
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabwidget(int)));

//槽函数初始化
    //首页
    home_init();

    //Teleop
    teleop_init();

    //导航
    nav_init();
    //示教
    teach_init();

    //arm
    // arm_init();

    //组合
    combin_init();

    //调度
    dispatch_init();


//数据初始化

    ui_state=0;

    key_client = nh_.serviceClient<reinovo_control::ask>("key_server");

    uithread = boost::thread(boost::bind(&ReinovoControl::ui_thread, this));

    //*************其他
    n.param<std::string>("/json_file/map_file", map_file, ".");

    // ofile.file_open(str+"/doc/reinovo_control.log");
    //开启多线程接收
    ros::AsyncSpinner spinner(3); 
    spinner.start();
}


//析构函数
ReinovoControl::~ReinovoControl()
{
    actionthread.interrupt();
    uithread.interrupt();
    uithread.join();
    delete timer_;
    delete ui;
    ROS_DEBUG_STREAM("reinovo control已关闭");
}

#include "pluginlib/class_list_macros.h"
PLUGINLIB_EXPORT_CLASS(ReinovoControl, rviz::Panel)
