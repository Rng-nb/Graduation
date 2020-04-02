#pragma once
#include <queue>
#include <vector>
#include <random>
class STA
{	
	public:
		STA();
		~STA();
		void produce_info();
		void set_channel(int channel);
		double get_privity();//获得该STA当前的优先级
		void get_dispach();//该STA被调度

		std::queue<std::pair<double, std::pair<double, int> > > info;//存放产生的信息
		std::vector<std::pair<double, std::pair<double, int> > > info_did;//存放调度过的信息
	private:
		double lambda_ex;//指数分布参数
		double lambda_po;//泊松分布参数
		double channel_speed;
		int channel_space;
		std::random_device rd;//借此来产生一个随机种子，不然每个随机数队列时相同
		std::default_random_engine random{ rd() };//随机数引擎
		
};

