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
		double get_privity();//��ø�STA��ǰ�����ȼ�
		void get_dispach();//��STA������

		std::queue<std::pair<double, std::pair<double, int> > > info;//��Ų�������Ϣ
		std::vector<std::pair<double, std::pair<double, int> > > info_did;//��ŵ��ȹ�����Ϣ
	private:
		double lambda_ex;//ָ���ֲ�����
		double lambda_po;//���ɷֲ�����
		double channel_speed;
		int channel_space;
		std::random_device rd;//���������һ��������ӣ���Ȼÿ�����������ʱ��ͬ
		std::default_random_engine random{ rd() };//���������
		
};

