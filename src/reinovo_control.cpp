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

/*******************    ros    **********************/
/*******************    ros    **********************/

//构造函数
ReinovoControl::ReinovoControl(QWidget* parent):rviz::Panel(parent),ui(new Ui::Form),timer_(new QTimer),client("center_server",true)
{
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
        ros::console::notifyLoggerLevelsChanged();
    }
    ui->setupUi(this);
    ROS_INFO("ReinovoControl");
    pub = nh_.advertise<std_msgs::String>("ui", 10);

//槽函数初始化
    //首页
    connect(ui->open_driver, SIGNAL(clicked()), this, SLOT(fopen_driver()));    //打开、关闭驱动
    connect(ui->open_slam, SIGNAL(clicked()), this, SLOT(fopen_slam()));    //开始、关闭建图
    connect(ui->save_map, SIGNAL(clicked()), this, SLOT(fsave_map()));      //保存地图
    connect(ui->open_all, SIGNAL(clicked()), this, SLOT(fopen_all()));      //open all
    connect(ui->other1, SIGNAL(clicked()), this, SLOT(fother1()));      //other1
    connect(ui->other2, SIGNAL(clicked()), this, SLOT(fother2()));      //other1
    connect(ui->other3, SIGNAL(clicked()), this, SLOT(fother3()));      //other1

    n.param<string>("other1", strother[0], "其他模块1");
    n.param<string>("other2", strother[1], "其他模块2");
    n.param<string>("other3", strother[2], "其他模块3");
    ui->other1->setText(QString::fromStdString(strother[0]));
    ui->other2->setText(QString::fromStdString(strother[1]));
    ui->other3->setText(QString::fromStdString(strother[2]));


    //Teleop
    connect(ui->speed_enable, SIGNAL(clicked()), this, SLOT(fspeed_enable()));   //速度使能
    ui->vx_cmd->setSuffix("m/s");       //设置后缀
    ui->vy_cmd->setSuffix("m/s");       //设置后缀
    ui->vth_cmd->setSuffix("rad/s");       //设置后缀

    connect(ui->vx_plus, SIGNAL(clicked()), this, SLOT(fpub_vxplus()));         //发布正vx
    connect(ui->vx_minus, SIGNAL(clicked()), this, SLOT(fpub_vxminus()));         //发布正vx
    connect(ui->vy_plus, SIGNAL(clicked()), this, SLOT(fpub_vyplus()));         //发布正vx
    connect(ui->vy_minus, SIGNAL(clicked()), this, SLOT(fpub_vyminus()));         //发布正vx
    connect(ui->vth_plus, SIGNAL(clicked()), this, SLOT(fpub_vthplus()));         //发布正vx
    connect(ui->vth_minus, SIGNAL(clicked()), this, SLOT(fpub_vthminus()));         //发布正vx
    connect(ui->vel_stop, SIGNAL(clicked()), this, SLOT(fvel_stop()));         //发布正vx



    //导航
    connect(ui->refresh_map, SIGNAL(clicked()), this, SLOT(frefresh_map()));         //发布正vx
    connect(ui->switch_map, SIGNAL(clicked()), this, SLOT(fswitch_map()));         //发布正vx
    connect(ui->delete_map, SIGNAL(clicked()), this, SLOT(fdelete_map()));         //发布正vx
    connect(ui->open_nav, SIGNAL(clicked()), this, SLOT(fopen_nav()));         //发布正vx
    connect(ui->refresh_target, SIGNAL(clicked()), this, SLOT(frefresh_target()));         //发布正vx
    connect(ui->goto_target, SIGNAL(clicked()), this, SLOT(fgoto_target()));         //发布正vx
    connect(ui->delete_target, SIGNAL(clicked()), this, SLOT(fdelete_target()));         //发布正vx
    connect(ui->get_gesture, SIGNAL(clicked()), this, SLOT(fget_gesture()));         //发布正vx
    connect(ui->save_target, SIGNAL(clicked()), this, SLOT(fsave_target()));         //发布正vx

    //手臂
    //开启手臂
    connect(ui->open_arm, SIGNAL(clicked()), this, SLOT(fopen_arm()));
    connect(ui->pump, SIGNAL(clicked()), this, SLOT(fpump()));
    
    arm_cmd = 3;
    flag_arm = 0;
    connect(ui->micro, SIGNAL(clicked()), this, SLOT(fmicro()));   //微调
    connect(ui->plusx, SIGNAL(clicked()), this, SLOT(fplusx()));         //发布正vx
    connect(ui->plusy, SIGNAL(clicked()), this, SLOT(fplusy()));         //发布正vx
    connect(ui->plusz, SIGNAL(clicked()), this, SLOT(fplusz()));         //发布正vx
    connect(ui->redx, SIGNAL(clicked()), this, SLOT(fredx()));         //发布正vx
    connect(ui->redy, SIGNAL(clicked()), this, SLOT(fredy()));         //发布正vx
    connect(ui->redz, SIGNAL(clicked()), this, SLOT(fredz()));         //发布正vx
    ui->plusx->setAutoRepeat(true);       //允许 自动重复
    ui->plusx->setAutoRepeatDelay(100);       //设置重复操作的时延
    ui->plusx->setAutoRepeatInterval(100);       //设置自动操作的间隔
    ui->plusy->setAutoRepeat(true);       //允许 自动重复
    ui->plusy->setAutoRepeatDelay(100);       //设置重复操作的时延
    ui->plusy->setAutoRepeatInterval(100);       //设置自动操作的间隔
    ui->plusz->setAutoRepeat(true);       //允许 自动重复
    ui->plusz->setAutoRepeatDelay(100);       //设置重复操作的时延
    ui->plusz->setAutoRepeatInterval(100);       //设置自动操作的间隔
    ui->redx->setAutoRepeat(true);       //允许 自动重复
    ui->redx->setAutoRepeatDelay(100);       //设置重复操作的时延
    ui->redx->setAutoRepeatInterval(100);       //设置自动操作的间隔
    ui->redy->setAutoRepeat(true);       //允许 自动重复
    ui->redy->setAutoRepeatDelay(100);       //设置重复操作的时延
    ui->redy->setAutoRepeatInterval(100);       //设置自动操作的间隔
    ui->redz->setAutoRepeat(true);       //允许 自动重复
    ui->redz->setAutoRepeatDelay(100);       //设置重复操作的时延
    ui->redz->setAutoRepeatInterval(100);       //设置自动操作的间隔



    //示教
    connect(ui->refresh_teach, SIGNAL(clicked()), this, SLOT(frefresh_teach()));         //发布正vx
    connect(ui->teach_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(fteach_list(QListWidgetItem*)));         //发布正vx
    connect(ui->teach_rea, SIGNAL(clicked()), this, SLOT(fteach_rea()));         //发布正vx
    connect(ui->delete_teach, SIGNAL(clicked()), this, SLOT(fdelete_teach()));         //发布正vx
    connect(ui->create_teach, SIGNAL(clicked()), this, SLOT(fcreate_teach()));         //发布正vx
    connect(ui->add_action, SIGNAL(clicked()), this, SLOT(fadd_action()));         //发布正vx
    connect(ui->delete_action, SIGNAL(clicked()), this, SLOT(fdelete_action()));         //发布正vx
    connect(ui->teach_info, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(fteach_info(QListWidgetItem*)));         //发布正vx
    connect(ui->param_info, SIGNAL(cellChanged(int,int)), this, SLOT(fparam_info(int,int)));         //发布正vx
    connect(ui->makefile2, SIGNAL(clicked()), this, SLOT(fmakefile2()));         //发布正vx


    //组合
    //qt
    connect(ui->refresh_path, SIGNAL(clicked()), this, SLOT(frefresh_path()));         //发布正vx
    connect(ui->path_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(fpath_list(QString)));         //发布正vx
    connect(ui->delete_path, SIGNAL(clicked()), this, SLOT(fdelete_path()));         //发布正vx
    connect(ui->create_path, SIGNAL(clicked()), this, SLOT(fcreate_path()));         //发布正vx
    //connect(ui->path_info, SIGNAL(cellChanged(int,int)), this, SLOT(fpath_info(int,int)));         //发布正vx
    connect(ui->delete_target1, SIGNAL(clicked()), this, SLOT(fdelete_target1()));         //发布正vx
    connect(ui->add_target, SIGNAL(clicked()), this, SLOT(fadd_target()));         //发布正vx
    connect(ui->mount, SIGNAL(clicked()), this, SLOT(fmount()));         //发布正vx
    connect(ui->makefile, SIGNAL(clicked()), this, SLOT(fmakefile()));         //发布正vx
    //ros
    get_path = nh_.serviceClient<reinovo_control::pathsrv>("get_path");
    pub_path = nh_.serviceClient<reinovo_control::pathserver>("pub_path");


    //调度
    connect(ui->open_dispatch, SIGNAL(clicked()), this, SLOT(fopen_dispatch()));         //发布正vx
    connect(ui->start_dispatch, SIGNAL(clicked()), this, SLOT(fstart_dispatch()));         //发布正vx
    connect(ui->pause_dispatch, SIGNAL(clicked()), this, SLOT(fpause_dispatch()));         //发布正vx
    connect(ui->recover_dispatch, SIGNAL(clicked()), this, SLOT(frecover_dispatch()));         //发布正vx
    connect(ui->refresh_path1, SIGNAL(clicked()), this, SLOT(frefresh_path1()));         //发布正vx
    connect(ui->load_path1, SIGNAL(clicked()), this, SLOT(fload_path1()));         //发布正vx
    connect(ui->auto_charging, SIGNAL(clicked()), this, SLOT(fauto_charging()));         //发布正vx


