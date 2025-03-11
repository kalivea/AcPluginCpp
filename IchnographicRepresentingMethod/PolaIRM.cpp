#include "stdafx.h"
#include "PolaIRM.h"

void PolaIRM::setBeamInfo(int beam_sn, int beam_segment_num, double beam_b, double beam_h)
{
	beam_sn_ = beam_sn;
	beam_segment_num_ = beam_segment_num;
	beam_b_ = beam_b;
	beam_h_ = beam_h;
}

void PolaIRM::setMainReinforcementInfo(int top_main_reinforcement_num, int top_main_reinforcement_d, int bottom_main_reinforcement_num, int bottom_main_reinforcement_d)
{
	top_main_reinforcement_num_ = top_main_reinforcement_num;
	top_main_reinforcement_d_ = top_main_reinforcement_d;
	bottom_main_reinforcement_num_ = bottom_main_reinforcement_num;
	bottom_main_reinforcement_d_ = bottom_main_reinforcement_d;
}

void PolaIRM::setStirrupReinforcementInfo(int stirrup_reinforcement_d, int stirrup_reinforcement_s, int stirrp_limb_num)
{
	stirrup_reinforcement_d_ = stirrup_reinforcement_d;
	stirrup_reinforcement_s_ = stirrup_reinforcement_s;
	stirrp_limb_num_ = stirrp_limb_num;
}

void PolaIRM::setSideReinforcementInfo(int side_reinforcement_num, int side_reinforcement_d)
{
	side_reinforcement_num_ = side_reinforcement_num;
	side_reinforcement_d_ = side_reinforcement_d;
}

void PolaIRM::setColumnEndAdditionReinforcementInfo(int column_end_addition_reinforcement_num, int column_end_addition_reinforcement_d)
{
	column_end_addition_reinforcement_num_ = column_end_addition_reinforcement_num;
	column_end_addition_reinforcement_d_ = column_end_addition_reinforcement_d;
}

void PolaIRM::setBeamMidAdditionReinforcementInfo(int beam_mid_addition_reinforcement_num, int beam_mid_addition_reinforcement_d)
{
	beam_mid_addition_reinforcement_num_ = beam_mid_addition_reinforcement_num;
	beam_mid_addition_reinforcement_d_ = beam_mid_addition_reinforcement_d;
}

void PolaIRM::getBeamInfo(int& beam_sn, int& beam_segment_num, double& beam_b, double& beam_h)
{
	beam_sn = beam_sn_;
	beam_segment_num = beam_segment_num_;
	beam_b = beam_b_;
	beam_h = beam_h_;
}

void PolaIRM::getMainReinforcementInfo(int& top_main_reinforcement_num, int& top_main_reinforcement_d, int& bottom_main_reinforcement_num, int& bottom_main_reinforcement_d)
{
	top_main_reinforcement_num = top_main_reinforcement_num_;
	top_main_reinforcement_d = top_main_reinforcement_d_;
	bottom_main_reinforcement_num = bottom_main_reinforcement_num_;
	bottom_main_reinforcement_d = bottom_main_reinforcement_d_;
}

void PolaIRM::getStirrupReinforcementInfo(int& stirrup_reinforcement_d, int& stirrup_reinforcement_s, int& stirrp_limb_num)
{
	stirrup_reinforcement_d = stirrup_reinforcement_d_;
	stirrup_reinforcement_s = stirrup_reinforcement_s_;
	stirrp_limb_num = stirrp_limb_num_;
}

void PolaIRM::getSideReinforcementInfo(int& side_reinforcement_num, int& side_reinforcement_d)
{
	side_reinforcement_num = side_reinforcement_num_;
	side_reinforcement_d = side_reinforcement_d_;
}

void PolaIRM::getColumnEndAdditionReinforcementInfo(int& column_end_addition_reinforcement_num, int& column_end_addition_reinforcement_d)
{
	column_end_addition_reinforcement_num = column_end_addition_reinforcement_num_;
	column_end_addition_reinforcement_d = column_end_addition_reinforcement_d_;
}

void PolaIRM::getBeamMidAdditionReinforcementInfo(int& beam_mid_addition_reinforcement_num, int& beam_mid_addition_reinforcement_d)
{
	beam_mid_addition_reinforcement_num = beam_mid_addition_reinforcement_num_;
	beam_mid_addition_reinforcement_d = beam_mid_addition_reinforcement_d_;
}