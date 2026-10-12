#include "stdafx.h"
#include "PillarTools.h"
#include "SelectEntitys.h"
#include "DwgFileInfo.h"
#include <fstream>
#include <string>
#include <cmath>
#include <memory>
#include "../Common/DbObjectOwner.h"
#include "PillarDwgFields.h"

bool PillarTools::detectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter)
{
	bool found = false;
	const Acad::ErrorStatus status = DetectPillar(point, pillar_id, pillar_diameter, found);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar lookup failed: %d"), static_cast<int>(status));
	return status == Acad::eOk && found;
}

Acad::ErrorStatus PillarTools::DetectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter, bool& found)
{
	pillar_id = AcDbObjectId::kNull;
	pillar_diameter = 0.0;
	found = false;
	if (!std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z)) return Acad::eInvalidInput;
	AcDbObjectIdArray all_pillar_ids;
	Acad::ErrorStatus status = GetAllPillarChecked(all_pillar_ids);
	if (status != Acad::eOk) return status;
	AcDbExtents extents;
	for (const auto& id : all_pillar_ids)
	{
		CPolaCustomPillar* raw = nullptr;
		status = acdbOpenObject(raw, id, AcDb::kForRead);
		if (status != Acad::eOk) return status;
		Pola::OpenedObject<CPolaCustomPillar> pillar(raw);
		status = pillar->getGeomExtents(extents);
		if (status != Acad::eOk) return Pola::Close(pillar, status);
		if (BasicTools::IsPointInRectangle(point, extents))
		{
			double temp_d, temp_h;
			pillar->getDiameter(temp_d, temp_h);
			status = Pola::Close(pillar);
			if (status != Acad::eOk) return status;
			pillar_id = id;
			pillar_diameter = temp_d;
			found = true;
			return Acad::eOk;
		}
		status = Pola::Close(pillar);
		if (status != Acad::eOk) return status;
	}
	pillar_id = AcDbObjectId::kNull;
	pillar_diameter = 0;
	return Acad::eOk;
}

bool PillarTools::detectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter)
{
	bool found = false;
	const Acad::ErrorStatus status = DetectRoundPillar(point, pillar_id, pillar_diameter, found);
	if (status != Acad::eOk) acutPrintf(_T("\nRound pillar lookup failed: %d"), static_cast<int>(status));
	return status == Acad::eOk && found;
}

Acad::ErrorStatus PillarTools::DetectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter, bool& found)
{
	pillar_id = AcDbObjectId::kNull;
	pillar_diameter = 0.0;
	found = false;
	AcDbObjectId pillar;
	double diameter = 0.0;
	bool detected = false;
	Acad::ErrorStatus status = DetectPillar(point, pillar, diameter, detected);
	if (status != Acad::eOk) return status;
	if (detected)
	{
		CPolaCustomPillar* raw = nullptr;
		status = acdbOpenObject(raw, pillar, AcDb::kForRead);
		if (status != Acad::eOk) return status;
		Pola::OpenedObject<CPolaCustomPillar> opened(raw);
		const bool round = opened->getPillarType() == 0;
		status = Pola::Close(opened);
		if (status != Acad::eOk) return status;
		if (round)
		{
			pillar_id = pillar;
			pillar_diameter = diameter;
			found = true;
		}
	}
	return Acad::eOk;
}

bool PillarTools::GetAllPillar(AcDbObjectIdArray& all_pillar_ids)
{
	const Acad::ErrorStatus status = GetAllPillarChecked(all_pillar_ids);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar enumeration failed: %d"), static_cast<int>(status));
	return status == Acad::eOk && !all_pillar_ids.isEmpty();
}

Acad::ErrorStatus PillarTools::GetAllPillarChecked(AcDbObjectIdArray& all_pillar_ids)
{
	return SelectEntitys::GetAllEntitysByTypeChecked(CPolaCustomPillar::desc(), all_pillar_ids);
}

