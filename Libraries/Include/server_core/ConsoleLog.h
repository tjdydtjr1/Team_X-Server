#pragma once

/*=============================
		  ConsoleLog
=============================*/

enum Color
{
	BLACK = 0,
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
};

class ConsoleLog
{
	enum {BUFFER_SIZE = 4096};

public:
	ConsoleLog();
	~ConsoleLog();

public:
	void WriteStdOut(Color color, const WCHAR* str, ...);
	void WriteStdError(Color color, const WCHAR* str, ...);

protected:
	void SetColor(bool stdOut, Color color) const;

private:
	HANDLE _stdOut{};
	HANDLE _stdErr{};
};

