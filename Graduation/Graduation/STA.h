#pragma once
#include <deque>
#include <vector>
#include <random>
#include <fstream>
#include <iostream>
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
		void produceInfo();
		void setChannel(int channel);
		void getDispach_f1();//该STA被第一种调度
		void getDispach(double &timeleft);//该STA被第二种调度
		double getPrivity_f1(int timenow);//获得该STA当前的优先级,比例公平
		double getPrivity_f2(int timenow);//最大吞吐量

		std::deque<Info> info_produce;//存放产生的信息
		std::vector<Info> info_dispached;//存放调度过的信息
	
	private:
		double speed[8] = { 1.5, 2.25, 3, 4.5, 6, 9, 12, 13.5 };//速度信息
		double lambda_ex_time;//时间符合指数分布
		double lambda_po_info;//数据符合泊松分布
		double channel_speed;//速度
		int channel_space;

		std::random_device rd;//借此来产生一个随机种子，不然每个随机数队列时相同
		std::default_random_engine random{ rd() };//随机数引擎
};

