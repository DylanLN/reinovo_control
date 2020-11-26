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
/*******************    首页    **********************/
//打开关闭驱动
void ReinovoControl::fopen_driver()
{
    if(flag_driver == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "bringup";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_driver->setText("关闭驱动");
                flag_driver=1;
            }else
            {
                ui->open_driver->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_driver == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "bringup";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_driver->setText("打开驱动");
                flag_driver=0;
            }else{
                ui->open_driver->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}

//开始停止建图
void ReinovoControl::fopen_slam()
{
    if(flag_slam == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "slam";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_slam->setText("停止建图");
                flag_slam=1;
            }else
            {
                ui->open_slam->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_slam == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "slam";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_slam->setText("开始建图");
                flag_slam=0;
            }else{
                ui->open_slam->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}

//保存地图
void ReinovoControl::fsave_map()
{
    //获取lineEdit内容
    string text=ui->map_name->text().toStdString();
    if(text==""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：地图名不能为空！ ");
    }else{
        if(flag_slam == 1){
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"map name: " + ui->map_name->text());
            reinovo_control::ask srv;
            srv.request.message = text;
            if (save_map.call(srv))
            {
                if (srv.response.success == true)
                {
                    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已保存 ");
                }else{
                    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未保存 ");
                }
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
            }
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未打开slam");
        }
    }
}

//open all
void ReinovoControl::fopen_all()
{
    if(flag_openall == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "all";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_all->setText("close all");
                flag_openall=1;
            }else
            {
                ui->open_all->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_openall == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "all";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_all->setText("open all");
                flag_openall=0;
            }else{
                ui->open_all->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}
//other1
void ReinovoControl::fother1()
{
    if(flag_other1 == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "other1";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other1->setText("关闭");
                flag_other1=1;
            }else{
                ui->other1->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_other1 == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "other1";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other1->setText("其他模块1");
                flag_other1=0;
            }else{
                ui->other1->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}

//other2
void ReinovoControl::fother2()
{
    if(flag_other2 == 0){
        ui->other2->setText("关闭");
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"其他模块2 关闭");
        flag_other2=1;
    }else if(flag_other2 == 1){
        ui->other2->setText("其他模块2");
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"其他模块2 打开");
        flag_other2=0;
    }
}
//other3
void ReinovoControl::fother3()
{
    if(flag_other3 == 0){
        ui->other3->setText("关闭");
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"其他模块3 关闭");
        flag_other3=1;
    }else if(flag_other3 == 1){
        ui->other3->setText("其他模块3");
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"其他模块3 打开");
        flag_other3=0;
    }
}


/*******************    Teleop    **********************/
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

