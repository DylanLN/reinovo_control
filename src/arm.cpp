#include "reinovo_control.h"
#include <yaml-cpp/yaml.h>

// struct type_arm{
//     string teachname;
//     arm_controller::control pos;
//     uint8_t ar;
//     bool pump;
// }


/*******************    手臂    **********************/

bool ReinovoControl::arm_init()
{
    //发布arm运行速度
    armvel_pub = nh_.advertise<geometry_msgs::Twist>("/arm_controller/cmd_vel", 10);    //发布速度信息
    //接收手臂坐标
    arm_sub = nh_.subscribe("arm_controller/position_info",100,&ReinovoControl::position_callback,this);
    //气泵控制客户端
    pump_client = nh_.serviceClient<std_srvs::SetBool>("pump");
    //电机解锁
    unlock_client = nh_.serviceClient<std_srvs::SetBool>("unlock");
    //goto
    armgoto_client = nh_.serviceClient<arm_controller::move>("goto_position");
    //goto ar
    argoto_client = nh_.serviceClient<arm_controller::PickPlace>("goto_ar");

    //笛卡尔运动速度
    arm_cmd = 3;
    flag_arm = 0;
    flag_cam = 0;
    flag_teach = 0;
    //手臂
    //开启手臂
    connect(ui->open_arm, SIGNAL(clicked()), this, SLOT(fopen_arm()));
    //气泵
    connect(ui->pump, SIGNAL(clicked()), this, SLOT(fpump()));
    //解锁电机
    connect(ui->unlock_arm, SIGNAL(clicked()), this, SLOT(funlock_arm()));
    //开启相机
    connect(ui->open_cam, SIGNAL(clicked()), this, SLOT(fopen_cam()));
    //保存目标点
    connect(ui->armsave_target, SIGNAL(clicked()), this, SLOT(farmsave_target()));
    //删除目标点 -yaml-cpp 无法删除节点,暂时停止使用
    // connect(ui->armdelete_target, SIGNAL(clicked()), this, SLOT(farmdelete_target()));
    //目标点选中框
    connect(ui->armtarget, SIGNAL(clicked()), this, SLOT(farmtarget()));
    //ar选中框
    connect(ui->ar, SIGNAL(clicked()), this, SLOT(far()));
    //goto
    connect(ui->arm_goto, SIGNAL(clicked()), this, SLOT(farm_goto()));
    //进入示教
    connect(ui->enther_teach, SIGNAL(clicked()), this, SLOT(fenther_teach()));

    //保存示教
    connect(ui->save_armteach, SIGNAL(clicked()), this, SLOT(fsave_armteach()));




    connect(ui->micro, SIGNAL(clicked()), this, SLOT(fmicro()));   //微调
    connect(ui->plusx, SIGNAL(clicked()), this, SLOT(fplusx()));         //发布正vx
    connect(ui->plusy, SIGNAL(clicked()), this, SLOT(fplusy()));         //发布正vx
    connect(ui->plusz, SIGNAL(clicked()), this, SLOT(fplusz()));         //发布正vx
    connect(ui->redx, SIGNAL(clicked()), this, SLOT(fredx()));         //发布正vx
    connect(ui->redy, SIGNAL(clicked()), this, SLOT(fredy()));         //发布正vx
    connect(ui->redz, SIGNAL(clicked()), this, SLOT(fredz()));         //发布正vx
    ui->plusx->setAutoRepeat(true);       //允许 自动重复
    ui->plusx->setAutoRepeatDelay(300);       //设置重复操作的时延
    ui->plusx->setAutoRepeatInterval(300);       //设置自动操作的间隔
    ui->plusy->setAutoRepeat(true);       //允许 自动重复
    ui->plusy->setAutoRepeatDelay(300);       //设置重复操作的时延
    ui->plusy->setAutoRepeatInterval(300);       //设置自动操作的间隔
    ui->plusz->setAutoRepeat(true);       //允许 自动重复
    ui->plusz->setAutoRepeatDelay(300);       //设置重复操作的时延
    ui->plusz->setAutoRepeatInterval(300);       //设置自动操作的间隔
    ui->redx->setAutoRepeat(true);       //允许 自动重复
    ui->redx->setAutoRepeatDelay(300);       //设置重复操作的时延
    ui->redx->setAutoRepeatInterval(300);       //设置自动操作的间隔
    ui->redy->setAutoRepeat(true);       //允许 自动重复
    ui->redy->setAutoRepeatDelay(300);       //设置重复操作的时延
    ui->redy->setAutoRepeatInterval(300);       //设置自动操作的间隔
    ui->redz->setAutoRepeat(true);       //允许 自动重复
    ui->redz->setAutoRepeatDelay(300);       //设置重复操作的时延
    ui->redz->setAutoRepeatInterval(300);       //设置自动操作的间隔

    return true;
}
//刷新页面
bool ReinovoControl::arm_refresh()
{
    ui->armtarget_list->clear();
    arm_goal.clear();

    YAML::Node config = arm_yaml.load_file(map_file + "/param/armgoal.yaml");

    int i=0;
    for (YAML::const_iterator it=config["armgoal"].begin();
        it!=config["armgoal"].end();
        it++)
    {
        ARMGOAL ag;
        ag.goalname = config["armgoal"][i]["name"].as<string>();
        ag.pose.position.x = config["armgoal"][i]["pose.x"].as<float>();
        ag.pose.position.y = config["armgoal"][i]["pose.y"].as<float>();
        ag.pose.position.z = config["armgoal"][i]["pose.z"].as<float>();
        arm_goal.push_back(ag);
        ui->armtarget_list->addItem(QString::fromStdString(ag.goalname));
        i++;
    }

}


