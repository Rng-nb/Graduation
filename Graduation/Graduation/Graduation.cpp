// Graduation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//时间间隔序列T呈指数分布
//假设共有100个时间周期
#include <iostream>
#include "STA.h"

int getmaxprivity(std::vector<STA*> &sta_info) {
	double max_privity = 0.0;
	int max = -1;
	for (int i = 0; i < sta_info.size(); ++i) {
		STA *tmp = sta_info[i];
		double privity = tmp->get_privity();
		privity > max_privity ? max_privity = privity, max = i : max = max, max_privity = max_privity;
	}
	return max;
}
int main() {
	int sta_num = 50;
	std::vector<STA*> sta_info;
	for (int i = 0; i < sta_num; ++i) {
		STA *sta = new STA();
		sta->produce_info();
		sta_info.push_back(sta);
	}
	/*轮询调度最大吞吐量优化*/
	/*
	//每次调度15个sta，确认每次调度的sta编号如下
	int start = 0;
	int end = 14;
	int time = 20;//假设一共调度20次
	while (time--) {
		if (start < end) {
			//优化目标--比例公平
			for (int j = start; j < end; ++j)
				std::cout << j << ' ';
			std::cout << std::endl;
		}
		else {
			for(int j = start; j < 20; ++j)
				std::cout << j << ' ';
			for(int j = 0; j < end; ++j)
				std::cout << j << ' ';
			std::cout << std::endl;
		}
		start = (start + 15) % 20;
		end = (end + 15) % 20;
	}*/
	/*轮询调度比例公平优化目标*/
	/*
	比例公平调度不考虑时延
	每次调度一个调度周期t
	*/
	//用此时速度/以往平均速度作为优先级调度
	int time = 150;
	while (time--) {
		int id = getmaxprivity(sta_info);
		sta_info[id]->get_dispach();
		std::cout << "第" << 150 - time << "次调度id-" << id << std::endl;
	}
	/*比例公平调度最大吞吐量分配资源*/
	return 0;
}