bool PillarTools::SaveAllPillarInfoToFile(const std::wstring& filePath)
{
	const Acad::ErrorStatus status = SaveAllPillarInfoToFileChecked(filePath);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar export failed: %d"), static_cast<int>(status));
	return status == Acad::eOk;
}

Acad::ErrorStatus PillarTools::SaveAllPillarInfoToFileChecked(const std::wstring& filePath)
{
	AcDbObjectIdArray all_pillar_id;
	Acad::ErrorStatus status = GetAllPillarChecked(all_pillar_id);
	if (status != Acad::eOk) return status;
	std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!outFile.is_open())
		return Acad::eFileAccessErr;

	// Write the number of pillars
	Adesk::Int32 pillarCount = all_pillar_id.length();
	outFile.write(reinterpret_cast<const char*>(&pillarCount), sizeof(Adesk::Int32));

	for (const auto& id : all_pillar_id)
	{
		CPolaCustomPillar* raw = nullptr;
		status = acdbOpenObject(raw, id, AcDb::kForRead);
		if (status != Acad::eOk) return status;
		Pola::OpenedObject<CPolaCustomPillar> pillar(raw);
		if (!CPolaCustomPillar::checkValue(pillar.get())) return Pola::Close(pillar, Acad::eInvalidInput);

		AcGePoint3d center_point = pillar->getCenterPoint();
		AcGeVector3d direction_vector = pillar->getDirectionVector();
		double pillar_d, pillar_h;
		pillar->getDiameter(pillar_d, pillar_h);
		bool viewable = pillar->getViewable();
		Adesk::Int32 pillar_property = pillar->getPillarProperty();
		const TCHAR* pillar_serial_number = pillar->getPillarSn();
		Adesk::Int32 pillar_type = pillar->getPillarType();
		AcGePoint3dArray rect_pillar_vertex = pillar->getRectVertex();
		if ((pillar_type == 0 && !rect_pillar_vertex.isEmpty()) || (pillar_type == 1 && rect_pillar_vertex.length() != 4))
			return Pola::Close(pillar, Acad::eInvalidInput);
		Adesk::Int32 concrete_grade = pillar->getConcreteGrade();
		Pola::PillarDwgFields fields;
		fields.type = pillar_type;
		fields.center = center_point;
		fields.direction = direction_vector;
		fields.diameter = pillar_d;
		fields.height = pillar_h;
		fields.visible = viewable;
		fields.property = pillar_property;
		fields.serial = pillar_serial_number;
		fields.vertices = rect_pillar_vertex;
		fields.grade = concrete_grade;
		status = Pola::ValidatePillarFields(fields);
		if (status != Acad::eOk) return Pola::Close(pillar, status);

		// Write center_point_
		outFile.write(reinterpret_cast<const char*>(&center_point.x), sizeof(double));
		outFile.write(reinterpret_cast<const char*>(&center_point.y), sizeof(double));
		outFile.write(reinterpret_cast<const char*>(&center_point.z), sizeof(double));

		// Write direction_vector_
		outFile.write(reinterpret_cast<const char*>(&direction_vector.x), sizeof(double));
		outFile.write(reinterpret_cast<const char*>(&direction_vector.y), sizeof(double));
		outFile.write(reinterpret_cast<const char*>(&direction_vector.z), sizeof(double));

		// Write pillar_d_ and pillar_h_
		outFile.write(reinterpret_cast<const char*>(&pillar_d), sizeof(double));
		outFile.write(reinterpret_cast<const char*>(&pillar_h), sizeof(double));

		// Write viewable_
		outFile.write(reinterpret_cast<const char*>(&viewable), sizeof(bool));

		// Write pillar_property_
		outFile.write(reinterpret_cast<const char*>(&pillar_property), sizeof(Adesk::Int32));

		// Write pillar_serial_number_
		std::wstring serialNumber(pillar_serial_number ? pillar_serial_number : L"");
		size_t serialLen = serialNumber.length();
		outFile.write(reinterpret_cast<const char*>(&serialLen), sizeof(size_t));
		outFile.write(reinterpret_cast<const char*>(serialNumber.c_str()), serialLen * sizeof(wchar_t));

		// Write pillar_type_
		outFile.write(reinterpret_cast<const char*>(&pillar_type), sizeof(Adesk::Int32));

		// Write rect_pillar_vertex_
		Adesk::Int32 vertexCount = rect_pillar_vertex.length();
		outFile.write(reinterpret_cast<const char*>(&vertexCount), sizeof(Adesk::Int32));
		for (int i = 0; i < vertexCount; ++i)
		{
			outFile.write(reinterpret_cast<const char*>(&rect_pillar_vertex[i].x), sizeof(double));
			outFile.write(reinterpret_cast<const char*>(&rect_pillar_vertex[i].y), sizeof(double));
			outFile.write(reinterpret_cast<const char*>(&rect_pillar_vertex[i].z), sizeof(double));
		}

		// Write concrete_grade_
		outFile.write(reinterpret_cast<const char*>(&concrete_grade), sizeof(Adesk::Int32));

		// Check file state
		if (!outFile.good())
		{
			return Pola::Close(pillar, Acad::eFileAccessErr);
		}
		status = Pola::Close(pillar);
		if (status != Acad::eOk) return status;
	}

	outFile.close();
	return outFile.fail() ? Acad::eFileAccessErr : Acad::eOk;
}

