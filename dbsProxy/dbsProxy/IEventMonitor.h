#pragma once

enum EVENT_TYPE {_ENVEVENT_, _WDQTEVENT_};

class IEventMonitor
{
public:
	IEventMonitor(void);
	virtual ~IEventMonitor(void);

	virtual void DoRefresh();

	void SetMainHwnd(HWND v_hMainWnd);
	HWND& GetMainHwnd();

private:
	HWND m_hMainWnd;
};
