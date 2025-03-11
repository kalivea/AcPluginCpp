#pragma once
#include "StdAfx.h"
class POLADLL_IRM_API PolaIRM
{
private:
	int beam_sn_;
	int beam_segment_num_;

	double beam_b_;
	double beam_h_;

	int top_main_reinforcement_num_;
	int top_main_reinforcement_d_;

	int bottom_main_reinforcement_num_;
	int bottom_main_reinforcement_d_;

	int stirrup_reinforcement_d_;
	int stirrup_reinforcement_s_;
	int stirrp_limb_num_;

	int side_reinforcement_num_;
	int side_reinforcement_d_;

	int column_end_addition_reinforcement_num_;
	int column_end_addition_reinforcement_d_;

	int beam_mid_addition_reinforcement_num_;
	int beam_mid_addition_reinforcement_d_;
public:
	PolaIRM() {};
	~PolaIRM() {};

	void setBeamInfo(int beam_sn, int beam_segment_num, double beam_b, double beam_h);
	void setMainReinforcementInfo(int top_main_reinforcement_num, int top_main_reinforcement_d, int bottom_main_reinforcement_num, int bottom_main_reinforcement_d);
	void setStirrupReinforcementInfo(int stirrup_reinforcement_d, int stirrup_reinforcement_s, int stirrp_limb_num);
	void setSideReinforcementInfo(int side_reinforcement_num, int side_reinforcement_d);
	void setColumnEndAdditionReinforcementInfo(int column_end_addition_reinforcement_num, int column_end_addition_reinforcement_d);
	void setBeamMidAdditionReinforcementInfo(int beam_mid_addition_reinforcement_num, int beam_mid_addition_reinforcement_d);

	void getBeamInfo(int& beam_sn, int& beam_segment_num, double& beam_b, double& beam_h);
	void getMainReinforcementInfo(int& top_main_reinforcement_num, int& top_main_reinforcement_d, int& bottom_main_reinforcement_num, int& bottom_main_reinforcement_d);
	void getStirrupReinforcementInfo(int& stirrup_reinforcement_d, int& stirrup_reinforcement_s, int& stirrp_limb_num);
	void getSideReinforcementInfo(int& side_reinforcement_num, int& side_reinforcement_d);
	void getColumnEndAdditionReinforcementInfo(int& column_end_addition_reinforcement_num, int& column_end_addition_reinforcement_d);
	void getBeamMidAdditionReinforcementInfo(int& beam_mid_addition_reinforcement_num, int& beam_mid_addition_reinforcement_d);

};