//open_arm
void ReinovoControl::fopen_arm()
{
    if(flag_arm == 0){

        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "arm";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_arm->setText("关闭手臂");
                flag_arm=1;
            }else{
                ui->open_arm->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_arm == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "arm";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_arm->setText("开启手臂");
                flag_arm=0;
            }else{
                ui->open_arm->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}

//open_cam
void ReinovoControl::fopen_cam()
{
    if(flag_cam == 0){

        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "cam";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_cam->setText("关闭相机");
                flag_cam=1;
            }else{
                ui->open_cam->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_cam == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "cam";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_cam->setText("开启相机");
                flag_cam=0;
            }else{
                ui->open_cam->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}


//气泵
void ReinovoControl::fpump()
{
    std_srvs::SetBool srv;
    srv.request.data = ui->pump->isChecked();
    if(pump_client.call(srv)){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"切换完毕");
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"失败,未连接到机械臂");
    }
}

//电机解锁
void ReinovoControl::funlock_arm()
{
    std_srvs::SetBool srv;
    srv.request.data = ui->unlock_arm->isChecked();
    if(unlock_client.call(srv)){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"电机锁");
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"失败,未连接到机械臂");
    }
}

//显示机械臂
void ReinovoControl::position_callback(const arm_controller::control& msg)
{
    ui->value_x->setText(QString("%1").arg(msg.position.x));
    ui->value_y->setText(QString("%1").arg(msg.position.y));
    ui->value_z->setText(QString("%1").arg(msg.position.z));
    arm_pos = msg;
}

//保存手臂当前位置
void ReinovoControl::farmsave_target()
{
    string text = ui->armtarget_name->text().toStdString();
    if(text == ""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"请输入保存的目标点名称");
    }else{
        //检查是否与现有点重名
        int flag = 0;
        for (size_t i = 0; i < arm_goal.size(); i++)
        {
            if(text == arm_goal[i].goalname){
                flag = 1;
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"与已有目标点重名");
                break;
            }
        }
        if (!flag)
        {
            ARMGOAL armgoal;
            armgoal.pose = arm_pos;
            armgoal.goalname = text;
            //保存数据
            arm_goal.push_back(armgoal);
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已保存");
            ui->armtarget_list->addItem(ui->armtarget_name->text());

            YAML::Node config;
            config["name"] = text;
            config["pose.x"] = armgoal.pose.position.x;
            config["pose.y"] = armgoal.pose.position.y;
            config["pose.z"] = armgoal.pose.position.z;
            arm_yaml.config["armgoal"].push_back(config);
            arm_yaml.file_write();
        }
    }
}

//删除目标点
void ReinovoControl::farmdelete_target()
{
    string text = ui->armtarget_list->currentText().toStdString();
    if (arm_goal.size() == 0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "已经不存在导航点了");
    }else{
        int curIndex = ui->armtarget_list->currentIndex();
        cout << "current index :" << curIndex << endl;
        if (arm_goal.size() == 1)
        {
            ui->armtarget_list->clear();
        }else{
            ui->armtarget_list->removeItem(curIndex);
        }
        arm_goal.erase(arm_goal.begin() + curIndex);

        arm_yaml.config["armgoal"].remove(curIndex);

        cout << arm_yaml.config["armgoal"].size() << "  arm goal . size" << endl;
        arm_yaml.file_write();

        ui->total_output->appendPlainText(QString::fromStdString(get_time()) + "已删除");
    }
}

//目标点勾选
//与ar互斥
void ReinovoControl::farmtarget()
{
    if (ui->armtarget->isChecked())
    {
        ui->ar->blockSignals(true);
        ui->ar->setCheckState(Qt::CheckState::Unchecked);
        ui->ar->blockSignals(false);
    }
}

//ar勾选
void ReinovoControl::far()
{
    if (ui->ar->isChecked())
    {
        ui->armtarget->blockSignals(true);
        ui->armtarget->setCheckState(Qt::CheckState::Unchecked);
        ui->armtarget->blockSignals(false);
    }
}

//goto
void ReinovoControl::farm_goto()
{
    if (ui->armtarget->isChecked())
    {
        ui_state = 2;
    }else if (ui->ar->isChecked()){
        ui_state = 3;
    }
}

