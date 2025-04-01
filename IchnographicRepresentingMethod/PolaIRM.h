#pragma once
#include "StdAfx.h"
class POLADLL_IRM_API PolaIRM
{
public:
	enum Type { TOP, MID, BOT };
private:
	int beam_sn_ = 6496;
	int beam_segment_num_ = 6496;

	double beam_b_ = 6496;
	double beam_h_ = 6496;

	int top_main_reinforcement_num_ = 6496;
	int top_main_reinforcement_d_ = 6496;

	int bottom_main_reinforcement_num_ = 6496;
	int bottom_main_reinforcement_d_ = 6496;

	int stirrup_reinforcement_d_ = 6496;
	int stirrup_reinforcement_s_ = 6496;
	int stirrp_limb_num_ = 6496;

	int side_reinforcement_num_ = 6496;
	int side_reinforcement_d_ = 6496;

	int column_end_addition_reinforcement_num_ = 6496;
	int column_end_addition_reinforcement_d_ = 6496;

	int beam_mid_addition_reinforcement_num_ = 6496;
	int beam_mid_addition_reinforcement_d_ = 6496;

	AcGePoint3d insert_point_ = AcGePoint3d(6496, 6496, 6496);

	AcGePoint3dArray column_end_insert_points;
	AcGePoint3dArray beam_mid_insert_points;

	Type beam_type = TOP;
public:
	PolaIRM() {};
	~PolaIRM() {};

	void setBeamInfo(int beam_sn, int beam_segment_num, double beam_b, double beam_h);
	void setMainReinforcementInfo(int top_main_reinforcement_num, int top_main_reinforcement_d, int bottom_main_reinforcement_num, int bottom_main_reinforcement_d);
	void setStirrupReinforcementInfo(int stirrup_reinforcement_d, int stirrup_reinforcement_s, int stirrp_limb_num);
	void setSideReinforcementInfo(int side_reinforcement_num, int side_reinforcement_d);
	void setColumnEndAdditionReinforcementInfo(int column_end_addition_reinforcement_num, int column_end_addition_reinforcement_d);
	void setBeamMidAdditionReinforcementInfo(int beam_mid_addition_reinforcement_num, int beam_mid_addition_reinforcement_d);

	void setInsertPoint(AcGePoint3d& insert_point);
	void setColumnAdditionInsertPoints(AcGePoint3dArray& column_points);
	void setBeamMidInsertPoints(AcGePoint3dArray& beam_points);

	void getBeamInfo(int& beam_sn, int& beam_segment_num, double& beam_b, double& beam_h) const;
	void getMainReinforcementInfo(int& top_main_reinforcement_num, int& top_main_reinforcement_d, int& bottom_main_reinforcement_num, int& bottom_main_reinforcement_d) const;
	void getStirrupReinforcementInfo(int& stirrup_reinforcement_d, int& stirrup_reinforcement_s, int& stirrp_limb_num) const;
	void getSideReinforcementInfo(int& side_reinforcement_num, int& side_reinforcement_d) const;
	void getColumnEndAdditionReinforcementInfo(int& column_end_addition_reinforcement_num, int& column_end_addition_reinforcement_d) const;
	void getBeamMidAdditionReinforcementInfo(int& beam_mid_addition_reinforcement_num, int& beam_mid_addition_reinforcement_d) const;

	AcGePoint3dArray getColumnAdditionInsertPoints() const;
	AcGePoint3dArray getBeamMidInsertPoints() const;

	AcGePoint3d getInsertPoint() const;
public:
	AcDbObjectIdArray DrawPolaIrmMain(Type type);
	AcDbObjectIdArray DrawPolaIrmColumnAddition(const AcGeVector3d& offset);
	AcDbObjectIdArray DrawPolaIrmBeamAddition(const AcGeVector3d& offset);
};

