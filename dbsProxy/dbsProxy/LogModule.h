/** \file
*	LogModule.h�������ӡ��debugview��־��
*
*	\author Fzuim
*
*	�汾��ʷ
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