// The caller owns pillar_t on both success and failure.

bool PillarTools::LoadPillarInfoFromFileBySn(const std::wstring& filePath, const TCHAR* sn, CPolaCustomPillar* pillar_t)
{
	bool found = false;
	const Acad::ErrorStatus status = LoadPillarInfoFromFileBySnChecked(filePath, sn, pillar_t, found);
	if (status != Acad::eOk) acutPrintf(_T("\nPillar import failed: %d"), static_cast<int>(status));
	return status == Acad::eOk && found;
}

Acad::ErrorStatus PillarTools::LoadPillarInfoFromFileBySnChecked(const std::wstring& filePath, const TCHAR* sn, CPolaCustomPillar* pillar_t, bool& found)
{
	found = false;
	if (!pillar_t || !sn) return Acad::eInvalidInput;
	std::ifstream stream(filePath, std::ios::in | std::ios::binary);
	if (!stream.is_open()) return Acad::eFileAccessErr;
	Pola::PillarDwgFields fields;
	const Acad::ErrorStatus status = Pola::ReadPillarBinaryBySerial(stream, sn, fields, found);
	if (status != Acad::eOk || !found) return status;
	pillar_t->assertWriteEnabled();
	pillar_t->center_point_ = fields.center;
	pillar_t->direction_vector_ = fields.direction;
	pillar_t->pillar_d_ = fields.diameter;
	pillar_t->pillar_h_ = fields.height;
	pillar_t->viewable_ = fields.visible;
	pillar_t->pillar_type_ = fields.type;
	pillar_t->pillar_property_ = fields.property;
	pillar_t->pillar_serial_number_.swap(fields.serial);
	pillar_t->concrete_grade_ = fields.grade;
	pillar_t->rect_pillar_vertex_ = fields.vertices;
	return Acad::eOk;
}
bool PillarTools::IsPillarInfoInFile(const std::wstring& filePath, const TCHAR* sn)
{
	CPolaCustomPillar pillar;
	return LoadPillarInfoFromFileBySn(filePath, sn, &pillar);
}

bool PillarTools::GetPillarBinPath(CString& binPath)
{
	TCHAR szFileName[MAX_PATH] = { 0 };
	CString path = _T("C:\\Users\\Polaris\\Documents\\arxini");

	if (DwgFileInfo::GetDwgFileNameWithoutExtension(szFileName, MAX_PATH))
		binPath.Format(_T("%s\\%s.bin"), path.GetString(), szFileName);
	else
		return false;
	return true;
}
