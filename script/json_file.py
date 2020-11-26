#!/usr/bin/env python
# -*- coding: UTF-8 -*-
'''JsonFile ROS Node'''
import sys
import os
import re
import json
import yaml
import threading

import rospy
import time

from reinovo_control.msg import *
from reinovo_control.srv import *

from reinovo_control.msg import action
from reinovo_control.msg import nav_goal
from reinovo_control.msg import path
from reinovo_control.msg import task


class JsonFile(object):
    task_list = []
    #功能包路径
    pkg_path = ""

    #action模板
    action_tem = []
    flag_dispatch = False

#删除地图
    def deletemap_callback(self,request):
        path = self.pkg_path+'/maps/' + request.message
        if os.path.exists(path):  # 如果文件存在
            os.remove(path)
        path = path.replace('.yaml','.pgm')
        print path
        if os.path.exists(path):  # 如果文件存在
            os.remove(path)
            return askResponse(True,"已删除")
        else:
            return askResponse(False,"未发现该文件")
#切换地图
    def switchmap_callback(self,request):
        try:
            killmap = rospy.ServiceProxy('/robot_map/shutdown', ask)
            resp1 = killmap(True, "-")
        except rospy.ServiceException, e:
            print "Service call failed: %s"%e
            
        exe_str = "source ~/.bashrc && "
        exe_str1 = "roslaunch reinovo_control robot_mapserver.launch map_file:=" + request.message + "&"
        print exe_str+exe_str1
        os.system(exe_str+exe_str1)
        return askResponse(True,"正在切换")
#获取地图列表
    def getmap_callback(self,request):
        navgoal_list = []
        for root, dirs, files in os.walk(self.pkg_path+'/maps/'):  
            for file in files:
                if os.path.splitext(file)[1] == '.yaml':
                    file_path = os.path.join(file)  
                    data_goal = nav_goal()
                    data_goal.name = file_path
                    navgoal_list.append(data_goal)
        return navgoalsrvResponse(navgoal_list)

#读取目标点
    def navgoal_callback(self,request):
        navgoal_list = []
        #读取目标点
        with open(self.pkg_path+"/param/goal.yaml",'r') as load_f:
            load_goal = yaml.load(load_f)

        for i in range(0,len(load_goal['navgoal'])):
            data_goal = nav_goal()
            goal_dict = load_goal['navgoal'][i]

            data_goal.name = goal_dict['name'].encode('gbk')
            data_goal.pose.x = goal_dict['pose.x']
            data_goal.pose.y = goal_dict['pose.y']
            data_goal.pose.theta = goal_dict['pose.th']
            navgoal_list.append(data_goal)

        print navgoal_list
        return navgoalsrvResponse(navgoal_list)
#目标点生成文件
    def savenavgoal_callback(self,request):
        navgoal_list = request.navgoal
        navgoal_json = {'navgoal':[]}
        for i in range(0,len(navgoal_list)):
            goal_dict = navgoal_list[i]
            navgoal_dic = {}
            #添加name
            name = {"name" :goal_dict.name}
            navgoal_dic.update(name)
            #添加 pose
            posex = {"pose.x" :goal_dict.pose.x}
            navgoal_dic.update(posex)
            posey = {"pose.y" :goal_dict.pose.y}
            navgoal_dic.update(posey)
            poseth = {"pose.th" :goal_dict.pose.theta}
            navgoal_dic.update(poseth)
            navgoal_json['navgoal'].append(navgoal_dic)

        with open(self.pkg_path+"/param/goal.yaml", "w") as fp:
            fp.write(yaml.dump(navgoal_json))
        return navgoalserverResponse(True,"save")


    def path_callback(self,request):
        path_list = []
        #读取路径
        with open(self.pkg_path+"/param/path.yaml",'r') as load_f:
            load_dict = yaml.load(load_f)
        #print load_dict

        for i in range(0,len(load_dict['path'])):
            data_path = path()
            path_dict = load_dict['path'][i]
            #print path_dict
            data_path.path_name = path_dict['name'].encode('gbk')
            data_path.number = path_dict['number']
            for num in range(0,data_path.number):
                data_path.nav_name.append(path_dict['nav_name'][num].encode('gbk'))
                data_path.task_name.append(path_dict['task_name'][num].encode('gbk'))
            path_list.append(data_path)
            print data_path
            print '\n'
        return pathsrvResponse(path_list)

    def pathserver_callback(self,request):
        print "path 长度"
        print len(request.path)
        path_json = {'path':[]}
        for num in range(0,len(request.path)):
            path_dic = {}
            #添加name
            name = {"name" :request.path[num].path_name}
            path_dic.update(name)
            #添加 number
            number = {"number" : request.path[num].number}
            path_dic.update(number)
            nt_name = {'nav_name':[],'task_name':[]}
            print request.path[num]
            if request.path[num].number > 0:
                for num1 in range(0,request.path[num].number):
                    nt_name['nav_name'].append(request.path[num].nav_name[num1])
                    nt_name['task_name'].append(request.path[num].task_name[num1])
            else:
                pass
            path_dic.update(nt_name)
            print path_dic
            path_json['path'].append(path_dic)
        with open(self.pkg_path+"/param/path.yaml", "w") as fp:
            fp.truncate()
            fp.write(yaml.dump(path_json))
        print path_json
        self.path_list = request.path
        return pathserverResponse(True,"yes")
