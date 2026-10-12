#include <cassert>
#include <memory>
#include <vector>
#include <cstdio>

namespace Acad { enum ErrorStatus { eOk, eInvalidInput, eNoDatabase, eTableError, eRecordError, eAppendError, eCloseError }; }
namespace AcDb { enum OpenMode { kForRead, kForWrite }; }
#define _T(text) text
#define ACDB_MODEL_SPACE "*Model_Space"
template<class... Args> void acutPrintf(const char*, Args...) {}
struct AcDbObjectId
{
    int value = 0;
    static const AcDbObjectId kNull;
    bool isNull() const { return value == 0; }
};
const AcDbObjectId AcDbObjectId::kNull{};
struct AcDbObjectIdArray
{
    std::vector<AcDbObjectId> ids;
    void append(AcDbObjectId id) { ids.push_back(id); }
};
struct AcDbEntity
{
    inline static int destructions = 0;
    AcDbObjectId id;
    int closes = 0;
    Acad::ErrorStatus close_status = Acad::eOk;
    virtual ~AcDbEntity() { ++destructions; }
    AcDbObjectId objectId() const { assert(closes == 0 && "SDK object dereferenced after close"); return id; }
    Acad::ErrorStatus close() { ++closes; return close_status; }
};
struct AcDbBlockTableRecord
{
    Acad::ErrorStatus append_status = Acad::eOk, close_status = Acad::eOk;
    int appends = 0, closes = 0;
    Acad::ErrorStatus appendAcDbEntity(AcDbObjectId& id, AcDbEntity* entity)
    {
        ++appends;
        if (append_status == Acad::eOk) { id.value = 123; entity->id = id; }
        return append_status;
    }
    Acad::ErrorStatus close() { ++closes; return close_status; }
};
struct AcDbBlockTable
{
    AcDbBlockTableRecord record;
    Acad::ErrorStatus get_status = Acad::eOk, close_status = Acad::eOk;
    int closes = 0;
    Acad::ErrorStatus getAt(const char*, AcDbBlockTableRecord*& result, AcDb::OpenMode)
    {
        if (get_status == Acad::eOk) result = &record;
        return get_status;
    }
    Acad::ErrorStatus close() { ++closes; return close_status; }
};
struct AcDbDatabase
{
    AcDbBlockTable table;
    Acad::ErrorStatus get_status = Acad::eOk;
    Acad::ErrorStatus getBlockTable(AcDbBlockTable*& result, AcDb::OpenMode)
    {
        if (get_status == Acad::eOk) result = &table;
        return get_status;
    }
};
struct Host
{
    AcDbDatabase* database = nullptr;
    AcDbDatabase* workingDatabase() { return database; }
} host;
Host* acdbHostApplicationServices() { return &host; }
class AddToModelSpace
{
public:
    static Acad::ErrorStatus AddEntityToModelSpace(AcDbEntity*, AcDbObjectId&, bool close_entity = true);
    static AcDbObjectId AddEntityToModelSpace(AcDbEntity*);
    static AcDbObjectIdArray AddEntityToModelSpace(std::vector<AcDbEntity*>);
};
#include "DbObjectOwner.h"
// The runner copies the production .cpp here, stripping only SDK includes.
#include "AppendUnderTest.inc"

int main()
{
    int cases = 0;
    {
        AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(nullptr, id) == Acad::eInvalidInput && id.isNull());
        AcDbEntity resident; resident.id.value = 9;
        assert(AddToModelSpace::AddEntityToModelSpace(&resident, id) == Acad::eInvalidInput);
        assert(resident.closes == 0); ++cases;
    }
    {
        host.database = nullptr; AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eNoDatabase);
        assert(id.isNull() && entity.id.isNull() && entity.closes == 0); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.get_status = Acad::eTableError;
        AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eTableError);
        assert(id.isNull() && entity.closes == 0 && db.table.closes == 0); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.table.get_status = Acad::eRecordError;
        db.table.close_status = Acad::eCloseError; AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eRecordError);
        assert(id.isNull() && entity.closes == 0 && db.table.closes == 1 && db.table.record.closes == 0); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.table.record.append_status = Acad::eAppendError;
        db.table.record.close_status = db.table.close_status = Acad::eCloseError;
        AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eAppendError);
        assert(id.isNull() && entity.id.isNull() && entity.closes == 0 && db.table.closes == 1 && db.table.record.closes == 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eOk);
        assert(id.value == 123 && entity.closes == 1 && db.table.closes == 1 && db.table.record.closes == 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; AcDbEntity entity; entity.close_status = Acad::eCloseError; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eCloseError);
        assert(id.isNull() && entity.id.value == 123 && entity.closes == 1 && db.table.closes == 1 && db.table.record.closes == 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.table.record.close_status = Acad::eCloseError;
        AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id) == Acad::eCloseError);
        assert(id.isNull() && entity.id.value == 123 && entity.closes == 1 && db.table.closes == 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id,false) == Acad::eOk);
        assert(id.value == 123 && entity.closes == 0 && db.table.closes == 1 && db.table.record.closes == 1);
        entity.close(); assert(entity.closes == 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.table.close_status = Acad::eCloseError;
        AcDbEntity entity; AcDbObjectId id{999};
        assert(AddToModelSpace::AddEntityToModelSpace(&entity,id,false) == Acad::eCloseError);
        assert(id.isNull() && entity.id.value == 123 && entity.closes == 0);
        entity.close(); assert(entity.closes == 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.table.record.append_status = Acad::eAppendError;
        const int before = AcDbEntity::destructions;
        assert(AddToModelSpace::AddEntityToModelSpace(new AcDbEntity()).isNull());
        assert(AcDbEntity::destructions == before + 1); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; AcDbEntity* entity = new AcDbEntity;
        entity->close_status = Acad::eCloseError;
        assert(AddToModelSpace::AddEntityToModelSpace(entity).isNull());
        assert(entity->closes == 1 && entity->id.value == 123);
        delete entity; // simulates final database destruction, never a transient-failure delete
        ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; AcDbEntity resident; resident.id.value = 9;
        const int before = AcDbEntity::destructions;
        assert(AddToModelSpace::AddEntityToModelSpace(&resident).isNull());
        assert(resident.closes == 0 && AcDbEntity::destructions == before); ++cases;
    }
    {
        AcDbDatabase db; host.database = &db; db.table.record.append_status = Acad::eAppendError;
        const int before = AcDbEntity::destructions; auto* entity = new AcDbEntity;
        const auto ids = AddToModelSpace::AddEntityToModelSpace(std::vector<AcDbEntity*>{entity, entity, nullptr});
        assert(ids.ids.size() == 3 && ids.ids[0].isNull() && ids.ids[1].isNull() && ids.ids[2].isNull());
        assert(AcDbEntity::destructions == before + 1 && db.table.record.appends == 1); ++cases;
    }
    std::printf("Common production append regression: %d cases passed\n", cases);
}
