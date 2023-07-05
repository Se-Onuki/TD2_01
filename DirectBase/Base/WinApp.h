#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <cstdint>

#include <wrl.h>

//#include <string>


#include "../../externals/imgui/imgui.h"
#include "../../externals/imgui/imgui_impl_dx12.h"
#include "../../externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class WinApp
{
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	RECT wrc;
	HWND hwnd;
	WNDCLASS wc{};	// ウィンドウクラス


	ComPtr<ID3D12Debug1> debugController;

public:
	WinApp();
	~WinApp();

	// ウィンドウサイズ
	static const int kWindowWidth = 1280; // 横幅
	static const int kWindowHeight = 720; // 縦幅


	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void CreateGameWindow(
		const char *title = "DirectXGame", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth, int32_t clientHeight = kWindowHeight);
};