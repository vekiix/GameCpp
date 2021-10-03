#include<windows.h>

bool running = true;
void *buffer_memory;
int width;
int height;
BITMAPINFO bitmap_info;

LRESULT CALLBACK CallBack(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) 
{
	LRESULT result = 0;
	switch (uMsg) 
	{
		case WM_CLOSE: 
		case WM_DESTROY: 
		{
			running = false;
		}break;
		case WM_SIZE: 
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;

			int buffer_size = width * height * sizeof(unsigned int);
			if (buffer_memory)VirtualFree(buffer_memory, 0, MEM_RELEASE);
			buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
			bitmap_info.bmiHeader.biWidth = width;
			bitmap_info.bmiHeader.biHeight = height;
			bitmap_info.bmiHeader.biPlanes = 1;
			bitmap_info.bmiHeader.biBitCount = 32;
			bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;
		default: 
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	return result;
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	
	//Kreiraj WindowClass
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.hInstance = hInstance;
	window_class.lpszClassName = "GameCpp";
	window_class.lpfnWndProc = CallBack;

	//Registriraj WindowClass
	RegisterClass(&window_class);

	//Kreiraj Prozor
	HWND window = CreateWindowA(window_class.lpszClassName,"GameCpp", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	
	while (running) 
	{
		//Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//Simulate
		unsigned int* pixel =(unsigned int*)buffer_memory;
		for (int y = 0; y < height * width; y++) 
		{
			
				*pixel++ = 50;
			
		}

		//Render
		StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, buffer_memory, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
	
}