#include "stdafx.h"
#include "PillarTools.h"
#include "SelectEntitys.h"
#include "DwgFileInfo.h"
#include <fstream>
#include <string>

bool PillarTools::detectPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter)
{
	AcDbObjectIdArray all_pillar_ids;
	SelectEntitys::GetAllEntitysByType(CPolaCustomPillar::desc(), all_pillar_ids);
	AcDbExtents extents;
	for (const auto& pillar : all_pillar_ids)
	{
		AcDbEntity* entity = nullptr;
		Acad::ErrorStatus error_status = acdbOpenObject(entity, pillar, OpenMode::kForRead);
		if (error_status != Acad::eOk)
			return false;
		CPolaCustomPillar* pillar = CPolaCustomPillar::cast(entity);
		pillar->getGeomExtents(extents);
		if (BasicTools::IsPointInRectangle(point, extents))
		{
			pillar_id = entity->objectId();
			double temp_d, temp_h;
			pillar->getDiameter(temp_d, temp_h);
			pillar_diameter = temp_d;
			pillar->close();
			return true;
		}
		entity->close();
	}
	pillar_id = AcDbObjectId::kNull;
	pillar_diameter = 0;
	return false;
}

bool PillarTools::detectRoundPillar(const AcGePoint3d& point, AcDbObjectId& pillar_id, double& pillar_diameter)
{
	AcDbObjectId pillar;
	double diameter;
	double height = 0;
	if (detectPillar(point, pillar, diameter))
	{
		AcDbEntity* entity = nullptr;
		Acad::ErrorStatus error_status = acdbOpenObject(entity, pillar, OpenMode::kForRead);
		if (error_status != Acad::eOk)
			return false;
		CPolaCustomPillar* pillar_entity = CPolaCustomPillar::cast(entity);
		if (pillar_entity->getPillarType() == 0)
		{
			pillar_entity->close();
			pillar_id = pillar_entity->objectId();
			pillar_diameter = diameter;
			return true;
		}
		pillar_entity->close();
	}
	return false;
}

bool PillarTools::GetAllPillar(AcDbObjectIdArray& all_pillar_ids)
{
	return SelectEntitys::GetAllEntitysByType(CPolaCustomPillar::desc(), all_pillar_ids);
}

bool PillarTools::SaveAllPillarInfoToFile(const std::wstring& filePath)
{
	std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!outFile.is_open())
		return false;

	AcDbObjectIdArray all_pillar_id;
	GetAllPillar(all_pillar_id);

	// Write the number of pillars
	Adesk::Int32 pillarCount = all_pillar_id.length();
	outFile.write(reinterpret_cast<const char*>(&pillarCount), sizeof(Adesk::Int32));

	AcDbObjectPointer<CPolaCustomPillar> pillar;
	for (const auto& id : all_pillar_id)
	{
		if (pillar.open(id, AcDb::kForRead) != Acad::eOk)
		{
			outFile.close();
			return false; // Failed to open pillar
		}

		AcGePoint3d center_point = pillar->getCenterPoint();
		AcGeVector3d direction_vector = pillar->getDirectionVector();
		double pillar_d, pillar_h;
		pillar->getDiameter(pillar_d, pillar_h);
		bool viewable = pillar->getViewable();
		Adesk::Int32 pillar_property = pillar->getPillarProperty();
		TCHAR* pillar_serial_number = pillar->getPillarSn();
		Adesk::Int32 pillar_type = pillar->getPillarType();
		AcGePoint3dArray rect_pillar_vertex = pillar->getRectVertex();
		Adesk::Int32 concrete_grade = pillar->getConcreteGrade();

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
			outFile.close();
			return false;
		}
		pillar.close(); // Explicitly close the pillar object
	}

	outFile.close();
	return true;
}

// if return false dnt forget [delete pillar_t]!!