//open_teach
void ReinovoControl::fenther_teach()
{
    if(flag_teach == 0){
        //气泵
        Connection_pump = connect(ui->pump, SIGNAL(clicked()), this, SLOT(ftpump()));
        //goto
        Connection_goto = connect(ui->arm_goto, SIGNAL(clicked()), this, SLOT(ftarm_goto()));

        //任务清空
        task_arm.action.clear();
        ui->enther_teach->setText("退出示教");
        flag_teach=1;
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"进入示教模式");
    }else if(flag_teach == 1){
        ui->pump->disconnect(Connection_goto);
        ui->arm_goto->disconnect(Connection_pump);

        ui->enther_teach->setText("进入示教");
        flag_teach=0;
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"退出示教模式");
    }
}

//示教气泵
void ReinovoControl::ftpump()
{
    reinovo_control::action act;
    act.name = "pump";
    act.number = 1;
    act.param_name.push_back("bool");
    act.param.push_back(ui->pump->isChecked());

    task_arm.action.push_back(act);


    QListWidgetItem *aItem=new QListWidgetItem("new item");
    aItem->setText("pump : " + QString::number(ui->pump->isChecked()));
    aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    ui->armteach_list->addItem(aItem);
}

//goto示教
void ReinovoControl::ftarm_goto()
{
    //已保存目标点
    if (ui->armtarget->isChecked())
    {
        reinovo_control::action act;
        act.name = "armgoto";
        act.number = 3;
        act.param_name.push_back("x");
        act.param_name.push_back("y");
        act.param_name.push_back("z");

        act.param.push_back(arm_goal[ui->armtarget_list->currentIndex()].pose.position.x);
        act.param.push_back(arm_goal[ui->armtarget_list->currentIndex()].pose.position.y);
        act.param.push_back(arm_goal[ui->armtarget_list->currentIndex()].pose.position.z);

        task_arm.action.push_back(act);

        QListWidgetItem *aItem=new QListWidgetItem("new item");
        aItem->setText("goto:" + QString::number(arm_goal[ui->armtarget_list->currentIndex()].pose.position.x)+","
                               + QString::number(arm_goal[ui->armtarget_list->currentIndex()].pose.position.y)+","
                               + QString::number(arm_goal[ui->armtarget_list->currentIndex()].pose.position.z)+",");
        aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        ui->armteach_list->addItem(aItem);

    }else if (ui->ar->isChecked()){//ar码
        reinovo_control::action act;
        act.name = "argoto";
        act.number = 1;
        act.param_name.push_back("number");
        act.param.push_back(ui->ar_value->value());

        task_arm.action.push_back(act);

        QListWidgetItem *aItem=new QListWidgetItem("new item");
        aItem->setText("ar goto:" + QString::number(ui->ar_value->value()));
        aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        ui->armteach_list->addItem(aItem);
    }else{
        reinovo_control::action act;
        act.name = "armgoto";
        act.number = 3;
        act.param_name.push_back("x");
        act.param_name.push_back("y");
        act.param_name.push_back("z");

        act.param.push_back(arm_pos.position.x);
        act.param.push_back(arm_pos.position.y);
        act.param.push_back(arm_pos.position.z);
        task_arm.action.push_back(act);

        QListWidgetItem *aItem=new QListWidgetItem("new item");
        aItem->setText("goto:" + QString::number(arm_pos.position.x)+","
                               + QString::number(arm_pos.position.y)+","
                               + QString::number(arm_pos.position.z)+",");
        aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        ui->armteach_list->addItem(aItem);
    }
}

//保存示教
void ReinovoControl::fsave_armteach()
{
    string text = ui->armteach_name->text().toStdString();
    if(text == ""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"请输入保存的示教程序名称");
    }else{
        //检查是否与现有程序重名
        int flag = 0;
        for (size_t i = 0; i < v_taskarm.size(); i++)
        {
            if(text == v_taskarm[i].name){
                flag = 1;
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"与已有程序重名");
                break;
            }
        }
        if (!flag)
        {
            task_arm.name = text;
            //保存数据
            v_taskarm.push_back(task_arm);
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已保存");
            ui->armteach_list_2->addItem(ui->armteach_name->text());

        }
    }
}


//微调
void ReinovoControl::fmicro()
{
    //CheckBox状态获取
    if (ui->micro->isChecked()){
        arm_cmd = 1;
    }else{
        arm_cmd = 3;
    }
}
//+x
void ReinovoControl::fplusx()
{
        geometry_msgs::Twist msg;
        msg.linear.x= arm_cmd;
        armvel_pub.publish(msg);
}
//+y
void ReinovoControl::fplusy()
{
        geometry_msgs::Twist msg;
        msg.linear.y= arm_cmd;
        armvel_pub.publish(msg);
}
//+z
void ReinovoControl::fplusz()
{
        geometry_msgs::Twist msg;
        msg.linear.z= arm_cmd;
        armvel_pub.publish(msg);
}
//-x
void ReinovoControl::fredx()
{
        geometry_msgs::Twist msg;
        msg.linear.x= -arm_cmd;
        armvel_pub.publish(msg);
}
//-y
void ReinovoControl::fredy()
{
        geometry_msgs::Twist msg;
        msg.linear.y= -arm_cmd;
        armvel_pub.publish(msg);
}
//-z
void ReinovoControl::fredz()
{
        geometry_msgs::Twist msg;
        msg.linear.z= -arm_cmd;
        armvel_pub.publish(msg);
}