//数据初始化
    //首页
    flag_driver=0;
    flag_slam=0;
    flag_openall=0;
    flag_other[0]=0;
    flag_other[1]=0;
    flag_other[2]=0;
    //Teleop
    flag_speed=0;
    //导航
    flag_nav=0;
    //调度
    flag_dispatch=0;
    flag_charging=0;
    dispatch_status = 0;

    ui_state=0;

    //保存map
    save_map = nh_.serviceClient<reinovo_control::ask>("save_map");

    key_client = nh_.serviceClient<reinovo_control::ask>("key_server");
    get_pose = nh_.serviceClient<reinovo_control::get_navgoal>("get_pose");

    get_navgoal = nh_.serviceClient<reinovo_control::navgoalsrv>("get_goal");
    save_navgoal = nh_.serviceClient<reinovo_control::navgoalserver>("save_navgoal");

    get_map = nh_.serviceClient<reinovo_control::navgoalsrv>("get_map");
    delete_map = nh_.serviceClient<reinovo_control::ask>("delete_map");
    goto_pose =  nh_.serviceClient<reinovo_control::goto_navgoal>("goto_pose");

    switch_map = nh_.serviceClient<reinovo_control::ask>("switch_map");
    //获取action模板
    get_actiontem = nh_.serviceClient<reinovo_control::actionsrv>("get_template");
    //获取任务task
    get_task = nh_.serviceClient<reinovo_control::tasksrv>("get_task");
    //保存task文件
    save_task = nh_.serviceClient<reinovo_control::taskserver>("save_task");
    //打开调度
    open_dispatch = nh_.serviceClient<reinovo_control::ask>("open_dispatch");

    //发布arm运行速度
    armvel_pub = nh_.advertise<geometry_msgs::Twist>("/arm_controller/cmd_vel", 10);    //发布速度信息
    //接收手臂坐标
    arm_sub = nh_.subscribe("arm_controller/position_info",100,&ReinovoControl::position_callback,this);
    //气泵控制客户端
    pump_client = nh_.serviceClient<std_srvs::SetBool>("pump");
    //暂停调度
    suspend_client =  nh_.serviceClient<std_srvs::SetBool>("center_suspend");

    uithread = boost::thread(boost::bind(&ReinovoControl::ui_thread, this));
    ros::AsyncSpinner spinner(3); 
    spinner.start();
}


//析构函数
ReinovoControl::~ReinovoControl()
{
    actionthread.interrupt();
    uithread.interrupt();
    delete timer_;
    delete ui;
    ROS_DEBUG_STREAM("reinovo control已关闭");
}

#include "pluginlib/class_list_macros.h"
PLUGINLIB_EXPORT_CLASS(ReinovoControl, rviz::Panel)
