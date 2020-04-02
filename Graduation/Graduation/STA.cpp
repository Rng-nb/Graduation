#include "STA.h"
#include <iostream>
STA::STA()
{
	std::uniform_real_distribution<double> random_un(1,10);//1-10���ȷֲ������double
	lambda_ex = random_un(random);
	lambda_po = random_un(random);
	channel_space = 5;
	info_did.clear();
}

STA::~STA()
{
}

void STA::produce_info()
{
	int i = 150;
	bool flag = true;
	while (i--) {
		std::exponential_distribution<double> random_ex(lambda_ex);//ʱ�������ϲ���Ϊlambda1��ָ���ֲ�
		double gap_rand = random_ex(random);
		double lambda = lambda_po * gap_rand * 1000;
		std::poisson_distribution<int> random_po(lambda);//ʱ����t�ڵ����������ϲ���Ϊlambda2*t�Ĳ��ɷֲ�
		double info_rand = random_po(random);
		double speed[8] = { 1.5,2.25,3,4.5,6,9,12,13.5 };
		std::uniform_int_distribution<int> random_un(1, 8);
		int rand_mcs = random_un(random);
		channel_speed = speed[rand_mcs - 1] * (channel_space / 5.0);
		info.push(std::pair<double,std::pair<double,int> >(channel_speed,std::pair<double,int>(gap_rand,info_rand)));
	}
}

void STA::set_channel(int channel)
{
	channel_space = channel;
}

double STA::get_privity()
{
	double privity = 0.0;
	if (info_did.empty()) {//���û�б����ȹ�
		privity = info.front().first;
	} 
	else {//���������ȼ��㵱ǰ�ٶȳ���ƽ����Ϊ���ȼ�
		int sum_info = 0;
		double speed_now = info.front().first;
		double sum_time = 0.0;
		for (int j = 0; j < info_did.size(); ++j) {
			sum_info += info_did[j].second.second;
			sum_time += (double)(info_did[j].second.second * 512.0 * 8.0 / 1000000.0) / info_did[j].first;
		}
		privity = speed_now / (sum_info / sum_time);
	}
	return privity;
}

void STA::get_dispach()//һ�ε���Ϊ10ms
{
	std::pair<double, std::pair<double, int> > info_tmp;
	info_tmp = info.front();
	info_did.push_back(info_tmp);
	info.pop();
}
