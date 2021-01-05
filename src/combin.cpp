#include "reinovo_control.h"

/*******************    组合    **********************/
bool ReinovoControl::combin_init()
{
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

    return true;
}


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
