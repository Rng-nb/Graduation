// Graduation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//时间间隔序列T呈指数分布
//假设共有100个时间周期
#include <iostream>
#include <random>
#include <vector>

void initset() { //数据量的设定
	std::default_random_engine randrom;//随机数引擎
	enum channel_space
	{
		cs1 = 5,
		cs2 = 10,
		cs3 = 20
	};
	struct speed
	{
		int mcs;
		channel_space cs;
	};
	double lambda1, lambda2;
	const int number = 10;//暂时假设共有10个时间间隔,也可以设为变量
	int num_rt;
	std::vector<std::vector<double>> time_gap;
	std::vector<std::vector<int>> num_info;
	std::uniform_real_distribution<double> distribution_un(1, 10);

	//std::cin >> num_rt;//RT数量
	num_rt = 10;
	for (int i = 0; i < num_rt; ++i) {
		lambda1 = distribution_un(randrom);
		std::exponential_distribution<double> distribution_ex(lambda1);//时间间隔符合参数为lambda1的指数分布
		std::vector<double> time_tmp;
		for (int j = 0; j < number; ++j) {
			double num_rand = distribution_ex(randrom);
			time_tmp.push_back(num_rand);
		}
		time_gap.push_back(time_tmp);
		time_tmp.clear();
	}

	for (int i = 0; i < num_rt; ++i) {
		lambda2 = distribution_un(randrom);
		std::vector<int> num_tmp;
		std::vector<double> time_tmp = time_gap[i];
		for (int j = 0; j < number; ++j) {
			double lambda = lambda2 * time_tmp[j] * 100;
			std::poisson_distribution<int> distribution_po(lambda);//时间间隔t内的数据量符合参数为lambda2*t的泊松分布
			double num_rand = distribution_po(randrom);
			num_tmp.push_back(num_rand);
		}
		num_info.push_back(num_tmp);
		num_tmp.clear();
	}
	for (int i = 0; i < time_gap.size(); ++i) {
		for (int j = 0; j < time_gap[i].size(); ++j) {
			std::cout << "RT" << i << "-gap" << j << ":" << time_gap[i][j]
				<< " 数据量: " << num_info[i][j] << std::endl;
		}
	}
}
int main() {
	initset();
	return 0;
}