/*******************    导航    **********************/
//地图列表
void ReinovoControl::frefresh_map()
{
    reinovo_control::navgoalsrv srv;
    if(get_map.call(srv)){
        ui->map_list->clear(); //清除列表
        v_map.clear();
        for (size_t i = 0; i < srv.response.navgoal.size(); i++)
        {
            v_map.push_back(srv.response.navgoal[i].name);
            //ui->total_output->appendPlainText(QString::fromStdString(v_map[i]));
            ui->map_list->addItem(QString::fromStdString(v_map[i])); //不带图标
        }
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已刷新地图列表");   
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
    }
}
//切换地图
void ReinovoControl::fswitch_map()
{
    reinovo_control::ask srv;
    srv.request.message = ui->map_list->currentText().toStdString();
    if(srv.request.message == ""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"没有可切换的地图");
    }else{
        if(switch_map.call(srv)){
            if(srv.response.success == true){
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"切换"+ui->map_list->currentText()+"成功");
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：切换失败！");
            }
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
        }
    }
}
//删除地图
void ReinovoControl::fdelete_map()
{
    reinovo_control::ask srv;
    srv.request.message = ui->map_list->currentText().toStdString();
    if(srv.request.message == ""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"地图已经被你删完了");
    }else{
        if(delete_map.call(srv)){
            if(srv.response.success == true){
                ui->map_list->removeItem(ui->map_list->currentIndex());   //删除项目    
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"删除成功");
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：删除失败！");
            }
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
        }
    }
}
//开启导航
void ReinovoControl::fopen_nav()
{
    if(flag_nav == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        string str1 = ui->map_list->currentText().toStdString();
        srv.request.message="nav:"+str1;
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_nav->setText("关闭导航");
                flag_nav=1;
            }else{
                ui->open_nav->setText("错误！");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_nav == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "nav";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->open_nav->setText("开启导航");
                flag_nav=0;
            }else{
                ui->open_nav->setText("错误！");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}
//刷新导航点列表
void ReinovoControl::frefresh_target()
{
    reinovo_control::navgoalsrv srv;
    if(get_navgoal.call(srv)){
        v_navgoal = srv.response.navgoal;
        ui->target_list->clear(); //清除列表
        for (size_t i = 0; i < v_navgoal.size(); i++)
        {
            ui->total_output->appendPlainText(QString::fromStdString(v_navgoal[i].name));
            ui->target_list->addItem(QString::fromStdString(v_navgoal[i].name)); //不带图标
        }
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已刷新导航点");   
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
    }
}
//goto
void ReinovoControl::fgoto_target()
{
    //goto_pose
    ui_state = 1;
    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"正在前往目标点 ："+ui->target_list->currentText());   
}
//删除导航点
void ReinovoControl::fdelete_target()
{
    if (v_navgoal.size() == 0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"导航点已经被你删完了");
    }else{
        int curIndex =ui->target_list->currentIndex();
        if (v_navgoal.size() == 1)
        {
            disconnect(ui->target_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(ftarget_list(QString)));         //发布正vx
            ui->target_list->clear();
        }else{
            ui->target_list->removeItem(curIndex);
        }
        ROS_INFO_STREAM("curIndex:" << curIndex);
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已删除 " +QString::fromStdString(v_navgoal[curIndex].name));
        v_navgoal.erase(v_navgoal.begin()+curIndex);
    }
}
//获取姿态
void ReinovoControl::fget_gesture()
{
    reinovo_control::get_navgoal srv;
    if (flag_nav == 1)
    {
        if (get_pose.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->target_x->setValue(srv.response.pose.x);            
                ui->target_y->setValue(srv.response.pose.y);            
                ui->target_th->setValue(srv.response.pose.theta);
                ROS_INFO_STREAM(" X:" << srv.response.pose.x << " Y " <<srv.response.pose.y << " th" << srv.response.pose.theta);
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"获得姿态" );
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"获取失败");
            }
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"请先开启导航");
    }
}
//保存目标点
void ReinovoControl::fsave_target()
{
    string text=ui->target_name->text().toStdString();
    if(text==""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：导航点名不能为空！ ");
    }else
    {
        int flag = 0;
        for (size_t i = 0; i < v_navgoal.size(); i++)
        {
            if(text == v_navgoal[i].name){
                flag = 1;
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"与已有目标点重名");
                break;
            }
        }

        if(flag == 0){
            //给ComboBox添加路径 
            if (v_navgoal.size() == 0)
            {
                ui->target_list->addItem(ui->target_name->text());
                connect(ui->target_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(fpath_list(QString)));         //发布正vx
            }else{
                ui->target_list->addItem(ui->target_name->text());
            }
            reinovo_control::nav_goal goal_data;
            goal_data.name = text;
            goal_data.pose.x = ui->target_x->value();            
            goal_data.pose.y = ui->target_y->value();            
            goal_data.pose.theta = ui->target_th->value();            

            v_navgoal.push_back(goal_data);
            reinovo_control::navgoalserver srv;
            for (size_t i = 0; i < v_navgoal.size(); i++)
            {
                ROS_INFO_STREAM("nav goal name:" << v_navgoal[i].name);
                srv.request.navgoal.push_back(v_navgoal[i]);
            }
            if (save_navgoal.call(srv))
            {
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已保存 ");
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
            }    
        }
    }

}

