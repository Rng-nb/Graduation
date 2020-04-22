#include "STA.h"
#include <map>

/*
	比例公平调度不考虑时延
	每次调度一个调度周期t
*/
//获得一个优先级
void getPrivity_PF(std::vector<STA*> &sta_info, std::vector<std::pair<int, double> > &privity, int timenow, int i) {
	int j = 0;
	double privity_tmp = sta_info[i]->getPrivity_f1(timenow);
	for (j = 0; j < privity.size(); ++j) {
		if (privity_tmp > privity[j].second)
			break;
	}
	privity.insert(privity.begin() + j, std::pair<int, double>(i, privity_tmp));
}
//比例公平调度 比例公平优化
void function1(std::vector<STA*> &sta_info) {
	std::vector<std::pair<int, double>> privity;//存储一次调度的各个优先级
	std::vector<int> dispach_id;//存储一个调度选择的id
	std::vector<double> speed;
	int time = 150;
	double cost_sum = 0;
	int info_sum = 0;
	while (time--) {
		std::cout << "第" << 150 - time << "次调度" << std::endl;
		for(int i = 0; i < sta_info.size(); ++i)
			getPrivity_PF(sta_info,privity,150 - time,i);
		for (int i = 0; i < privity.size(); ++i) 
			std::cout << "id =" << privity[i].first << " privity= " << privity[i].second << std::endl;
		//选择优先级最高的5个调度
		for (int i = 0; i < privity.size() && i < 5; ++i) {
			dispach_id.push_back(privity[i].first);
		}
		//调度，每次调度的最小时间单位为1ms,剩下时间优先满足高优先级
		double timeleft = 5.0;
		for (int i = 0; i < dispach_id.size(); ++i) {
			sta_info[dispach_id[i]]->getDispach(timeleft);
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
		privity.clear();
		dispach_id.clear();
	}
}
/*轮询调度比例公平优化*/
void function2(std::vector<STA*> &sta_info) {
	std::vector<std::pair<int, double>> privity;//存储一次调度的各个优先级
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
			for (int i = start; i < end; ++i)
				getPrivity_PF(sta_info, privity, 150 - time, i);
		}
		else {
			for (int i = start; i < 50; ++i) 
				getPrivity_PF(sta_info, privity, 150 - time, i);
			for (int i = 0; i < end; ++i) 
				getPrivity_PF(sta_info, privity, 150 - time, i);
		}
		for (int i = 0; i < privity.size() && i < 5; ++i) {
			dispach_id.push_back(privity[i].first);
		}
		//调度，每次调度的最小时间单位为1ms,剩下时间优先满足高优先级
		double timeleft = 5.0;
		for (int i = 0; i < dispach_id.size(); ++i) {
			sta_info[dispach_id[i]]->getDispach(timeleft);
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
		privity.clear();
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

	std::cout << "比例公平调度：" << std::endl;
	function1(sta_info);
	std::cout << "轮询调度：" << std::endl;
	function2(sta_info);
	return 0;
}