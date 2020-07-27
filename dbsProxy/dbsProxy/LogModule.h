/** \file
*	LogModule.h，定义打印到debugview日志类
*
*	\author Fzuim
*
*	版本历史
*	\par 2020-5-20
*
*/

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