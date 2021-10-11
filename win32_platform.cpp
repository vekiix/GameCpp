#include<windows.h>
#include<ctime>
#include "Platform_Common.cpp"

struct Render_Info 
{
	void* buffer_memory;
	int width, height;
	BITMAPINFO bitmap_info;
};
Render_Info render_info;

#include "Renderer.cpp"
#include "game.cpp"

bool running = true;

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
			render_info.width = rect.right - rect.left;
			render_info.height = rect.bottom - rect.top;

			int buffer_size = render_info.width * render_info.height * sizeof(unsigned int);
			if (render_info.buffer_memory)VirtualFree(render_info.buffer_memory, 0, MEM_RELEASE);
			render_info.buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_info.bitmap_info.bmiHeader.biSize = sizeof(render_info.bitmap_info.bmiHeader);
			render_info.bitmap_info.bmiHeader.biWidth = render_info.width;
			render_info.bitmap_info.bmiHeader.biHeight = render_info.height;
			render_info.bitmap_info.bmiHeader.biPlanes = 1;
			render_info.bitmap_info.bmiHeader.biBitCount = 32;
			render_info.bitmap_info.bmiHeader.biCompression = BI_RGB;
			render_background();
			//render();

		} break;
		default: 
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	return result;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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

	Input input = {};
	render_background();



	float delta_time = 0.016666666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (running) 
	{
		//Input
		for (int i = 0; i < BUTTON_COUNT; i++) 
		{
			input.buttons[i].changed = false;
		}
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) 
		{
			switch (message.message) 
			{
				case WM_KEYUP:
				{
				}
				case WM_KEYDOWN:
				{
					unsigned int vk_code = (unsigned int)message.wParam;
					bool is_down = ((message.lParam & (static_cast <unsigned __int64>(1) << static_cast <unsigned __int64>(31))) == 0);
					switch (vk_code) 
					{
						case VK_LEFT: 
						{
							input.buttons[BUTTON_LEFT].changed = is_down != input.buttons[BUTTON_LEFT].is_down;
							input.buttons[BUTTON_LEFT].is_down = is_down;
						}break;
						case VK_RIGHT: 
						{
							input.buttons[BUTTON_RIGHT].changed = is_down != input.buttons[BUTTON_RIGHT].is_down;
							input.buttons[BUTTON_RIGHT].is_down = is_down;
						}break;
					}

				}break;
				default: 
				{
					TranslateMessage(&message);
					DispatchMessage(&message);	
				}	
			}
		}
		//Simulate
		try
		{
			simulate_game(&input, delta_time);
		}
		catch(int score)
		{
			running = false;
			
		}
		//Render
		StretchDIBits(hdc, 0, 0, render_info.width, render_info.height, 0, 0, render_info.width, render_info.height, render_info.buffer_memory, &render_info.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
}