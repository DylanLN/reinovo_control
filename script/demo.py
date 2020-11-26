#!/usr/bin/env python
# -*- coding: UTF-8 -*-
'''JsonFile ROS Node'''
import sys
import os
import re
import json

import yaml

import rospy
import time
from reinovo_control.msg import action
from reinovo_control.msg import nav_goal
from reinovo_control.msg import path
from reinovo_control.msg import task

class JsonFile(object):
    action_tem = []
    task_list = []
    path_list = []
    data_path = path()
    data_goal = nav_goal()
    fun = 0

    def __init__(self):
        '''JsonFile'''
        print " list  over"

        rospy.init_node('JsonFile', anonymous=True)

        # #写入文件
        # print " list  over"

        # for i in range(0,5):
        #     data_task = task()
        #     data_task.name = "task" + str(i)
        #     data_task.step = 6
        #     for step in range(0,data_task.step):
        #         data_task.action.append(self.action_tem[step])
        #         for num in range(0,data_task.action[step].number):
        #             data_task.action[step].param[num] = num
        #     self.task_list.append(data_task)
        #     print "data_task :" 
        #     print data_task
        # print self.task_list

        # print " list  over"

        # #转换json
        # task_json = {'task':[]}
        # for i in range(0,5):
        #     task_dic = {'action' :[]}
        #     for step in range(0,self.task_list[i].step):
        #         action_dic = {}
        #         action_dic['name'] = self.task_list[i].action[step].name
        #         action_dic['number'] = self.task_list[i].action[step].number
        #         action_dic['param_name'] = self.task_list[i].action[step].param_name
        #         action_dic['param'] = self.task_list[i].action[step].param
        #         task_dic['action'].append(action_dic)
        #     name = {"name" :self.task_list[i].name}
        #     task_dic.update(name)
        #     step = {"step" :self.task_list[i].step}
        #     task_dic.update(step)
        #     task_json['task'].append(task_dic)
        #     print task_dic

        # print task_json
        # with open("task1.json", "w") as fp:
        #     fp.truncate()
        #     fp.write(json.dumps(task_json))
        # print "done"

        path_json = {'path':[]}
        for i in range(0,5):
            self.data_path.path_name='mpath'+str(i)
            self.data_path.number=3
            for num in range(0,self.data_path.number):
                self.data_path.task_name.append('')
                self.data_path.nav_name.append('goal'+str(num))
            path_dic = {}
            #添加name
            name = {"name" :self.data_path.path_name}
            path_dic.update(name)
            #添加 number
            number = {"number" : self.data_path.number}
            path_dic.update(number)
            nt_name = {'nav_name':[],'task_name':[]}
            for num in range(0,self.data_path.number):
                nt_name['nav_name'].append(self.data_path.nav_name[num])
                nt_name['task_name'].append(self.data_path.task_name[num])
            path_dic.update(nt_name)
            print path_dic
            path_json['path'].append(path_dic)


        with open("1.yaml", "w") as fp:
            fp.write(yaml.dump(path_json))
        print path_json


        navgoal_json = {'navgoal':[]}
        for i in range(0,5):
            self.data_goal.name='goal'+str(i)
            self.data_goal.pose.x = i
            self.data_goal.pose.y = i*2
            self.data_goal.pose.theta = i*3
            
            navgoal_dic = {}
            #添加name
            name = {"name" :self.data_goal.name}
            navgoal_dic.update(name)
            #添加 pose
            posex = {"pose.x" :self.data_goal.pose.x}
            navgoal_dic.update(posex)
            posey = {"pose.y" :self.data_goal.pose.y}
            navgoal_dic.update(posey)
            poseth = {"pose.th" :self.data_goal.pose.theta}
            navgoal_dic.update(poseth)

            print navgoal_dic
            navgoal_json['navgoal'].append(navgoal_dic)

        with open("goal.yaml", "w") as fp:
            fp.write(yaml.dump(navgoal_json))
        print navgoal_json








if __name__ == '__main__':
    try:
        JsonFile()
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")



