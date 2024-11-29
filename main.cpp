#include <windows.h>

HINSTANCE hInst;
int squareSize = 6; // Начальный размер квадрата
COLORREF squareColor = RGB(255, 0, 0); // Начальный цвет квадрата (красный)

// Функция для установки цвета квадрата
void SetSquareColor(int r, int g, int b) {
    squareColor = RGB(r, g, b);
}

// Функция для установки размера квадрата
void SetSquareSize(int size) {
    squareSize = size > 4 ? size : 4; // Минимальный размер 4
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Устанавливаем цвет для квадрата
        HBRUSH brush = CreateSolidBrush(squareColor);
        SelectObject(hdc, brush);

        // Получаем размеры окна
        RECT rect;
        GetClientRect(hwnd, &rect);

        // Вычисляем центр окна
        int centerX = (rect.right - rect.left) / 2;
        int centerY = (rect.bottom - rect.top) / 2;

        // Рисуем квадрат по центру окна
        Rectangle(hdc, centerX - squareSize / 2, centerY - squareSize / 2,
            centerX + squareSize / 2, centerY + squareSize / 2);

        DeleteObject(brush); // Освобождаем ресурсы
        EndPaint(hwnd, &ps);
    } break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN: {
        // Изменяем размер квадрата с помощью клавиш
        if (wParam == VK_UP) {
            SetSquareSize(squareSize + 2); // Увеличиваем размер
        }
        else if (wParam == VK_DOWN) {
            SetSquareSize(squareSize - 2); // Уменьшаем размер
        }

        // Обновляем размеры окна в соответствии с новым размером квадрата
        SetWindowPos(hwnd, nullptr, 0, 0, squareSize, squareSize, SWP_NOMOVE | SWP_NOZORDER);

        // Центрируем окно
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

        InvalidateRect(hwnd, NULL, TRUE); // Перерисовываем окно
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

    // Получаем разрешение экрана
    RECT desktopRect;
    HWND desktop = GetDesktopWindow();
    GetWindowRect(desktop, &desktopRect);
    int screenWidth = desktopRect.right;
    int screenHeight = desktopRect.bottom;

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED, // Окно всегда сверху и с поддержкой прозрачности
        CLASS_NAME,
        L"CrossDraw Beta v1.0",
        WS_POPUP | WS_VISIBLE, // Окно без рамок
        (screenWidth - squareSize) / 2, (screenHeight - squareSize) / 2, // Центрируем окно
        squareSize, squareSize,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    // Устанавливаем цвет квадрата (можно изменить на любой другой)
    SetSquareColor(0, 255, 0); // Устанавливаем цвет квадрата в зеленый

    // Устанавливаем стиль окна, чтобы оно не реагиров ало на мышь
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA); // Полная непрозрачность

    // Основной цикл сообщений
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}