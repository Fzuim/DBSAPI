#include "StdAfx.h"
#include "IEventMonitor.h"

IEventMonitor::IEventMonitor(void)
{
}

IEventMonitor::~IEventMonitor(void)
{
}

void IEventMonitor::DoRefresh()
{

}

void IEventMonitor::SetMainHwnd(HWND v_hMainWnd)
{
	m_hMainWnd = v_hMainWnd;
}

HWND& IEventMonitor::GetMainHwnd()
{
	return m_hMainWnd;
}
