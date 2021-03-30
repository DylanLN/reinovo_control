#include "reinovo_control.h"

/*******************    调度    **********************/
bool ReinovoControl::dispatch_init()
{
    connect(ui->open_dispatch, SIGNAL(clicked()), this, SLOT(fopen_dispatch()));         //发布正vx
    connect(ui->start_dispatch, SIGNAL(clicked()), this, SLOT(fstart_dispatch()));         //发布正vx
    connect(ui->pause_dispatch, SIGNAL(clicked()), this, SLOT(fpause_dispatch()));         //发布正vx
    connect(ui->recover_dispatch, SIGNAL(clicked()), this, SLOT(frecover_dispatch()));         //发布正vx
    connect(ui->refresh_path1, SIGNAL(clicked()), this, SLOT(frefresh_path1()));         //发布正vx
    connect(ui->load_path1, SIGNAL(clicked()), this, SLOT(fload_path1()));         //发布正vx
    connect(ui->auto_charging, SIGNAL(clicked()), this, SLOT(fauto_charging()));         //发布正vx

    connect(this,SIGNAL(AppendText(QString)),this,SLOT(fSlotAppendText(QString)));

    flag_dispatch=0;
    flag_charging=0;
    dispatch_status = 0;

    //打开调度
    open_dispatch = nh_.serviceClient<reinovo_control::ask>("open_dispatch");

    //暂停调度
    suspend_client =  nh_.serviceClient<std_srvs::SetBool>("center_suspend");

    return true;
}

//自定义槽函数,多线程信息输出
void ReinovoControl::fSlotAppendText(const QString &text)
{
    ui->dispatch_output->appendPlainText(text);
}

//开启、关闭调度
void ReinovoControl::fopen_dispatch()
{
    if(flag_dispatch == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = ui->path_list1->currentText().toStdString();
        if (open_dispatch.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_dispatch->setText("关闭调度");
                flag_dispatch=1;
            }else{
                ui->open_dispatch->setText("错误！");                
            }
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_dispatch == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "dispatch";
        if (open_dispatch.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_dispatch->setText("开启调度");
                flag_dispatch=0;
            }else{
                ui->open_dispatch->setText("错误！");                
            }
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}
/************************************************************************/
/*
 *action完成时的回调函数，一次性
 */
void ReinovoControl::doneCb(const actionlib::SimpleClientGoalState& state, const oryxbot_msgs::centerResultConstPtr& result)
{
    emit AppendText(QString::fromStdString(get_time())+"调度已完成");
    ui->start_dispatch->setText("开始调度");
    dispatch_status = 0;

    //  QScrollBar *scrollbar = ui->dispatch_output->verticalScrollBar();
    //  if (scrollbar)
    //  {
    //     scrollbar->setSliderPosition(scrollbar->maximum());
    //  }
}

/*
 *action启动时的回调函数，一次性
 */
void ReinovoControl::activeCb()
{
    emit AppendText(QString::fromStdString(get_time())+"启动完成");
}

/*
 *action收到反馈时的回调函数
 */
void ReinovoControl::feedbackCb(const oryxbot_msgs::centerFeedbackConstPtr& feedback)
{
    emit AppendText(QString::fromStdString(get_time())+QString::fromStdString(feedback->message.c_str()));
    // ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(feedback->message.c_str()));
    // ui->dispatch_output->moveCursor(QTextCursor::End);
}

void ReinovoControl::actionclient()
{
    client.waitForServer();
    oryxbot_msgs::centerGoal goal;
    goal.point_number = 0;
    client.sendGoal(goal,boost::bind(&ReinovoControl::doneCb, this, _1, _2),
                boost::bind(&ReinovoControl::activeCb, this),
                boost::bind(&ReinovoControl::feedbackCb, this, _1));
    ros::spin();
}


/************************************************************************/

//开始调度
void ReinovoControl::fstart_dispatch()
{
    if(flag_dispatch  == 1 && dispatch_status == 0){
        //actionthread = boost::thread(boost::bind(&ReinovoControl::actionclient, this));
        client.waitForServer();
        oryxbot_msgs::centerGoal goal;
        goal.point_number = 0;
        client.sendGoal(goal,boost::bind(&ReinovoControl::doneCb, this, _1, _2),
                    boost::bind(&ReinovoControl::activeCb, this),
                    boost::bind(&ReinovoControl::feedbackCb, this, _1));
        
        ui->start_dispatch->setText("停止调度");
        dispatch_status = 1;
        ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"开始调度");
    }else if(flag_dispatch  == 1 && dispatch_status == 1){
        // actionthread.interrupt();
        client.cancelAllGoals();

        ui->start_dispatch->setText("开始调度");
        dispatch_status = 0;
        ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"停止调度");
    }
}
//暂停调度
void ReinovoControl::fpause_dispatch()
{
    std_srvs::SetBool srv;
    srv.request.data = true;
    if(flag_dispatch  == 1&& dispatch_status == 1){
        if(suspend_client.call(srv)){
            dispatch_status = 0;
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"暂停调度");
        }else{
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"失败,未连接到调度节点");
        }
    }else{
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"未打开调度");
    }
}
//恢复调度
void ReinovoControl::frecover_dispatch()
{
    std_srvs::SetBool srv;
    srv.request.data = false;
    if(flag_dispatch  == 1){
        if(suspend_client.call(srv)){
            dispatch_status = 1;
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"暂停调度");
        }else{
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"失败,未连接到调度节点");
        }
    }else{
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"错误");
    }
}
 //刷新路径
void ReinovoControl::frefresh_path1()
{
    reinovo_control::pathsrv srv;
    if (get_path.call(srv))
    {
        v_path=srv.response.path;
        ui->path_list1->clear(); //清除列表
        if (v_path.size() > 0)
        {
            for (size_t i = 0; i < v_path.size(); i++)
            {
                ui->path_list1->addItem(QString::fromStdString(v_path[i].path_name)); //不带图标
            }
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"刷新路径");
        }
    }
}
//加载路径
void ReinovoControl::fload_path1()
{
    n.setParam("/center/path_name", ui->path_list1->currentText().toStdString());
    ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"加载路径 ："+ui->path_list1->currentText());
}
//自动充电
void ReinovoControl::fauto_charging()
{
    if (ui->auto_charging->isChecked()){
        if(ui->voltage_threshold->text() == ""){
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"未设置回充电压阈值");
        }else if(ui->charging_time->text() == ""){
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"未设置回充电时间");
        }else{
            fvoltage_threshold=ui->voltage_threshold->text().toFloat();
            fcharging_time=ui->charging_time->text().toFloat();
            ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"打开自动充电,电压阈值："+QString::number(fvoltage_threshold,'f',2)+"V, 回充时间："+QString::number(fcharging_time,'f',2));
        }
    }else{
        ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"停止自动充电");
    }
}
