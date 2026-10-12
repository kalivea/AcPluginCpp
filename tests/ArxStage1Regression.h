#pragma once

#include "../Common/DbObjectOwner.h"
#include <memory>
#include <fstream>
#include <stdexcept>
#include <string>

// Runs in a disposable side database. No document drawing is modified.
inline void RunPolaStage1Regression()
{
    struct WorkingDatabaseScope {
        AcDbDatabase* original;
        explicit WorkingDatabaseScope(AcDbDatabase* database)
            : original(acdbHostApplicationServices()->workingDatabase()) {
            acdbHostApplicationServices()->setWorkingDatabase(database);
        }
        ~WorkingDatabaseScope() { acdbHostApplicationServices()->setWorkingDatabase(original); }
    };
    int checks = 0;
    std::basic_string<TCHAR> result_path;
    auto check = [&checks](bool condition) {
        if (!condition) throw std::runtime_error("regression assertion failed");
        ++checks;
    };
    auto ok = [&check](Acad::ErrorStatus status) {
        if (status != Acad::eOk)
            acutPrintf(_T("\nPOLA_STAGE1_ERROR %s"), acadErrorStatusText(status));
        check(status == Acad::eOk);
    };
    try {
        TCHAR path[MAX_PATH] = {};
        const DWORD length = GetEnvironmentVariable(_T("ACPLUGIN_STAGE1_DWG"), path, MAX_PATH);
        check(length > 0 && length < MAX_PATH);
        result_path = std::basic_string<TCHAR>(path) + _T(".result.txt");
        AcDbDatabase database(true, true);
        WorkingDatabaseScope working(&database);
        AcDbObjectId id;
        check(AddToModelSpace::AddEntityToModelSpace(nullptr, id) == Acad::eInvalidInput && id.isNull());
        check(StyleTools::GetTextStyleId(_T("POLA_MISSING_STYLE_73912"), id) == Acad::eKeyNotFound && id.isNull());
        check(StyleTools::GetLayerId(_T("POLA_MISSING_LAYER_73912"), id) == Acad::eKeyNotFound && id.isNull());

        Pola::DbObjectOwner<AcDbLine> line(new AcDbLine(AcGePoint3d(0, 0, 0), AcGePoint3d(1000, 0, 0)));
        AcGeLineSeg3d segment;
        ok(BasicTools::EntityToLineSegment(line.get(), segment));
        check(line->startPoint() == AcGePoint3d::kOrigin);
        ok(AddToModelSpace::AddEntityToModelSpace(line.get(), id, false));
        AcDbObjectId rejected = id;
        check(AddToModelSpace::AddEntityToModelSpace(line.get(), rejected, false) == Acad::eInvalidInput && rejected.isNull());
        ok(line->setColorIndex(3));
        ok(line.release()->close());
        AcDbObjectId copy;
        ok(EditEntity::CopyEntity(id, AcGePoint3d::kOrigin, AcGePoint3d(0, 500, 0), copy));
        check(!copy.isNull());

        CPolaCustomPillar rectangle;
        ok(rectangle.setDiameter(600, 800));
        ok(rectangle.setPillarType(1));
        ok(rectangle.setCenterPoint(AcGePoint3d(3000, 4000, 0)));
        ok(rectangle.setSn(_T("P-001")));
        ok(rectangle.setConcreteGrade(35));
        ok(rectangle.setPillarProperty(0));
        rectangle.setViewable(true);
        ok(rectangle.CalculateVertex());
        ok(rectangle.CalculateVertex());
        check(rectangle.getRectVertex().length() == 4);
        CPolaCustomPillar independent(rectangle);
        ok(independent.setSn(_T("P-002")));
        check(_tcscmp(rectangle.getPillarSn(), _T("P-001")) == 0);
        ok(CPolaCustomPillar::SingleInsert(rectangle, rectangle.getCenterPoint(), id));
        CPolaCustomPillar round;
        ok(round.setDiameter(500, 0));
        ok(round.setSn(_T("R-001")));
        ok(round.setConcreteGrade(30));
        ok(CPolaCustomPillar::SingleInsert(round, AcGePoint3d(9000, 4000, 0), id));

        Pola::DbObjectOwner<CPolaCustomBeam> beam(new CPolaCustomBeam());
        ok(beam->setBeamWidth(300));
        ok(beam->setBeamHeight(600));
        ok(beam->setBeamProperty(7));
        AcGePoint3dArray vertices;
        vertices.append(AcGePoint3d(0, 0, 0));
        vertices.append(AcGePoint3d(5000, 0, 0));
        vertices.append(AcGePoint3d(5000, 3000, 0));
        ok(beam->setBeamVertexes(vertices));
        std::vector<Adesk::Int32> visible(3, 1);
        ok(beam->setBeamViewable(visible));
        ok(AddToModelSpace::AddEntityToModelSpace(beam.get(), id, false));
        std::unique_ptr<AcRxObject> clone(beam->clone());
        CPolaCustomBeam* beam_clone = CPolaCustomBeam::cast(clone.get());
        check(beam_clone && beam_clone->objectId().isNull() && beam_clone->getVertexesNum() == 3);
        ok(beam.release()->close());

        ok(StyleTools::CreateTextStyle(_T("grid_text"), _T(""), _T("txt.shx"), 1.0, id));
        Grid grid(500, 1000, 0);
        ok(grid.CreateVerticalAxis(id));
        AcDbObjectId same;
        ok(grid.CreateVerticalAxis(same));
        check(id == same);
        ok(Grid::InsertVerticalAxis(AcGePoint3d(10000, 0, 0), id));
        AcDbBlockReference* raw = nullptr;
        ok(acdbOpenObject(raw, id, AcDb::kForWrite));
        Pola::OpenedObject<AcDbBlockReference> reference(raw);
        ok(Grid::SetAttribute(reference.get(), _T("ID"), _T("A1")));
        ok(Pola::Close(reference));

        ok(database.saveAs(path, false, AcDb::kDHL_CURRENT));
        AcDbDatabase reopened(false, true);
        ok(reopened.readDwgFile(path, AcDbDatabase::OpenMode::kForReadAndAllShare));
        ok(reopened.closeInput(true));
        AcDbObjectIdArray ids;
        ok(BasicTools::GetAllEntityIdsInDatabase(ids, nullptr, &reopened));
        int pillars = 0, beams = 0, blocks = 0;
        for (int i = 0; i < ids.length(); ++i) {
            AcDbEntity* raw_entity = nullptr;
            ok(acdbOpenObject(raw_entity, ids[i], AcDb::kForRead));
            Pola::OpenedObject<AcDbEntity> entity(raw_entity);
            if (CPolaCustomPillar* pillar = CPolaCustomPillar::cast(entity.get())) {
                ++pillars;
                double diameter = 0, height = 0;
                pillar->getDiameter(diameter, height);
                if (pillar->getPillarType() == 1) {
                    check(diameter == 600 && height == 800 && pillar->getRectVertex().length() == 4);
                    check(_tcscmp(pillar->getPillarSn(), _T("P-001")) == 0 && pillar->getConcreteGrade() == 35);
                    check(pillar->getCenterPoint() == AcGePoint3d(3000, 4000, 0));
                } else {
                    check(diameter == 500 && _tcscmp(pillar->getPillarSn(), _T("R-001")) == 0);
                }
            }
            if (CPolaCustomBeam* saved_beam = CPolaCustomBeam::cast(entity.get())) {
                ++beams;
                check(saved_beam->getVertexesNum() == 3 && saved_beam->getBeamWidth() == 300 && saved_beam->getBeamHeight() == 600);
                check(saved_beam->getBeamProperty() == 7 && saved_beam->getBeamViewable() == visible);
                check(saved_beam->getBeamVertexes()[2] == AcGePoint3d(5000, 3000, 0));
            }
            if (AcDbBlockReference::cast(entity.get())) ++blocks;
            ok(Pola::Close(entity));
        }
        check(pillars == 2 && beams == 1 && blocks == 1);
        acutPrintf(_T("\nPOLA_STAGE1_PASS checks=%d\n"), checks);
        std::ofstream result(result_path.c_str());
        result << "POLA_STAGE1_PASS checks=" << checks << "\n";
    } catch (const std::exception& error) {
        acutPrintf(_T("\nPOLA_STAGE1_FAIL checks=%d %hs\n"), checks, error.what());
        if (!result_path.empty()) { std::ofstream result(result_path.c_str()); result << "POLA_STAGE1_FAIL checks=" << checks << " " << error.what() << "\n"; }
    }
}
