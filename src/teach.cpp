#include "reinovo_control.h"

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
