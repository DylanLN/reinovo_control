#include "reinovo_control.h"

/*******************    手臂    **********************/
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
                ui->open_arm->setText(QString::fromStdString(strother[0]));
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
//显示机械臂
void ReinovoControl::position_callback(const arm_controller::control& msg)
{
    ui->value_x->setText(QString("%1").arg(msg.position.x));
    ui->value_y->setText(QString("%1").arg(msg.position.y));
    ui->value_z->setText(QString("%1").arg(msg.position.z));
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

void ReinovoControl::fplusx()
{
        geometry_msgs::Twist msg;
        msg.linear.x= arm_cmd;
        armvel_pub.publish(msg);
}
void ReinovoControl::fplusy()
{
        geometry_msgs::Twist msg;
        msg.linear.y= arm_cmd;
        armvel_pub.publish(msg);
}
void ReinovoControl::fplusz()
{
        geometry_msgs::Twist msg;
        msg.linear.z= arm_cmd;
        armvel_pub.publish(msg);
}
void ReinovoControl::fredx()
{
        geometry_msgs::Twist msg;
        msg.linear.x= -arm_cmd;
        armvel_pub.publish(msg);
}
void ReinovoControl::fredy()
{
        geometry_msgs::Twist msg;
        msg.linear.y= -arm_cmd;
        armvel_pub.publish(msg);
}
void ReinovoControl::fredz()
{
        geometry_msgs::Twist msg;
        msg.linear.z= -arm_cmd;
        armvel_pub.publish(msg);
}