#保存地图
    def savemap_callback(self,request):
        exe_str = "source ~/.bashrc && "
        exe_str1 = "rosrun map_server map_saver -f " + self.pkg_path+'/maps/' + request.message
        os.system(exe_str+exe_str1)
        return askResponse(True,"创建完成")

    def gettemplate_callback(self,request):
        #读取参数
        action_tem = []
        fun = rospy.get_param('/json_file/fun',0)
        for i in range(0,fun):
            data_action = action()
            data_action.name = rospy.get_param('/json_file/action' + str(i) + '/name',"")
            data_action.number =  rospy.get_param('/json_file/action' + str(i) + '/num',0)
            for num in range(0,data_action.number):
                print str(num)+"   :" +str(data_action.number)
                param_name = rospy.get_param('/json_file/action' + str(i) + '/param' + str(num),"")
                data_action.param_name.append(param_name)
                data_action.param.append(0)
            action_tem.append(data_action)
        print action_tem
        return actionsrvResponse(action_tem)

    def gettask_callback(self,request):
        task_list = []
        #读取路径
        with open(self.pkg_path+"/param/task.yaml",'r') as load_f:
            load_dict = yaml.load(load_f)

        for i in range(0,len(load_dict['task'])):
            data_task = task()
            task_dict = load_dict['task'][i]

            data_task.name = task_dict['name'].encode('gbk')
            data_task.step = task_dict['step']
            for step in range(0,data_task.step):
                data_action = action()
                action_dict = task_dict['action'][step]
                data_action.name = action_dict['name'].encode('gbk')
                data_action.number = action_dict['number']
                for num in range(0,data_action.number):
                    data_action.param.append(action_dict['param'][num])
                    data_action.param_name.append(action_dict['param_name'][num].encode('gbk'))
                data_task.action.append(data_action)
            task_list.append(data_task)
        return tasksrvResponse(task_list)

#将task保存为文件
    def savetask_callback(self,request):
        #转换json
        print request
        task_json = {'task':[]}
        for i in range(0,len(request.task)):
            task_dic = {'action' :[]}
            for step in range(0,request.task[i].step):
                action_dic = {}
                action_dic['name'] = request.task[i].action[step].name
                action_dic['number'] = request.task[i].action[step].number
                action_dic['param_name'] = request.task[i].action[step].param_name
                action_dic['param'] = request.task[i].action[step].param
                task_dic['action'].append(action_dic)
            name = {"name" :request.task[i].name}
            task_dic.update(name)
            step = {"step" :request.task[i].step}
            task_dic.update(step)
            task_json['task'].append(task_dic)
            print task_dic

        with open(self.pkg_path+"/param/task.yaml", "w") as fp:
            #fp.truncate()
            fp.write(yaml.dump(task_json))
        return taskserverResponse(True,"保存成功")

#打开调度
    def opendispatch_callback(self,request):
        if self.flag_dispatch == 0 and request.mode == 1:
            exe_str = "source ~/.bashrc && "
            exe_str1 = "roslaunch reinovo_control robot_dispatch.launch pathname:="+ request.message
            print exe_str+exe_str1
            os.system(exe_str+exe_str1+"&")
            print "open ok"
            self.flag_dispatch = 1
            return askResponse(True,"调度正在启动")
        else:
            if self.flag_dispatch == 1 and request.mode == 0:
                try:
                    shutdown = rospy.ServiceProxy('/robot_dispatch/shutdown', ask)
                    resp = shutdown(True," ")
                    if resp.success ==  1:
                        self.flag_dispatch = 0
                        return askResponse(True,"调度已关闭")
                    elif resp.success ==  10:
                        return askResponse(False,"调度关闭失败")
                except rospy.ServiceException, e:
                    return askResponse(False,"未连接到调度节点")
        return askResponse(False,"发生错误,可能需要重启界面")



    def __init__(self):
        '''JsonFile'''
        rospy.init_node('JsonFile', anonymous=True)

        #获取路径
        s1 = rospy.Service('get_path',pathsrv,self.path_callback)
        #接收路径
        s2 = rospy.Service('pub_path',pathserver,self.pathserver_callback)
        #保存map
        s3 = rospy.Service('save_map',ask,self.savemap_callback)
        #获取目标点
        s4 = rospy.Service('get_goal',navgoalsrv,self.navgoal_callback)
        #目标点生成文件
        s41 = rospy.Service('save_navgoal',navgoalserver,self.savenavgoal_callback)
        #获取map
        s5 = rospy.Service('get_map',navgoalsrv,self.getmap_callback)
        #删除map
        s6 = rospy.Service('delete_map',ask,self.deletemap_callback)
        #切换map
        s7 = rospy.Service('switch_map',ask,self.switchmap_callback)

        #获取action模板
        s8 = rospy.Service('get_template',actionsrv,self.gettemplate_callback)

        #获取task
        s9 = rospy.Service('get_task',tasksrv,self.gettask_callback)

        #生成task
        s10 = rospy.Service('save_task',taskserver,self.savetask_callback)
        
        #打开调度
        s11 = rospy.Service('open_dispatch',ask,self.opendispatch_callback)
        self.pkg_path = rospy.get_param('/json_file/map_file',"no")
        print self.pkg_path
        rospy.spin()



if __name__ == '__main__':
    try:
        JsonFile()
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")