#ifndef _WINDOWS_PROCESS_HPP_
#define _WINDOWS_PROCESS_HPP_

#include <cstdint>
#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include "Input/Input.hpp"

class WindowsProcess
{
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void ExecuteMessageLoop();
	static void SetInputHandler(_In_ Eternal::Input::Input* InputHandler);
	static void Map(_In_ uint32_t WindowsKey, _In_ const Eternal::Input::Input::Key& KeyName);

	WindowsProcess();

private:
	static Eternal::Input::Input* _InputHandler;

	static void MapRange(_In_ uint32_t WindowsKeyStart, _In_ const Eternal::Input::Input::Key& KeyNameStart, _In_ uint32_t Range);
};

#endif
