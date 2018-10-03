#include "PlayerWindow.h"
#include <thread>
#include <iostream>

PlayerWindow::PlayerWindow() : PlayerWindow(500, 500)
{
	
}

PlayerWindow::PlayerWindow(int width, int heigth) : m_hwnd(nullptr), m_width(width), m_height(heigth)
{
	m_wclass = LPlayerWinClass;
	m_wtitle = LPlayerWinTitle;
	memset(&m_infohead, 0, sizeof(m_infohead));

	m_video_height = heigth;
	m_video_width = width;

	while (m_video_width % 4)
	{
		m_video_width++;
	}

	m_infohead.biSize = sizeof(m_infohead);
	m_infohead.biWidth = m_video_width;
	m_infohead.biHeight = m_video_height;
	m_infohead.biPlanes = 1;
	m_infohead.biBitCount = 24;
	m_infohead.biCompression = 0;
	m_infohead.biSizeImage = m_video_width * m_video_height * 3;

	m_img_buf = new char[m_video_width * m_video_height * 3];

	memset(m_img_buf, 0, m_video_width * m_video_height * 3);

	
}

PlayerWindow::~PlayerWindow()
{
	delete m_img_buf;
}

HWND PlayerWindow::get_hwnd()
{
	return m_hwnd;
}

void PlayerWindow::draw_img(char * data)
{	
	int n_line_pix = m_width * 3;
	for (int i = 0; i < m_height; ++i) 
	{
		memcpy(m_img_buf + i * n_line_pix, data + (m_height - i - 1) * n_line_pix, n_line_pix);
	}
	::StretchDIBits(m_hdc, 0, 0, m_video_width, m_video_height, 0, 0, m_video_width, m_video_height,
		m_img_buf, (LPBITMAPINFO)&m_infohead, DIB_RGB_COLORS, SRCCOPY);

}

int PlayerWindow::register_class()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, L"");
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"";
	wcex.lpszClassName = m_wclass.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, L"");

	return RegisterClassEx(&wcex);
}

void PlayerWindow::start_play()
{
	m_start_time = chrono::steady_clock::now();
}

void PlayerWindow::wait_times(long long int duration)
{
	long long int duration_ = 0;
	do 
	{
		auto cur_point = chrono::steady_clock::now();
		duration_ = (chrono::duration_cast<chrono::milliseconds>(cur_point - m_start_time)).count();

	} while (duration_ < duration);
}

int PlayerWindow::init_instance()
{

	m_hwnd = CreateWindow(m_wclass.c_str(), m_wtitle.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, m_width, m_height, NULL, NULL, m_hInst, NULL);

	m_hdc = GetDC(m_hwnd);

	if (!m_hwnd)
	{
		return FALSE;
	}

	ShowWindow(m_hwnd, 9);
	UpdateWindow(m_hwnd);
	return true;

}

int PlayerWindow::show_window()
{
	WM_DRAW_IMAGE = RegisterWindowMessage(L"LipracerMsg");

	if (!WM_DRAW_IMAGE)
	{
		cout << "Register fail!!!" << endl;
		return 0;
	}

	unique_lock<mutex> uqlck(m_window_start_mtx);
	auto msg_loop = [this]() -> int 
	{
		MSG msg;

		m_hInst = GetModuleHandle(0);

		do
		{
			if (!register_class())
			{
				break;
			}

			if (!init_instance())
			{
				break;
			}
		} while (0);
		
		cout << "create window notify_one!!!" << endl;
		m_window_start_cdv.notify_one();

		while (GetMessage(&msg, m_hwnd, 0, 0/*, PM_REMOVE*/))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			//else if (msg.message == WM_DRAW_IMAGE)
			//{

			//}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		return (int)msg.wParam;
	};

	thread th_msg_loop(msg_loop);
	th_msg_loop.detach();

	m_window_start_cdv.wait(uqlck, [this]() { return m_hwnd; });

	cout << "create window succeed!!!" << endl;

	return 0;
	
}

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;


	switch (message)
	{
	case WM_COMMAND:
	
		break;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	
		PostQuitMessage(0);
		break;
	default:
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
