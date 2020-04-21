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
		double get_privity_f1();//��ø�STA��ǰ�����ȼ�
		void get_dispach_f1();//��STA����һ�ֵ���
		void get_dispach_f2();//��STA���ڶ��ֵ���

		std::deque<std::pair<double, int> > info;//��Ų�������Ϣ
		std::vector<std::pair<int, std::pair<double, int> > > info_did;//��ŵ��ȹ�����Ϣ
	private:
		double lambda_ex;//ָ���ֲ�����
		double lambda_po;//���ɷֲ�����
		double channel_speed;
		int channel_space;
		std::random_device rd;//���������һ��������ӣ���Ȼÿ�����������ʱ��ͬ
		std::default_random_engine random{ rd() };//���������
		
};

