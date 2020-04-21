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
	while (i--) {
		std::exponential_distribution<double> random_ex(lambda_ex);//ʱ�������ϲ���Ϊlambda1��ָ���ֲ�
		double gap_rand = random_ex(random);
		double lambda = lambda_po * gap_rand * 1000;
		std::poisson_distribution<int> random_po(lambda);//ʱ����t�ڵ����������ϲ���Ϊlambda2*t�Ĳ��ɷֲ�
		double info_rand = random_po(random) / 50;
		double speed[8] = { 1.5,2.25,3,4.5,6,9,12,13.5 };
		std::uniform_int_distribution<int> random_un(1, 8);
		int rand_mcs = random_un(random);
		channel_speed = speed[rand_mcs - 1] * (channel_space / 5.0);
		info.push_back(std::pair<double,int>(channel_speed,info_rand));
		
	}
}

void STA::set_channel(int channel)
{
	channel_space = channel;
}

double STA::get_privity_f1()
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

void STA::get_dispach_f1()//һ����������10ms,���䴫��ʱ��Ϊ9ms��1msΪ����ʱ϶
{
	std::pair<double,int> info_front_tmp;
	info_front_tmp = info.front();
	//����9ms�Ե�ǰ�ٶ��ܴ����������Ϊ���
	int info_num = (int)info_front_tmp.first * 1000000.0 * 0.009 / 8.0 / 512.0;
	
	if (info_num == info_front_tmp.second) {
		std::cout << "һ�δ���պ����"<< std::endl;
		info_did.push_back(std::pair<int,std::pair<int, int> >(9,info_front_tmp));
		info.pop_front();
	}
	else if (info_num > info_front_tmp.second) {
		std::cout << "����������������" << std::endl;
		double time = info_front_tmp.second * 8.0 * 512.0 * 1000 / 1000000.0 / info_front_tmp.first;
		int cost = time + 1;
		info_did.push_back(std::pair<int, std::pair<double, int> >(cost + 1, info_front_tmp));
		info.pop_front();
	}
	else {
		std::cout << "һ�δ��䲻�꣬�˴δ���������" << info_num << "ʣ�ࣺ" << info_front_tmp.second - info_num << std::endl;
		double speed[8] = { 1.5,2.25,3,4.5,6,9,12,13.5 };
		std::uniform_int_distribution<int> random_un(1, 8);
		int rand_mcs = random_un(random);
		//ʣ�µ���Ϣ��
		int info_left = info_front_tmp.second - info_num;
		info_did.push_back(std::pair<int, std::pair<double,int> > (9,std::pair<double, int>(info_front_tmp.first,info_num)));
		info.pop_front();
		//���»��������׵���Ϣ
		channel_speed = speed[rand_mcs - 1] * (channel_space / 5.0);
		info.push_front(std::pair<double, int> (channel_speed, info_left));
	}
	
}

void STA::get_dispach_f2()
{
}
