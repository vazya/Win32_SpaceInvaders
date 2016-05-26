void PainterClass::Draw() {
///*
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

	DrawMatrix(memDC);
//	DrawShip(memDC);
	DrawShipBullet(memDC);
////////	DrawShipBitmap2(dc, memDC);
	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
	MoveShip();
	HANDLE hOldBitmap = SelectObject(memDC, hBitmap);
	StretchBlt(dc,ship.x,ship.y,ship.width,ship.height,memDC,0, 0,Bitmap.bmWidth,Bitmap.bmHeight,SRCCOPY);
	SelectObject(memDC, oldBitmap);
	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);


	::DeleteObject(memBitmap);
	::DeleteObject(oldBitmap);
	::DeleteDC(dc);
	::DeleteDC(memDC);
}