#include "CursorController.h"

CursorController::CursorController(HWND windowHandle, HCURSOR* cursorOut)
{
	hwnd = windowHandle;
	currentCursor = cursorOut;
}

void CursorController::setCursor(cursorType cursor)
{
	//HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
	switch (cursor)
	{
	case cursorType::normal:
		*currentCursor = LoadCursor(NULL, IDC_ARROW);
		break;
	case cursorType::hand:
		*currentCursor = LoadCursor(NULL, IDC_HAND);
		break;
	case cursorType::dragAll:
		*currentCursor = LoadCursor(NULL, IDC_SIZEALL);
		break;
	case cursorType::dragHorizontal:
		*currentCursor = LoadCursor(NULL, IDC_SIZEWE);
		break;
	case cursorType::dragVertical:
		*currentCursor = LoadCursor(NULL, IDC_SIZENS);
		break;
	case cursorType::wait:
		*currentCursor = LoadCursor(NULL, IDC_WAIT);
		break;
	default:
		break;
	}
}

void CursorController::setMousePosition(int x, int y)
{
    POINT pt{ x, y };
    ClientToScreen(hwnd, &pt);
    SetCursorPos(pt.x, pt.y);
}

void CursorController::showMouse(bool show)
{
	CURSORINFO ci = { sizeof(CURSORINFO) };
	GetCursorInfo(&ci);
	if (ci.flags != show)
		ShowCursor(show);
}