/*******************    示教    **********************/
void ReinovoControl::frefresh_teach()  //刷新示教
{
    reinovo_control::actionsrv srv;
    if (get_actiontem.call(srv))
    {
        v_actiontem.clear();
        v_actiontem = srv.response.action;
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"刷新示教");
        ui->action_list->clear(); //清除列表
        for (size_t i = 0; i < v_actiontem.size(); i++)
        {
            ui->action_list->addItem(QString::fromStdString(v_actiontem[i].name)); //不带图标
        }
    }else
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
    }
    reinovo_control::tasksrv srv1;
    if (get_task.call(srv1))
    {
        v_task.clear();
        v_task = srv1.response.task;
        QListWidgetItem *aItem;
        ui->teach_list->clear(); //清除列表
        for (size_t i = 0; i < v_task.size(); i++)
        {
            aItem=new QListWidgetItem();
            aItem->setText(QString::fromStdString(v_task[i].name));
            aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
            ui->teach_list->addItem(aItem);
        }
    }
}
void ReinovoControl::fteach_list(QListWidgetItem* aItem)     //示教列表
{
    int cur = -1;
    for (size_t i = 0; i < v_task.size(); i++)
    {
        if (aItem->text().toStdString() == v_task[i].name)
        {
            cur = i;
            break;
        }
    }
    if (cur < 0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误:");
    }else{
        QListWidgetItem *aIteminfo;
        ui->teach_info->clear(); //清除信息
        for (size_t i = 0; i < v_task[cur].action.size(); i++)
        {
            aIteminfo=new QListWidgetItem();
            aIteminfo->setText(QString::fromStdString(v_task[cur].action[i].name));
            aIteminfo->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
            ui->teach_info->addItem(aIteminfo);
        }
    }
}

void ReinovoControl::fteach_rea()      //示教重现
{
    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"示教重现");
}
void ReinovoControl::fdelete_teach()   //删除示教
{
    if (v_task.size()==0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"示教任务已经被你删完了");
    }else if (v_task.size()==1)
    {
        int row=ui->teach_list->currentRow();
        QListWidgetItem* aItem=ui->teach_list->takeItem(row);
        delete aItem;
        v_task.clear();
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"删除示教");
    }else{
        int row=ui->teach_list->currentRow();
        QListWidgetItem* aItem=ui->teach_list->takeItem(row);
        delete aItem;
        v_task.erase(v_task.begin()+ui->teach_list->currentRow());
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"删除示教");
    }
}
void ReinovoControl::fcreate_teach()   //创建示教
{
    int cur = 0;
    for (size_t i = 0; i < v_task.size(); i++)
    {
        if (ui->teach_name->text().toStdString() == v_task[i].name)
        {
            cur = 1;
            break;
        }
    }

    if (cur == 0)
    {
        string text=ui->teach_name->text().toStdString();
        if(text==""){
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：示教名不能为空！ ");
        }else{
            QListWidgetItem *aItem=new QListWidgetItem("new item");
            aItem->setText(ui->teach_name->text());
            aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
            ui->teach_list->insertItem(ui->teach_list->currentRow()+1,aItem);
            //delete aItem;
            reinovo_control::task task;
            task.name = ui->teach_name->text().toStdString();
            v_task.insert(v_task.begin()+ui->teach_list->currentRow()+1,task);
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"创建示教"+ui->teach_name->text());

        }
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"重名");
    }
    
}
void ReinovoControl::fadd_action()     //添加action
{
    QListWidgetItem *aItem=new QListWidgetItem("new item");
    aItem->setText(ui->action_list->currentText());
    aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
    ui->teach_info->insertItem(ui->teach_info->currentRow()+1,aItem);

    int teach_list = ui->teach_list->currentRow();
    int teach_info = ui->teach_info->currentRow();

    v_task[teach_list].action.insert(v_task[teach_list].action.begin()+teach_info+1,v_actiontem[ui->action_list->currentIndex()]);
    v_task[teach_list].step++;

    //ROS_INFO_STREAM("V TASK:"<<v_task[teach_list].action[teach_info].name <<v_task[teach_list].step);
    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"添加action");
}
void ReinovoControl::fdelete_action()  //删除action
{
    int teach_list = ui->teach_list->currentRow();
    int teach_info = ui->teach_info->currentRow();
    if ( v_task[teach_list].action.size()==0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"action已经被你删完了");
    }else{
        int row=ui->teach_info->currentRow();
        v_task[teach_list].action.erase(v_task[teach_list].action.begin()+row);

        QListWidgetItem* aItem=ui->teach_info->takeItem(row);
        delete aItem;
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"删除action");
    }
}

