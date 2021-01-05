#include "reinovo_control.h"

/*******************    Teleop    **********************/
bool ReinovoControl::teleop_init()
{
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

    //Teleop
    flag_speed=0;
    
    return true;
}


//速度使能
void ReinovoControl::fspeed_enable()
{
    //CheckBox状态获取
    if (ui->speed_enable->isChecked()){
        string text=ui->teleop_topic->text().toStdString();
        if (text=="")
        {
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：请设置速度话题名称！");
        }else{
            ui->vx_plus->setAutoRepeat(true);       //允许 自动重复
            ui->vx_plus->setAutoRepeatDelay(50);       //设置重复操作的时延
            ui->vx_plus->setAutoRepeatInterval(50);       //设置自动操作的间隔
            ui->vx_minus->setAutoRepeat(true);       //允许 自动重复
            ui->vx_minus->setAutoRepeatDelay(50);       //设置重复操作的时延
            ui->vx_minus->setAutoRepeatInterval(50);       //设置自动操作的间隔
            ui->vy_plus->setAutoRepeat(true);       //允许 自动重复
            ui->vy_plus->setAutoRepeatDelay(50);       //设置重复操作的时延
            ui->vy_plus->setAutoRepeatInterval(50);       //设置自动操作的间隔
            ui->vy_minus->setAutoRepeat(true);       //允许 自动重复
            ui->vy_minus->setAutoRepeatDelay(50);       //设置重复操作的时延
            ui->vy_minus->setAutoRepeatInterval(50);       //设置自动操作的间隔
            ui->vth_plus->setAutoRepeat(true);       //允许 自动重复
            ui->vth_plus->setAutoRepeatDelay(50);       //设置重复操作的时延
            ui->vth_plus->setAutoRepeatInterval(50);       //设置自动操作的间隔
            ui->vth_minus->setAutoRepeat(true);       //允许 自动重复
            ui->vth_minus->setAutoRepeatDelay(50);       //设置重复操作的时延
            ui->vth_minus->setAutoRepeatInterval(50);       //设置自动操作的间隔
            vel.linear.x=ui->vx_cmd->value();       //获取DoubleSpinBox数值
            vel.linear.y=ui->vy_cmd->value();
            vel.angular.z=ui->vth_cmd->value();
            vel_pub = nh_.advertise<geometry_msgs::Twist>(text, 1 );    //发布速度信息
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"速度使能");
            flag_speed=1;
        }
    }else{
        vel_pub.shutdown();
        flag_speed=0;
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"速度失能");
    }
}

//发布+vx指令
void ReinovoControl::fpub_vxplus()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        msg.linear.x= vel.linear.x;
        vel_pub.publish(msg);
    }
}
//发布-vx指令
void ReinovoControl::fpub_vxminus()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        msg.linear.x= -vel.linear.x;
        vel_pub.publish(msg);
    }
}

//发布vy指令
void ReinovoControl::fpub_vyplus()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        msg.linear.y= vel.linear.y;
        vel_pub.publish(msg);
    }
}

//发布-vy指令
void ReinovoControl::fpub_vyminus()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        msg.linear.y= -vel.linear.y;
        vel_pub.publish(msg);
    }
}
//发布vth指令
void ReinovoControl::fpub_vthplus()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        msg.angular.z= vel.angular.z;
        vel_pub.publish(msg);
    }
}
//发布-vth指令
void ReinovoControl::fpub_vthminus()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        msg.angular.z= -vel.angular.z;
        vel_pub.publish(msg);
    }
}

//发布-vth指令
void ReinovoControl::fvel_stop()
{
    if(flag_speed == 1){
        geometry_msgs::Twist msg;
        vel_pub.publish(msg);
    }
}
