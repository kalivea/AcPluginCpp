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

void PolaIRM::setInsertPoint(AcGePoint3d insert_point)
{
	insert_point_ = insert_point;
}

void PolaIRM::getBeamInfo(int& beam_sn, int& beam_segment_num, double& beam_b, double& beam_h) const
{
	beam_sn = beam_sn_;
	beam_segment_num = beam_segment_num_;
	beam_b = beam_b_;
	beam_h = beam_h_;
}

void PolaIRM::getMainReinforcementInfo(int& top_main_reinforcement_num, int& top_main_reinforcement_d, int& bottom_main_reinforcement_num, int& bottom_main_reinforcement_d) const
{
	top_main_reinforcement_num = top_main_reinforcement_num_;
	top_main_reinforcement_d = top_main_reinforcement_d_;
	bottom_main_reinforcement_num = bottom_main_reinforcement_num_;
	bottom_main_reinforcement_d = bottom_main_reinforcement_d_;
}

void PolaIRM::getStirrupReinforcementInfo(int& stirrup_reinforcement_d, int& stirrup_reinforcement_s, int& stirrp_limb_num) const
{
	stirrup_reinforcement_d = stirrup_reinforcement_d_;
	stirrup_reinforcement_s = stirrup_reinforcement_s_;
	stirrp_limb_num = stirrp_limb_num_;
}

void PolaIRM::getSideReinforcementInfo(int& side_reinforcement_num, int& side_reinforcement_d) const
{
	side_reinforcement_num = side_reinforcement_num_;
	side_reinforcement_d = side_reinforcement_d_;
}

void PolaIRM::getColumnEndAdditionReinforcementInfo(int& column_end_addition_reinforcement_num, int& column_end_addition_reinforcement_d) const
{
	column_end_addition_reinforcement_num = column_end_addition_reinforcement_num_;
	column_end_addition_reinforcement_d = column_end_addition_reinforcement_d_;
}

void PolaIRM::getBeamMidAdditionReinforcementInfo(int& beam_mid_addition_reinforcement_num, int& beam_mid_addition_reinforcement_d) const
{
	beam_mid_addition_reinforcement_num = beam_mid_addition_reinforcement_num_;
	beam_mid_addition_reinforcement_d = beam_mid_addition_reinforcement_d_;
}

AcGePoint3d PolaIRM::getInsertPoint() const
{
	return insert_point_;
}

AcDbObjectIdArray PolaIRM::DrawPolaIRM()
{
	insert_point_ = AcGePoint3d(0, 0, 0);
	AcDbObjectIdArray IRM_ids;
	std::vector<int> reinforcement_per_row = BasicTools::CalculateReinforcement(static_cast<int>(beam_b_), top_main_reinforcement_d_, top_main_reinforcement_num_, 45, stirrup_reinforcement_d_);
	AcGePoint3d end_point = insert_point_ + AcGeVector3d(0, beam_b_ / 2.0 + 500 + 2000, 0);
	IRM_ids.append(DrawEntity::DrawLine(insert_point_, end_point));			// IRM line
	// ----------------------------------------Beam info text------------------------------
	std::wstring beam_info;
	std::wstringstream beam_info_stream;
	beam_info_stream <<
		_T("KL") << beam_sn_ << _T(" ") << _T("(") << beam_segment_num_ << _T(")") << static_cast<int>(beam_b_) << _T("¡Á") << static_cast<int>(beam_h_);
	beam_info = beam_info_stream.str();
	IRM_ids.append(DrawEntity::AddText(end_point + AcGeVector3d(150, -400, 0), beam_info.c_str(), StyleTools::GetTextStyleId(_T("IRM_default")), 350));
	// ----------------------------------------stirrup info text------------------------------
	std::wstring stirrup_info;
	std::wstringstream stirrup_info_stream;
	stirrup_info_stream <<
		_T("%%132") << stirrup_reinforcement_d_ << _T("@") << stirrup_reinforcement_s_ << _T("/") <<
		stirrup_reinforcement_s_ * 2 << _T("(") << stirrp_limb_num_ << _T(")");
	stirrup_info = stirrup_info_stream.str();
	IRM_ids.append(DrawEntity::AddText(end_point + AcGeVector3d(150, -800, 0), stirrup_info.c_str(), StyleTools::GetTextStyleId(_T("IRM_default")), 350));
	// ----------------------------------------main reinforcement info text------------------------------
	std::wstring main_reinforcement_info;
	std::wstringstream main_reinforcement_info_stream;
	main_reinforcement_info_stream <<
		top_main_reinforcement_num_ << _T("%%132") << top_main_reinforcement_d_ << _T(" ");
	for (size_t i = 0; i < reinforcement_per_row.size(); ++i)
	{
		main_reinforcement_info_stream << reinforcement_per_row.at(i);
		if (i != reinforcement_per_row.size() - 1)
		{
			main_reinforcement_info_stream << "/";
		}
	}
	main_reinforcement_info_stream << _T(" ; ") << bottom_main_reinforcement_num_ << _T("%%132") << bottom_main_reinforcement_d_ << _T(" ");
	for (size_t i = reinforcement_per_row.size(); i > 0; --i)
	{
		size_t index = i - 1;
		main_reinforcement_info_stream << reinforcement_per_row.at(index);
		if (index != 0)
		{
			main_reinforcement_info_stream << "/";
		}
	}
	main_reinforcement_info = main_reinforcement_info_stream.str();
	IRM_ids.append(DrawEntity::AddText(end_point + AcGeVector3d(150, -1200, 0), main_reinforcement_info.c_str(), StyleTools::GetTextStyleId(_T("IRM_default")), 350));
	// ----------------------------------------side reinforcement info text------------------------------
	std::wstring side_reinforcement_info;
	std::wstringstream side_reinforcement_info_stream;
	side_reinforcement_info_stream <<
		_T("N") << side_reinforcement_num_ << _T("%%132") << side_reinforcement_d_;
	side_reinforcement_info = side_reinforcement_info_stream.str();
	IRM_ids.append(DrawEntity::AddText(end_point + AcGeVector3d(150, -1600, 0), side_reinforcement_info.c_str(), StyleTools::GetTextStyleId(_T("IRM_default")), 350));

	return IRM_ids;
}
