#include "STA.h"

STA::STA()
{
	std::uniform_real_distribution<double> random_un(1,5);//1-5均匀分布的随机double
	lambda_ex_time = random_un(random);//设定一个随机的初始λ值
	lambda_po_info = random_un(random);//设定一个随机的初始λ值
	channel_space = 5;
	info_dispached.clear();
}

STA::~STA()
{
}

void STA::produceInfo()
{
	int i = 150;
	bool flag_first = true; //标志第一次信息产生
	double time_tmp = 0.0;
	while (i--) {
		std::exponential_distribution<double> random_time(lambda_ex_time);//时间间隔符合参数为lambda1的指数分布
		double time_rand = random_time(random);//随机一个时间间隔

		double lambda = lambda_po_info * time_rand * 1000; //λt呈泊松分布
		std::poisson_distribution<int> random_info(lambda);//时间间隔t内的数据量符合参数为lambda2*t的泊松分布
		int info_rand = random_info(random);//随机一个数据包数量
		
		std::uniform_int_distribution<int> random_un(1, 8);//随机一个速度值
		channel_speed = speed[random_un(random) - 1] * (channel_space / 5.0);

		Info info_tmp; //存储产生的一次随机信息
		flag_first == true ? info_tmp.time = 0.0, flag_first = false : info_tmp.time = time_tmp + time_rand * 10, time_tmp += time_rand * 10;
		info_tmp.info_speed = channel_speed * 4;
		info_tmp.info_num = info_rand;
		info_produce.push_back(info_tmp);
	}
}

void STA::setChannel(int channel)
{
	channel_space = channel;
}
//比例公平的优先级
double STA::getPrivity_f1(int timenow)
{
	if (info_produce.front().time > (timenow - 1) * 10) 
		return 0.0;
	double privity = 0.0;
	if (info_dispached.empty()) {//如果没有被调度过，以当前速度为优先级
		privity = info_produce.front().info_speed;
	} 
	else {//发生过调度计算当前速度除以平均作为优先级
		int sum_info = 0;
		double speed_now = info_produce.front().info_speed;
		double sum_time = 0.0;
		for (int j = 0; j < info_dispached.size(); ++j) {
			sum_info += info_dispached[j].info_num;
			sum_time += info_dispached[j].time;
		}
		privity = speed_now * 1000.0 / (sum_info * 1.0 / sum_time);
	}
	return privity;
}

double STA::getPrivity_f2(int timenow)
{
	if (info_produce.front().time > (timenow - 1) * 10)
		return 0.0;
	double privity = info_produce.front().info_speed;
	return privity;
}

void STA::getDispach(double &timeleft)
{
	//先调度1ms
	Info info_tmp;
	Info dispached;
	info_tmp = info_produce.front();
	info_produce.pop_front();
	dispached.info_speed = info_tmp.info_speed;
	int num_trans = (int)info_tmp.info_speed * 1000000.0 * 0.001 / 8.0 / 512.0;//计算1ms以当前速度能传输的数据量为多大
	
	if (num_trans == info_tmp.info_num) {
		std::cout << "一次传输刚好完成" << std::endl;
		dispached.time = 1.0;
		dispached.info_num = num_trans;
	}
	else if (num_trans > info_tmp.info_num) {
		std::cout << "传输能力大于需求" << std::endl;
		double time = info_tmp.info_num * 8.0 * 512.0 * 1000 / 1000000.0 / info_tmp.info_speed;//保存为ms
		dispached.time = time;
		dispached.info_num = info_tmp.info_num;
	}
	else {
		std::cout << "1ms 不够用";
		if (timeleft > 0) {//高优先级剩余时间给后面的STA分配
			std::cout << "timeleft 剩余 " << timeleft;
			int num_trans_now = (int)info_tmp.info_speed * 1000000.0 * (timeleft + 1) / 1000.0 / 8.0 / 512.0;//计算剩余总时间以当前速度能传输的数据量为多大
			
			if (num_trans_now == info_tmp.info_num) {
				std::cout << " timeleft刚好完成" << std::endl;
				dispached.time = timeleft + 1.0;
				dispached.info_num = num_trans;
			}
			else if (num_trans_now > info_tmp.info_num) {
				std::cout << " timeleft 用不完" << std::endl;
				double time = info_tmp.info_num * 8.0 * 512.0 * 1000.0 / 1000000.0 / info_tmp.info_speed;//保存为ms
				dispached.time = time;
				dispached.info_num = info_tmp.info_num;
				timeleft -= time;
			}
			else {
				std::cout << "timeleft 不够用-" << timeleft << std::endl;
				info_tmp.info_num = info_tmp.info_num - num_trans_now;//剩下的信息量
				dispached.info_num = num_trans_now;
				dispached.time = timeleft + 1;
				//更新缓冲区队首的信息
				std::uniform_int_distribution<int> random_un(1, 8);//随机一个速度值
				channel_speed = speed[random_un(random) - 1] * (channel_space / 5.0);
				info_tmp.info_speed = channel_speed * 4;
				info_produce.push_front(info_tmp);
				timeleft = 0.0;
			}
		}
		else {//高优先级将时间用完
			std::cout << "timeleft = 0" << std::endl;
			info_tmp.info_num = info_tmp.info_num - num_trans;//剩下的信息量
			dispached.info_num = num_trans;
			dispached.time = 1.0;
			//更新缓冲区队首的信息
			std::uniform_int_distribution<int> random_un(1, 8);//随机一个速度值
			channel_speed = speed[random_un(random) - 1] * (channel_space / 5.0);
			info_tmp.info_speed = channel_speed * 4;
			info_produce.push_front(info_tmp);
		}
	}
	info_dispached.push_back(dispached);
}