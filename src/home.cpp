#include "reinovo_control.h"

/*******************    首页    **********************/

void ReinovoControl::freinovo_control()
{
    ui->total_output->appendPlainText(QString::fromStdString(get_time())+ "reinovo control 是一款开源的机器人控制软件,用户可根据自己需求修改.可通过登录深圳市元创兴科技有限公司官网www.reinovo.com或其他方式联系我们." ); 
}


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
    if(flag_other[0] == 0){
        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "other1";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other1->setText("关闭");
                flag_other[0]=1;
            }else{
                ui->other1->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_other[0] == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "other1";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other1->setText(QString::fromStdString(strother[0]));
                flag_other[0]=0;
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
    if(flag_other[1] == 0){

        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "other2";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other2->setText("关闭");
                flag_other[1]=1;
            }else{
                ui->other2->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_other[1] == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "other2";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other2->setText(QString::fromStdString(strother[1]));
                flag_other[1]=0;
            }else{
                ui->other2->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}
//other3
void ReinovoControl::fother3()
{
    if(flag_other[2] == 0){

        reinovo_control::ask srv;
        srv.request.mode = 1;
        srv.request.message = "other3";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other3->setText("关闭");
                flag_other[2]=1;
            }else{
                ui->other3->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }else if(flag_other[2] == 1){
        reinovo_control::ask srv;
        srv.request.mode = 0;
        srv.request.message = "other3";
        if (key_client.call(srv))
        {
            if (srv.response.success == true)
            {
                ui->other3->setText(QString::fromStdString(strother[0]));
                flag_other[2]=0;
            }else{
                ui->other3->setText("错误");                
            }
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+QString::fromStdString(srv.response.message));
        }else{
            ui->total_output->appendPlainText(QString::fromStdString(get_time())+"未连接到底层节点");
        }
    }
}
