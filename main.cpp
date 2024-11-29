#include <windows.h>

HINSTANCE hInst;
int squareSize = 6; // ��������� ������ ��������
COLORREF squareColor = RGB(255, 0, 0); // ��������� ���� �������� (�������)

// ������� ��� ��������� ����� ��������
void SetSquareColor(int r, int g, int b) {
    squareColor = RGB(r, g, b);
}

// ������� ��� ��������� ������� ��������
void SetSquareSize(int size) {
    squareSize = size > 4 ? size : 4; // ����������� ������ 4
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // ������������� ���� ��� ��������
        HBRUSH brush = CreateSolidBrush(squareColor);
        SelectObject(hdc, brush);

        // �������� ������� ����
        RECT rect;
        GetClientRect(hwnd, &rect);

        // ��������� ����� ����
        int centerX = (rect.right - rect.left) / 2;
        int centerY = (rect.bottom - rect.top) / 2;

        // ������ ������� �� ������ ����
        Rectangle(hdc, centerX - squareSize / 2, centerY - squareSize / 2,
            centerX + squareSize / 2, centerY + squareSize / 2);

        DeleteObject(brush); // ����������� �������
        EndPaint(hwnd, &ps);
    } break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN: {
        // �������� ������ �������� � ������� ������
        if (wParam == VK_UP) {
            SetSquareSize(squareSize + 2); // ����������� ������
        }
        else if (wParam == VK_DOWN) {
            SetSquareSize(squareSize - 2); // ��������� ������
        }

        // ��������� ������� ���� � ������������ � ����� �������� ��������
        SetWindowPos(hwnd, nullptr, 0, 0, squareSize, squareSize, SWP_NOMOVE | SWP_NOZORDER);

        // ���������� ����
        RECT desktopRect;
        HWND desktop = GetDesktopWindow();
        GetWindowRect(desktop, &desktopRect);
        int screenWidth = desktopRect.right;
        int screenHeight = desktopRect.bottom;

        SetWindowPos(hwnd, nullptr,
            (screenWidth - squareSize) / 2,
            (screenHeight - squareSize) / 2,
            0, 0,
            SWP_NOSIZE | SWP_NOZORDER);

        InvalidateRect(hwnd, NULL, TRUE); // �������������� ����
    } break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    hInst = hInstance;

    const wchar_t CLASS_NAME[] = L"CrossDrawBeta";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // �������� ���������� ������
    RECT desktopRect;
    HWND desktop = GetDesktopWindow();
    GetWindowRect(desktop, &desktopRect);
    int screenWidth = desktopRect.right;
    int screenHeight = desktopRect.bottom;

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED, // ���� ������ ������ � � ���������� ������������
        CLASS_NAME,
        L"CrossDraw Beta v1.0",
        WS_POPUP | WS_VISIBLE, // ���� ��� �����
        (screenWidth - squareSize) / 2, (screenHeight - squareSize) / 2, // ���������� ����
        squareSize, squareSize,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    // ������������� ���� �������� (����� �������� �� ����� ������)
    SetSquareColor(0, 255, 0); // ������������� ���� �������� � �������

    // ������������� ����� ����, ����� ��� �� �������� ��� �� ����
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA); // ������ ��������������

    // �������� ���� ���������
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}