#include <cstdio>
#include <mutex>
#include <thread>

#include "hooks.hpp"

#include "backend/dx10/hook_directx10.hpp"
#include "backend/dx11/hook_directx11.hpp"
#include "backend/dx12/hook_directx12.hpp"
#include "backend/dx9/hook_directx9.hpp"

#include "backend/opengl/hook_opengl.hpp"
#include "backend/vulkan/hook_vulkan.hpp"

#include "../console/console.hpp"
#include "../menu/menu.hpp"
#include "../utils/utils.hpp"

#include "../dependencies/minhook/MinHook.h"

#define DISABLE_LOGGING_CONSOLE

static HWND g_hWindow = NULL;
static std::mutex g_mReinitHooksGuard;

static DWORD WINAPI ReinitializeGraphicalHooks(LPVOID lpParam) {
    std::lock_guard<std::mutex> guard{ g_mReinitHooksGuard };

    HWND hNewWindow = U::GetProcessWindow();
    while (hNewWindow == reinterpret_cast<HWND>(lpParam)) {
        hNewWindow = U::GetProcessWindow();
    }

    H::bShuttingDown = true;

    H::Free();
    H::Init();

    H::bShuttingDown = false;
    Menu::bShowMenu = true;

    return 0;
}

static WNDPROC oWndProc;
static LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (uMsg == WM_KEYDOWN) {

        if (wParam == (Menu::menu_key)) {
            Menu::bShowMenu = !Menu::bShowMenu;
            return 0;
        }

    }
    else if (uMsg == WM_DESTROY) {


        HANDLE hHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ReinitializeGraphicalHooks, hWnd, 0, NULL);


        if (hHandle != NULL)
            CloseHandle(hHandle);
    }

    LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    if (Menu::bShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {


        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

namespace Hooks {
    void Init() {
        g_hWindow = U::GetProcessWindow();

#ifdef DISABLE_LOGGING_CONSOLE
        bool bNoConsole = GetConsoleWindow() == NULL;
        if (bNoConsole) {
            AllocConsole();
            HWND consoleWindow = GetConsoleWindow();
            ShowWindow(consoleWindow, SW_HIDE);
        }
#endif

        RenderingBackend_t eRenderingBackend = U::GetRenderingBackend();
        switch (eRenderingBackend) {
        case DIRECTX11:
            DX11::Hook(g_hWindow);
            break;
        }

#ifdef DISABLE_LOGGING_CONSOLE
        if (bNoConsole) {
            FreeConsole();
        }
#endif

        oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
    }

    void Free() {
        if (oWndProc) {

            SetWindowLongPtr(g_hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
        }

        MH_DisableHook(MH_ALL_HOOKS);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        RenderingBackend_t eRenderingBackend = U::GetRenderingBackend();

        switch (eRenderingBackend) {
        case DIRECTX11:
            DX11::Unhook();
            break;
        }
    }
} // namespace Hooks