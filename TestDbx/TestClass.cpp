#include "StdAfx.h"
#include "TestClass.h"
#include "PolaReactor.h"
#include "PolaIRM.h"
#include "EditDwg.h"
#include "PillarTools.h"
#include "DwgFileInfo.h"

void TestClass::TestClassInit()
{
	acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestClass"), _T("TestClass"), ACRX_CMD_MODAL, Test);
	//acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestDLQ"), _T("TestDLQ"), ACRX_CMD_MODAL, DLQ);
	//acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestYHZ"), _T("TestYHZ"), ACRX_CMD_MODAL, YHZ);
	//acedRegCmds->addCommand(_T("tmpGroupName"), _T("TestGZC"), _T("TestGZC"), ACRX_CMD_MODAL, GZC);
}
void TestClass::TestClassUnload()
{
	acedRegCmds->removeGroup(_T("tmpGroupName"));
}

void TestClass::Test()
{
	/*if (PillarTools::SaveAllPillarInfoToFile(_T("C:\\Users\\Polaris\\Documents\\arxini\\AllPillarInfo.bin")))
		acutPrintf(_T("success\n"));
	else
		acutPrintf(_T("error\n"));*/
	/*TCHAR szFileName[MAX_PATH] = { 0 };
	TCHAR szFilePath[MAX_PATH] = { 0 };

	if (DwgFileInfo::GetDwgFileInfo(szFileName, MAX_PATH, szFilePath, MAX_PATH))
	{
		CString path = _T("C:\\Users\\Polaris\\Documents\\arxini");
		CString full_path;
		full_path.Format(_T("%s\\%s.bin"), path.GetString(), szFileName);
		if (PillarTools::SaveAllPillarInfoToFile(full_path.GetString()))
			acutPrintf(_T("success\n"));
		else
			acutPrintf(_T("error\n"));
	}
	else
		acutPrintf(_T("ERROR"));*/
	StyleTools::LoadLineType(_T("CENTER"), _T("acad.lin"));
	StyleTools::LoadLineType(_T("DASHED"), _T("acad.lin"));
	StyleTools::CreateLayerStyle(_T("POLA_PILLAR_STRUCTURE"), 2, _T("CONTINUOUS"));
	StyleTools::CreateLayerStyle(_T("POLA_PILLAR_MARK"), 3, _T("CONTINUOUS"));

	CPolaCustomPillar* pillar = new CPolaCustomPillar;
	PillarTools::LoadPillarInfoFromFileBySn(_T("C:\\Users\\Polaris\\Documents\\arxini\\Drawing1.dwg.bin"), _T("呀1"), pillar);

	AddToModelSpace::AddEntityToModelSpace(pillar);
}
//
//void TestClass::Test()
//{
//	TCHAR* a = _T("DAfsdfa");
//	std::wstring aa;
//	std::wstringstream aaa;
//	aaa << a;
//	aa = aaa.str();
//	acutPrintf(aa.c_str());
//}
//
//void TestClass::DLQ()
//{
//	OPENFILENAME open_file_name;
//	TCHAR file_path[MAX_PATH] = { 0 };
//	TCHAR* default_path = _T("C:\\Users\\Polaris\\Desktop\\7-图纸模块\\1地下结构专业\\1各工法节点大样图\\1-明挖\\2-地下连续墙");
//	ZeroMemory(&open_file_name, sizeof(open_file_name));
//	open_file_name.lStructSize = sizeof(open_file_name);
//	open_file_name.hwndOwner = NULL;
//	open_file_name.lpstrFile = file_path;
//	open_file_name.nMaxFile = sizeof(file_path) / sizeof(*file_path);
//	open_file_name.lpstrFilter = _T("All Files\0*.*\0Text Files\0*.txt\0");
//	open_file_name.nFilterIndex = 1;
//	open_file_name.lpstrInitialDir = default_path;
//	open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
//
//	if (GetOpenFileName(&open_file_name))
//	{
//		std::basic_string<TCHAR> escaped_path = BasicTools::EscapeBackslashes(file_path);
//
//		std::basic_string<TCHAR> path_str(file_path);
//		size_t last_slash = path_str.find_last_of(_T("\\/"));
//		size_t last_dot = path_str.find_last_of(_T("."));
//
//		std::basic_string<TCHAR> block_name;
//		if (last_slash != std::basic_string<TCHAR>::npos)
//		{
//			if (last_dot != std::basic_string<TCHAR>::npos && last_dot > last_slash)
//				block_name = path_str.substr(last_slash + 1, last_dot - last_slash - 1);
//			else
//				block_name = path_str.substr(last_slash + 1);
//		}
//		else
//		{
//			if (last_dot != std::basic_string<TCHAR>::npos)
//				block_name = path_str.substr(0, last_dot);
//			else
//				block_name = path_str;
//		}
//		AcGePoint3d point;
//		SelectEntitys::PickPoint(_T("选择插入点"), point);
//		EditBlock::InsertBlockRef(EditDwg::InsertDwgAsBlockDef(escaped_path.c_str(), block_name.c_str(), true), point, 1, 0);
//	}
//}
//
//void TestClass::YHZ()
//{
//	OPENFILENAME open_file_name;
//	TCHAR file_path[MAX_PATH] = { 0 };
//	TCHAR* default_path = _T("C:\\Users\\Polaris\\Desktop\\7-图纸模块\\1地下结构专业\\1各工法节点大样图\\1-明挖\\3-咬合桩");
//	ZeroMemory(&open_file_name, sizeof(open_file_name));
//	open_file_name.lStructSize = sizeof(open_file_name);
//	open_file_name.hwndOwner = NULL;
//	open_file_name.lpstrFile = file_path;
//	open_file_name.nMaxFile = sizeof(file_path) / sizeof(*file_path);
//	open_file_name.lpstrFilter = _T("All Files\0*.*\0Text Files\0*.txt\0");
//	open_file_name.nFilterIndex = 1;
//	open_file_name.lpstrInitialDir = default_path;
//	open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
//
//	if (GetOpenFileName(&open_file_name))
//	{
//		std::basic_string<TCHAR> escaped_path = BasicTools::EscapeBackslashes(file_path);
//
//		std::basic_string<TCHAR> path_str(file_path);
//		size_t last_slash = path_str.find_last_of(_T("\\/"));
//		size_t last_dot = path_str.find_last_of(_T("."));
//
//		std::basic_string<TCHAR> block_name;
//		if (last_slash != std::basic_string<TCHAR>::npos)
//		{
//			if (last_dot != std::basic_string<TCHAR>::npos && last_dot > last_slash)
//				block_name = path_str.substr(last_slash + 1, last_dot - last_slash - 1);
//			else
//				block_name = path_str.substr(last_slash + 1);
//		}
//		else
//		{
//			if (last_dot != std::basic_string<TCHAR>::npos)
//				block_name = path_str.substr(0, last_dot);
//			else
//				block_name = path_str;
//		}
//		AcGePoint3d point;
//		SelectEntitys::PickPoint(_T("选择插入点"), point);
//		EditBlock::InsertBlockRef(EditDwg::InsertDwgAsBlockDef(escaped_path.c_str(), block_name.c_str(), true), point, 1, 0);
//	}
//}
//
//void TestClass::GZC()
//{
//	OPENFILENAME open_file_name;
//	TCHAR file_path[MAX_PATH] = { 0 };
//	TCHAR* default_path = _T("C:\\Users\\Polaris\\Desktop\\7-图纸模块\\1地下结构专业\\1各工法节点大样图\\1-明挖\\8-钢支撑及钢围檩");
//	ZeroMemory(&open_file_name, sizeof(open_file_name));
//	open_file_name.lStructSize = sizeof(open_file_name);
//	open_file_name.hwndOwner = NULL;
//	open_file_name.lpstrFile = file_path;
//	open_file_name.nMaxFile = sizeof(file_path) / sizeof(*file_path);
//	open_file_name.lpstrFilter = _T("All Files\0*.*\0Text Files\0*.txt\0");
//	open_file_name.nFilterIndex = 1;
//	open_file_name.lpstrInitialDir = default_path;
//	open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
//
//	if (GetOpenFileName(&open_file_name))
//	{
//		std::basic_string<TCHAR> escaped_path = BasicTools::EscapeBackslashes(file_path);
//
//		std::basic_string<TCHAR> path_str(file_path);
//		size_t last_slash = path_str.find_last_of(_T("\\/"));
//		size_t last_dot = path_str.find_last_of(_T("."));
//
//		std::basic_string<TCHAR> block_name;
//		if (last_slash != std::basic_string<TCHAR>::npos)
//		{
//			if (last_dot != std::basic_string<TCHAR>::npos && last_dot > last_slash)
//				block_name = path_str.substr(last_slash + 1, last_dot - last_slash - 1);
//			else
//				block_name = path_str.substr(last_slash + 1);
//		}
//		else
//		{
//			if (last_dot != std::basic_string<TCHAR>::npos)
//				block_name = path_str.substr(0, last_dot);
//			else
//				block_name = path_str;
//		}
//		AcGePoint3d point;
//		SelectEntitys::PickPoint(_T("选择插入点"), point);
//		EditBlock::InsertBlockRef(EditDwg::InsertDwgAsBlockDef(escaped_path.c_str(), block_name.c_str(), true), point, 1, 0);
//	}
//}