bool PillarTools::LoadPillarInfoFromFileBySn(const std::wstring& filePath, TCHAR* sn, CPolaCustomPillar* pillar_t)
{
	bool result = false;

	std::ifstream inFile(filePath, std::ios::in | std::ios::binary);
	if (!inFile.is_open())
		return false;

	// Read pillar count
	Adesk::Int32 pillar_count = 0;
	inFile.read(reinterpret_cast<char*>(&pillar_count), sizeof(Adesk::Int32));
	if (!inFile.good() || pillar_count <= 0)
	{
		inFile.close();
		return false;
	}

	for (Adesk::Int32 i = 0; i < pillar_count; ++i)
	{
		// Read center point
		AcGePoint3d center_point;
		inFile.read(reinterpret_cast<char*>(&center_point.x), sizeof(double));
		inFile.read(reinterpret_cast<char*>(&center_point.y), sizeof(double));
		inFile.read(reinterpret_cast<char*>(&center_point.z), sizeof(double));

		// Read direction vector
		AcGeVector3d direction_vector;
		inFile.read(reinterpret_cast<char*>(&direction_vector.x), sizeof(double));
		inFile.read(reinterpret_cast<char*>(&direction_vector.y), sizeof(double));
		inFile.read(reinterpret_cast<char*>(&direction_vector.z), sizeof(double));

		// Read pillar dimensions
		double pillar_d = 0.0, pillar_h = 0.0;
		inFile.read(reinterpret_cast<char*>(&pillar_d), sizeof(double));
		inFile.read(reinterpret_cast<char*>(&pillar_h), sizeof(double));

		// Read viewable status
		bool viewable = false;
		inFile.read(reinterpret_cast<char*>(&viewable), sizeof(bool));

		// Read pillar property
		Adesk::Int32 pillar_property = 0;
		inFile.read(reinterpret_cast<char*>(&pillar_property), sizeof(Adesk::Int32));

		// Read serial number
		size_t serialLen = 0;
		inFile.read(reinterpret_cast<char*>(&serialLen), sizeof(size_t));
		std::wstring pillar_serial_number;
		if (serialLen > 0)
		{
			std::vector<wchar_t> buffer(serialLen);
			inFile.read(reinterpret_cast<char*>(buffer.data()), serialLen * sizeof(wchar_t));
			pillar_serial_number.assign(buffer.data(), serialLen);
		}

		// Read pillar type
		Adesk::Int32 pillar_type = 0;
		inFile.read(reinterpret_cast<char*>(&pillar_type), sizeof(Adesk::Int32));

		// Read rectangle vertices
		Adesk::Int32 vertexCount = 0;
		inFile.read(reinterpret_cast<char*>(&vertexCount), sizeof(Adesk::Int32));
		AcGePoint3dArray rect_pillar_vertex;
		for (int j = 0; j < vertexCount; ++j)
		{
			AcGePoint3d vertex;
			inFile.read(reinterpret_cast<char*>(&vertex.x), sizeof(double));
			inFile.read(reinterpret_cast<char*>(&vertex.y), sizeof(double));
			inFile.read(reinterpret_cast<char*>(&vertex.z), sizeof(double));
			rect_pillar_vertex.append(vertex);
		}

		// Read concrete grade
		Adesk::Int32 concrete_grade = 0;
		inFile.read(reinterpret_cast<char*>(&concrete_grade), sizeof(Adesk::Int32));

		// Validate stream state
		if (!inFile.good())
		{
			inFile.close();
			return false;
		}

		if (pillar_serial_number.compare(sn) == 0)
		{
			pillar_t->setSn(pillar_serial_number.c_str());
			pillar_t->setConcreteGrade(concrete_grade);
			pillar_t->setDiameter(pillar_d, pillar_h);
			pillar_t->setViewable(viewable);
			pillar_t->setPillarType(pillar_type);
			pillar_t->setPillarProperty(pillar_property);
			pillar_t->setCenterPoint(center_point);
			pillar_t->CalculateVertex();
			result = true;
			break;
		}
	}
	inFile.close();
	return result;
}

bool PillarTools::IsPillarInfoInFile(const std::wstring& filePath, TCHAR* sn)
{
	CPolaCustomPillar* pillar = new CPolaCustomPillar;
	if (LoadPillarInfoFromFileBySn(filePath, sn, pillar))
	{
		delete pillar;
		return true;
	}
	else
	{
		delete pillar;
		return false;
	}
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

