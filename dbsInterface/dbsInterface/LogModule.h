/********************************************************************
	created:	2019/03/06
	created:	6:3:2019   13:42
	filename: 	..\hook_Inject\hook_Inject\LogModule.h
	file path:	..\hook_Inject\hook_Inject
	file base:	LogModule
	file ext:	h
	author:		fzuim
	
	purpose:	»’÷æ¿‡
*********************************************************************/
#pragma once

#include <stdio.h>
#include <stdlib.h>

class CLogModule
{
public:
	CLogModule();
	~CLogModule();

	static void Odprintf(PCWSTR format, ...);
	static void Odprintf(PCSTR format, ...);
};