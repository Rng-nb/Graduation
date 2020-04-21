#include "STA.h"
#include <map>

//获取优先级最高的STA id
int getmaxprivity(std::vector<STA*> &sta_info, int timenow) {
	double max_privity = 0.0;
	int max = -1;
	for (int i = 0; i < sta_info.size(); ++i) {
		STA *tmp = sta_info[i];
		double privity = tmp->getPrivity_f1(timenow);
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
	double cost_sum = 0;
	int info_sum = 0;
	std::vector<double> speed;
	while (time--) {
		int id = getmaxprivity(sta_info,150 - time);
		std::cout << "第" << 150 - time << "次调度id-" << id << std::endl;
		sta_info[id]->getDispach_f1();
		cost_sum += sta_info[id]->info_dispached.back().time;//ms
		info_sum += sta_info[id]->info_dispached.back().info_num;
		std::cout << "吞吐量：" << info_sum * 512.0 * 8.0 / (cost_sum / 1000.0) / 1000000 << "Mb/s"<< std::endl;
		out << "吞吐量：" << info_sum * 512.0 * 8.0 / (cost_sum / 1000.0) / 1000000 << "Mb/s"<< std::endl;
		
		speed.push_back(sta_info[id]->info_dispached.back().info_speed);
		int m = speed.size();
		double speedsum_x = 0.0;
		double speedsum_xx = 0.0;
		for (int i = 0; i < m; ++i) {
			speedsum_x += speed[i];
			speedsum_xx += speed[i] * speed[i];
		}
		std::cout << "Jain：" << speedsum_x * speedsum_x / (m * 1.0) / speedsum_xx << std::endl;
	} 
}

void function2(std::vector<STA*> &sta_info) {
	std::vector<std::pair<int, double>> priority;//存储一次调度的各个优先级
	std::vector<int> dispach_id;//存储一个调度选择的id
	std::vector<double> speed;
	int time = 150;
	double cost_sum = 0;
	int info_sum = 0;
	while (time--) {
		std::cout << "第" << 150 - time << "次调度" << std::endl;
		for (int i = 0; i < sta_info.size(); ++i) {
			double priority_tmp = sta_info[i]->getPrivity_f1(150 - time);
			int j = 0;
			for (j = 0; j < priority.size(); ++j) {
				if (priority_tmp > priority[j].second)
					break;
			}
			priority.insert(priority.begin() + j, std::pair<int, double>(i, priority_tmp));
		}
		//选择优先级最高的5个调度
		for (int i = 0; i < priority.size() && i < 5; ++i) {
			dispach_id.push_back(priority[i].first);
		}
		//调度，每次调度的最小时间单位为1ms,剩下时间优先满足高优先级
		double timeleft = 5.0;
		for (int i = 0; i < dispach_id.size(); ++i) {
			sta_info[dispach_id[i]]->getDispach_f2(timeleft);
			cost_sum += sta_info[dispach_id[i]]->info_dispached.back().time;//ms
			info_sum += sta_info[dispach_id[i]]->info_dispached.back().info_num;
			speed.push_back(sta_info[dispach_id[i]]->info_dispached.back().info_speed);
		}

		std::cout << "吞吐量：" << info_sum * 512.0 * 8.0 / (cost_sum / 1000.0) / 1000000 << "Mb/s" << std::endl;
		int m = speed.size();
		double speedsum_x = 0.0;
		double speedsum_xx = 0.0;
		for (int i = 0; i < m; ++i) {
			speedsum_x += speed[i];
			speedsum_xx += speed[i] * speed[i];
		}
		std::cout << "Jain：" << speedsum_x * speedsum_x / (m * 1.0) / speedsum_xx << std::endl;
		priority.clear();
		dispach_id.clear();
	}
}
/*轮询调度比例公平优化*/
//每次调度30个sta，确认每次调度的sta编号如下
void function3(std::vector<STA*> &sta_info) {
	std::vector<std::pair<int, double>> priority;//存储一次调度的各个优先级
	std::vector<int> dispach_id;//存储一个调度选择的id
	std::vector<double> speed;
	int start = 0;
	int end = 5;
	int time = 150;//假设一共调度100次
	double cost_sum = 0;
	int info_sum = 0;
	while (time--) {
		if (start < end) {
			//优化目标--比例公平
			for (int i = start; i < end; ++i) {
				double priority_tmp = sta_info[i]->getPrivity_f1(150 - time);
				int j = 0;
				for (j = 0; j < priority.size(); ++j) {
					if (priority_tmp > priority[j].second)
						break;
				}
				priority.insert(priority.begin() + j, std::pair<int, double>(i, priority_tmp));
			}
		}
		else {
			for (int i = start; i < 50; ++i) {
				double priority_tmp = sta_info[i]->getPrivity_f1(150 - time);
				int j = 0;
				for (j = 0; j < priority.size(); ++j) {
					if (priority_tmp > priority[j].second)
						break;
				}
				priority.insert(priority.begin() + j, std::pair<int, double>(i, priority_tmp));
			}
			for (int i = 0; i < end; ++i) {
				double priority_tmp = sta_info[i]->getPrivity_f1(150 - time);
				int j = 0;
				for (j = 0; j < priority.size(); ++j) {
					if (priority_tmp > priority[j].second)
						break;
				}
				priority.insert(priority.begin() + j, std::pair<int, double>(i, priority_tmp));
			}
		}
		for (int i = 0; i < priority.size() && i < 5; ++i) {
			dispach_id.push_back(priority[i].first);
		}
		//调度，每次调度的最小时间单位为1ms,剩下时间优先满足高优先级
		double timeleft = 5.0;
		for (int i = 0; i < dispach_id.size(); ++i) {
			sta_info[dispach_id[i]]->getDispach_f2(timeleft);
			cost_sum += sta_info[dispach_id[i]]->info_dispached.back().time;//ms
			info_sum += sta_info[dispach_id[i]]->info_dispached.back().info_num;
			speed.push_back(sta_info[dispach_id[i]]->info_dispached.back().info_speed);
		}
		std::cout << "吞吐量：" << info_sum * 512.0 * 8.0 / (cost_sum / 1000.0) / 1000000 << "Mb/s" << std::endl;
		int m = speed.size();
		double speedsum_x = 0.0;
		double speedsum_xx = 0.0;
		for (int i = 0; i < m; ++i) {
			speedsum_x += speed[i];
			speedsum_xx += speed[i] * speed[i];
		}
		std::cout << "Jain：" << speedsum_x * speedsum_x / (m * 1.0) / speedsum_xx << std::endl;
		start = (start + 5) % 50;
		end = (end + 5) % 50;
		priority.clear();
		dispach_id.clear();
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
		sta->produceInfo();
		sta_info.push_back(sta);
		//数据写入文件
		data << "STA " << i;
		for (int j = 0; j < sta->info_produce.size(); ++j) {
			data << "time:" << sta->info_produce[j].time << " speed:" << sta->info_produce[j].info_speed << " num:" << sta->info_produce[j].info_num << std::endl;
		}
		data << std::endl;
	}

	std::cout << "调度1：" << std::endl;
	function1(sta_info);
	//std::cout << "调度2：" << std::endl;
	//function2(sta_info);
	//std::cout << "调度3：" << std::endl;
	//function3(sta_info);
	return 0;
}