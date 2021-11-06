/* This file is part of ctrlaltt
 * Copyright (c) 2021 James Petersen.
 *
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

static const char *system_command = NULL;

static DWORD create_thread_thing(void *);

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int main(int argc, char **argv) {
	if (argc == 1) {
		system_command = "bash ~";
	} else {
		system_command = argv[argc - 1];
	}
	
	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	
	const wchar_t CLASS_NAME[] = L"BRUH";
	
	WNDCLASS wc = {};
	
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	
	RegisterClass(&wc);
	
	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"BRUH",
		WS_OVERLAPPEDWINDOW,
		
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if (!hwnd) {
		return 0;
	}
	
	ShowWindow(hwnd, SW_HIDE);
	
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
	destroy:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_ALT, 'T')) {
			goto destroy;
		}
		break;
	case WM_HOTKEY: if (wParam == 1) {
			HANDLE handle = CreateThread(
				NULL,
				0,
				create_thread_thing,
				NULL,
				0,
				NULL
			);
			CloseHandle(handle);
		} else { break; }
		return 0;
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static DWORD create_thread_thing(void *ptr) {
	system(system_command);
	return 0;
}

