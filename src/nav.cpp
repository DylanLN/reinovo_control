#include "reinovo_control.h"

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
        reinovo_control::navgoalserver srv;
        for (size_t i = 0; i < v_navgoal.size(); i++)
        {
            ROS_INFO_STREAM("nav goal name:" << v_navgoal[i].name);
            srv.request.navgoal.push_back(v_navgoal[i]);
        }
        if (save_navgoal.call(srv))
        {
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"已删除 ");
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"错误：未连接到jsonfile节点！");   
        }
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