void ReinovoControl::fteach_info(QListWidgetItem* aItem)    //示教信息teach_info
{
    int teach_list = ui->teach_list->currentRow();
    int teach_info = ui->teach_info->currentRow();
    //设置单元格 行
    ui->param_info->setRowCount(v_task[teach_list].action[teach_info].number);

    QTableWidgetItem *tableItem;
    //表格头
    QStringList headerText;
    headerText<<"参数"<<"data";
    //设置单元格2列
    ui->param_info->setColumnCount(headerText.count());

    for (size_t i = 0; i < ui->param_info->columnCount(); i++)
    {
        /* code for loop body */
        tableItem = new QTableWidgetItem(headerText.at(i));
        QFont font = tableItem->font();
        font.setPointSize(9);
        tableItem->setFont(font);
        ui->param_info->setHorizontalHeaderItem(i,tableItem);
    }
    //指定列宽
    ui->param_info->setColumnWidth(0,110);
    ui->param_info->setColumnWidth(1,60);
    
    QString param_name;
    QString data;
    for (size_t i = 0; i < v_task[teach_list].action[teach_info].number; i++)
    {
        param_name=QString::fromStdString(v_task[teach_list].action[teach_info].param_name[i]);
        //设置第i行第0列
        ui->param_info->setItem(i,0,new QTableWidgetItem(param_name));
        data = QString::number(v_task[teach_list].action[teach_info].param[i],'f',2);
        //设置第i行第1列
        ui->param_info->setItem(i,1,new QTableWidgetItem(data));
        //设置行高
        ui->param_info->setRowHeight(i,30);
    }
}
//参数信息单元格发生变化时
void ReinovoControl::fparam_info(int currentRow, int currentColumn)
{
    if(currentColumn == 1){
        //关闭信号
        ui->total_output->blockSignals(true);

        QTableWidgetItem* item=ui->param_info->item(currentRow,currentColumn); //获取单元格的 Item
        //获取数据
        float data=item->text().toFloat();

        int teach_list = ui->teach_list->currentRow();
        int teach_info = ui->teach_info->currentRow();

        if (data!=v_task[teach_list].action[teach_info].param[currentRow])
        {
            v_task[teach_list].action[teach_info].param[currentRow]=data;
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"参数："+ui->param_info->item(currentRow,0)->text()+" 修改为："+QString::number(v_task[teach_list].action[teach_info].param[currentRow],'f',2));
            cout << "data : " << data << endl;
        }
        //打开信号
        ui->total_output->blockSignals(false);
    }
}

void ReinovoControl::fmakefile2()      //生成文件
{
    reinovo_control::taskserver srv;
    srv.request.task = v_task;
    if (save_task.call(srv))
    {
        if (srv.response.success == true)
        {
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"文件已保存");
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"文件保存失败");
        }
        
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到JsonFile节点");
    }
}


