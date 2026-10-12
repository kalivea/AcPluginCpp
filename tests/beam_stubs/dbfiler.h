#pragma once
#include <cstdint>
namespace Adesk { using Int32 = std::int32_t; using UInt32 = std::uint32_t; }
namespace Acad {
enum ErrorStatus { eOk, eInvalidInput, eMakeMeProxy, eOutOfMemory, eEndOfFile, eFileAccessErr };
}
