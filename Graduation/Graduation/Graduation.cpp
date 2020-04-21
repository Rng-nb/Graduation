#include <iostream>
#include <fstream>
#include "STA.h"

//获取优先级最高的STA id
int getmaxprivity(std::vector<STA*> &sta_info) {
	double max_privity = 0.0;
	int max = -1;
	for (int i = 0; i < sta_info.size(); ++i) {
		STA *tmp = sta_info[i];
		double privity = tmp->get_privity_f1();
		privity > max_privity ? max_privity = privity, max = i : max = max, max_privity = max_privity;
	}
	return max;
}

/*
	比例公平调度不考虑时延
	每次调度一个调度周期t
*/
//用此时 速度/以往平均速度 作为优先级调度
void function1(std::vector<STA*> &sta_info) {
	std::ofstream out("tun.txt");
	int time = 150;
	int cost_sum = 0;
	int info_sum = 0;
	while (time--) {
		int id = getmaxprivity(sta_info);
		std::cout << "第" << 150 - time << "次调度id-" << id << std::endl;
		sta_info[id]->get_dispach_f1();
		cost_sum += sta_info[id]->info_did.back().first;
		info_sum += sta_info[id]->info_did.back().second.second;
		std::cout << "吞吐量：" << info_sum * 512.0 * 8.0 / (cost_sum * 1.0 / 1000.0) / 1000000 << std::endl;
		out << "吞吐量：" << info_sum * 512.0 * 8.0 / (cost_sum * 1.0 / 1000.0) / 1000000 << std::endl;
	} 
}

/*轮询调度比例公平优化*/
//每次调度30个sta，确认每次调度的sta编号如下
void function2(std::vector<STA*> &sta_info) {
	int start = 0;
	int end = 30;
	int time = 100;//假设一共调度100次
	while (time--) {
		if (start < end) {
			//优化目标--比例公平
			for (int j = start; j < end; ++j)
				std::cout << j << ' ';
			std::cout << std::endl;
		}
		else {
			for(int j = start; j < 50; ++j)
				std::cout << j << ' ';
			for(int j = 0; j < end; ++j)
				std::cout << j << ' ';
			std::cout << std::endl;
		}
		start = (start + 30) % 50;
		end = (end + 30) % 50;
	}
}

/*比例公平调度最大吞吐量分配资源*/

/*轮询调度比例公平优化目标*/

int main() {
	int sta_num = 50;
	std::vector<STA*> sta_info;
	std::ofstream data("data.txt");
	
	for (int i = 0; i < sta_num; ++i) {
		STA *sta = new STA();
		sta->produce_info();
		sta_info.push_back(sta);
		data << "STA " << i;
		for (int j = 0; j < sta->info.size(); ++j) {
			data << "Spedd-" << sta->info[j].first << "info_num-" << sta->info[j].second << '\t';
		}
		data << std::endl;
	}


	//std::cout << "调度1：" << std::endl;
	//function1(sta_info);
	//std::cout << "调度2：" << std::endl;
	//function2(sta_info);
	return 0;
}

