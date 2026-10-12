#include <afxwin.h>
#include <cassert>
#include <vector>
#include <cstdio>

struct AcGeIntArray
{
    std::vector<int> values;
    void append(int value) { values.push_back(value); }
    void removeAll() { values.clear(); }
};
#include "../DialogBarUi/CPolaMenu.h"
// The runner copies the production .cpp, stripping only SDK/PCH includes.
#include "MenuUnderTest.inc"
CWinApp application;

int main()
{
    assert(AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0));
    CPolaMenu menu;
    CStringArray config;
    config.Add(_T("***POP")); config.Add(_T("[Root]"));
    config.Add(_T("[->Nested]")); config.Add(_T("[Leaf]_LINE "));
    config.Add(_T("[<-Last]_POINT ")); config.Add(_T("[Sibling]_CIRCLE "));
    int command_id = 100;
    menu.Set(config, command_id);
    assert(menu.m_menu.GetMenuItemCount() == 1 && command_id == 103);
    CMenu* root = menu.m_menu.GetSubMenu(0);
    assert(root != nullptr && ::IsMenu(root->GetSafeHmenu()) && root->GetMenuItemCount() == 2);
    CMenu* nested = root->GetSubMenu(0);
    assert(nested != nullptr && ::IsMenu(nested->GetSafeHmenu()) && nested->GetMenuItemCount() == 2);
    assert(menu.m_saCommand.GetSize() == 3 && menu.m_saID.values.size() == 3);
    // Building a menu again replaces native handles and command metadata.
    config.Add(_T("***POP")); config.Add(_T("[Replacement]")); config.Add(_T("[One]_LINE "));
    command_id = 200;
    menu.Set(config, command_id);
    assert(menu.m_menu.GetMenuItemCount() == 1 && command_id == 201);
    assert(menu.m_saCommand.GetSize() == 1 && menu.m_saID.values.size() == 1);
    HMENU parent_handle = menu.m_menu.GetSafeHmenu();
    HMENU child_handle = menu.m_menu.GetSubMenu(0)->GetSafeHmenu();
    assert(::IsMenu(parent_handle) && ::IsMenu(child_handle));
    assert(menu.m_menu.DestroyMenu());
    assert(!::IsMenu(parent_handle) && !::IsMenu(child_handle));
    CStringArray parts;
    AnalyzeData(parts, _T("abc"), _T(""));
    assert(parts.GetSize() == 1 && parts[0] == _T("abc"));
    parts.Add(_T("stale output"));
    assert(!CPolaMenu::ReadMenuConfigFromFile(_T("\\?\nonexistent-AcPluginCpp-menu-config"), parts));
    assert(parts.IsEmpty());
    std::puts("MFC production menu regression: 5 cases passed");
    return 0;
}
