#pragma once
#include <deque>
#include <vector>
#include <random>
typedef struct {
	double time;
	double info_speed;
	int info_num;
}Info;

class STA
{	
	public:
		STA();
		~STA();
		void produce_info();
		void set_channel(int channel);
		double get_privity_f1();//获得该STA当前的优先级
		void get_dispach_f1();//该STA被第一种调度
		void get_dispach_f2();//该STA被第二种调度

		std::deque<std::pair<double, int> > info;//存放产生的信息
		std::vector<std::pair<int, std::pair<double, int> > > info_did;//存放调度过的信息
	private:
		double lambda_ex;//指数分布参数
		double lambda_po;//泊松分布参数
		double channel_speed;
		int channel_space;
		std::random_device rd;//借此来产生一个随机种子，不然每个随机数队列时相同
		std::default_random_engine random{ rd() };//随机数引擎
		
};