/*******************    组合    **********************/
//刷新路径
void ReinovoControl::frefresh_path()
{
    //ROS_INFO("刷新");
    reinovo_control::pathsrv srv;
    if (get_path.call(srv))
    {
        v_path=srv.response.path;
        ui->path_list->clear(); //清除列表
        if (v_path.size() > 0)
        {
            for (size_t i = 0; i < v_path.size(); i++)
            {
                ui->path_list->addItem(QString::fromStdString(v_path[i].path_name)); //不带图标
            }

            //刷新路径信息
            //设置单元格五行
            ui->path_info->setRowCount(v_path[0].number);

            QTableWidgetItem *tableItem;
            //表格头
            QStringList headerText;
            headerText<<"导航点"<<"示教程序";
            //设置单元格2列
            ui->path_info->setColumnCount(headerText.count());
            for (size_t i = 0; i < ui->path_info->columnCount(); i++)
            {
                /* code for loop body */
                tableItem = new QTableWidgetItem(headerText.at(i));
                QFont font = tableItem->font();
                font.setPointSize(9);
                tableItem->setFont(font);
                ui->path_info->setHorizontalHeaderItem(i,tableItem);
                //指定列宽
                ui->path_info->setColumnWidth(i,90);
            }
            QString param_name;
            QString data;
            for (size_t i = 0; i < ui->path_info->rowCount(); i++)
            {
                param_name=QString::fromStdString(v_path[0].nav_name[i]);
                //设置第i行第0列
                ui->path_info->setItem(i,0,new QTableWidgetItem(param_name));
                //设置第i行第1列
                ui->path_info->setItem(i,1,new QTableWidgetItem(QString::fromStdString(v_path[0].task_name[i])));
                //设置行高
                ui->path_info->setRowHeight(i,30);
            }
            //设为不可编辑
            ui->path_info->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->target_list1->clear(); //清除列表

            for (size_t i = 0; i < v_navgoal.size(); i++)
            {
                ui->target_list1->addItem(QString::fromStdString(v_navgoal[i].name)); //不带图标
            }
            ui->teach_list2->clear(); //清除列表
            for (size_t i = 0; i < v_task.size(); i++)
            {
                ui->teach_list2->addItem(QString::fromStdString(v_task[i].name)); //不带图标
            }

            reinovo_control::navgoalsrv srv;
            if(get_navgoal.call(srv)){
                v_navgoal = srv.response.navgoal;
                ui->target_list1->clear(); //清除列表
                for (size_t i = 0; i < v_navgoal.size(); i++)
                {
                    ui->target_list1->addItem(QString::fromStdString(v_navgoal[i].name)); //不带图标
                }
            }else{
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"刷新组合页面完毕");   
        }else{
            disconnect(ui->path_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(fpath_list(QString)));         //发布正vx
            ui->path_list->clear();
        }
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
    }
}
//路径列表
void ReinovoControl::fpath_list(QString text)
{

    ui->path_info->clear();
    if(text == "" || ui->path_list->currentIndex()<0){
        ui->path_info->setRowCount(1);
    }else{
        //取得当前path 号码
        int curIndex =ui->path_list->currentIndex();
        //设置单元格五行
        ui->path_info->setRowCount(v_path[curIndex].number);
        QTableWidgetItem *tableItem;
        //表格头
        QStringList headerText;
        headerText<<"导航点"<<"示教程序";
        //设置单元格2列
        ui->path_info->setColumnCount(headerText.count());
        //设置单元格2列
        ui->path_info->setColumnCount(headerText.count());
        for (size_t i = 0; i < ui->path_info->columnCount(); i++)
        {
            /* code for loop body */
            tableItem = new QTableWidgetItem(headerText.at(i));
            QFont font = tableItem->font();
            font.setPointSize(9);
            tableItem->setFont(font);
            ui->path_info->setHorizontalHeaderItem(i,tableItem);
            //指定列宽
            ui->path_info->setColumnWidth(i,110);
        }
        QString param_name;
        QString data;
        for (size_t i = 0; i < ui->path_info->rowCount(); i++)
        {
            param_name=QString::fromStdString(v_path[curIndex].nav_name[i]);
            //设置第i行第0列
            ui->path_info->setItem(i,0,new QTableWidgetItem(param_name));
            //设置第i行第1列
            ui->path_info->setItem(i,1,new QTableWidgetItem(QString::fromStdString(v_path[curIndex].task_name[i])));
            //设置行高
            ui->path_info->setRowHeight(i,30);
        }
    }
}
//删除路径
void ReinovoControl::fdelete_path()
{
    //删除ComboBox路径
    if (v_path.size() == 0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"路径已经被你删完了");
    }else{
        int curIndex =ui->path_list->currentIndex();
        if (v_path.size() == 1)
        {
            disconnect(ui->path_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(fpath_list(QString)));         //发布正vx
            ui->path_list->clear();
            //ui->path_list->addItem(QString::fromStdString(" ")); //不带图标
        }else{
            ui->path_list->removeItem(curIndex);
        }
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已删除 " +QString::fromStdString(v_path[curIndex].path_name));
        v_path.erase(v_path.begin()+curIndex);
    }
}
//创建路径
void ReinovoControl::fcreate_path()
{
    std_msgs::String msg;
    //获取lineEdit内容
    string text=ui->path_name->text().toStdString();
    if(text==""){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：路径名不能为空！ ");
    }else{
        if (v_path.size() == 0)
        {
            ui->path_list->addItem(QString::fromStdString(text));
            connect(ui->path_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(fpath_list(QString)));         //发布正vx
            reinovo_control::path path_data;
            path_data.path_name = text;
            v_path.push_back(path_data);
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已保存 ");
        }else{
            int flag = 0;
            for (size_t i = 0; i < v_path.size(); i++)
            {
                if(text == v_path[i].path_name){
                    flag = 1;
                    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"与已有路径重名");
                    break;
                }
            }
            if(flag == 0){
                //给ComboBox添加路径 
                if (v_path.size() == 0)
                {
                    ui->path_list->addItem(ui->path_name->text());
                    connect(ui->path_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(fpath_list(QString)));         //发布正vx
                }else{
                    ui->path_list->addItem(ui->path_name->text());
                }
                reinovo_control::path path_data;
                path_data.path_name = text;
                v_path.push_back(path_data);
                // for (size_t i = 0; i < v_path.size(); i++)
                // {
                //     ROS_INFO_STREAM("路径列表:" << v_path[i].path_name);
                // }
                ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已保存 ");
            }
        }
    }
}
// void fpath_info();      //路径信息
//删除导航点
void ReinovoControl::fdelete_target1()
{
    int curRow = ui->path_info->currentRow();
    if (v_path[ui->path_list->currentIndex()].number == 0)
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"该路径已经没有导航点了");
    }else if(curRow < 0){
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未选中");
    }else{
        v_path[ui->path_list->currentIndex()].nav_name.erase(v_path[ui->path_list->currentIndex()].nav_name.begin()+curRow);
        v_path[ui->path_list->currentIndex()].task_name.erase(v_path[ui->path_list->currentIndex()].task_name.begin()+curRow);
        v_path[ui->path_list->currentIndex()].number = v_path[ui->path_list->currentIndex()].nav_name.size() ;
        ui->path_info->removeRow(curRow);

        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"删除导航点");
    }
}
//添加导航点
void ReinovoControl::fadd_target()
{
    int curRow = ui->path_info->currentRow();

    ui->path_info->insertRow(curRow+1);
    QString navgoal_name;
    navgoal_name= ui->target_list1->currentText();
    ui->path_info->setItem(curRow+1,0,new QTableWidgetItem(navgoal_name));

    v_path[ui->path_list->currentIndex()].nav_name.insert(v_path[ui->path_list->currentIndex()].nav_name.begin()+curRow+1,navgoal_name.toStdString());
    v_path[ui->path_list->currentIndex()].task_name.insert(v_path[ui->path_list->currentIndex()].task_name.begin()+curRow+1,"");

    v_path[ui->path_list->currentIndex()].number = v_path[ui->path_list->currentIndex()].nav_name.size();
    ROS_INFO_STREAM(" 现有导航点  "<< v_path[ui->path_list->currentIndex()].number);
    ui->total_output->appendPlainText(QString::fromStdString(get_time())+"添加导航点:"+navgoal_name);
}
//挂载
void ReinovoControl::fmount()
{
    int curRow = ui->path_info->currentRow();
    if (curRow >= 0)
    {
        ui->path_info->setItem(curRow,1,new QTableWidgetItem(ui->teach_list2->currentText()));
        v_path[ui->path_list->currentIndex()].task_name[curRow] = ui->teach_list2->currentText().toStdString();
        // for (size_t i = 0; i < v_path[ui->path_list->currentIndex()].task_name.size(); i++)
        // {
        //     ROS_INFO_STREAM("task name:" << v_path[ui->path_list->currentIndex()].task_name[i]);
        // }
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"挂载程序:"+ui->teach_list2->currentText());
    }else
    {
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"请先选中导航点");
    }
}
//保存文件
void ReinovoControl::fmakefile()
{
    reinovo_control::pathserver srv;
    srv.request.path = v_path;
    if (pub_path.call(srv)){
        if (srv.response.success == 1)
        {
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"保存成功");
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"保存失败");
        }
    }else{
        ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到节点jsonfile");
    }
}


/*******************    调度    **********************/
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
    ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"调度已完成");
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
    ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"启动完成");
}

/*
 *action收到反馈时的回调函数
 */
void ReinovoControl::feedbackCb(const oryxbot_msgs::centerFeedbackConstPtr& feedback)
{
    ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(feedback->message.c_str()));
    ui->dispatch_output->moveCursor(QTextCursor::End);
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
        actionthread = boost::thread(boost::bind(&ReinovoControl::actionclient, this));
        ui->start_dispatch->setText("停止调度");
        dispatch_status = 1;
        ui->dispatch_output->appendPlainText(QString::fromStdString(get_time())+"开始调度");
    }else if(flag_dispatch  == 1 && dispatch_status == 1){
        client.cancelAllGoals();
        actionthread.interrupt();
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

void ReinovoControl::ui_thread()
{
    ros::Rate loop(10);
    while(ros::ok()){
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
        loop.sleep();
    }
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

    connect(ui->vx_plus, SIGNAL(released()), this, SLOT(fvel_stop()));         //发布正vx
    // connect(ui->vx_minus, SIGNAL(released()), this, SLOT(fvel_stop()));         //发布正vx
    // connect(ui->vy_plus, SIGNAL(released()), this, SLOT(fvel_stop()));         //发布正vx
    // connect(ui->vy_minus, SIGNAL(released()), this, SLOT(fvel_stop()));         //发布正vx
    // connect(ui->vth_plus, SIGNAL(released()), this, SLOT(fvel_stop()));         //发布正vx
    // connect(ui->vth_minus, SIGNAL(released()), this, SLOT(fvel_stop()));         //发布正vx



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
    flag_other1=0;
    flag_other2=0;
    flag_other3=0;
    //Teleop
    flag_speed=0;
    //导航
    flag_nav=0;
    //调度
    flag_dispatch=0;
    flag_charging=0;
    dispatch_status = 0;

    ui_state=0;

    //保存ｍａｐ
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

    //暂停调度
    suspend_client =  nh_.serviceClient<std_srvs::SetBool>("center_suspend");

    uithread = boost::thread(boost::bind(&ReinovoControl::ui_thread, this));

}

ReinovoControl::~ReinovoControl()
{
    uithread.interrupt();
    delete timer_;
    delete ui;
    ROS_DEBUG_STREAM("Destory");
}

#include "pluginlib/class_list_macros.h"
PLUGINLIB_EXPORT_CLASS(ReinovoControl, rviz::Panel)
