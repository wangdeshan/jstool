//This file is part of JSLint Plugin for Notepad++
//Copyright (C) 2010 Martin Vladic <martin.vladic@gmail.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#pragma once

#include "Util.h"

////////////////////////////////////////////////////////////////////////////////

class JSLintReportItem
{
public:
	JSLintReportItem(int line, int character, const string& strReason, const string& strEvidence)
		: m_line(line)
		, m_character(character)
		, m_strReason(strReason)
		, m_strEvidence(strEvidence)
	{
	}

	int GetLine() const { return m_line; }
	int GetCharacter() const { return m_character; }
	string GetReason() const { return m_strReason; }
	string GetEvidence() const { return m_strEvidence; }

private:
	int m_line;
	int m_character;
	string m_strReason;
	string m_strEvidence;
};

////////////////////////////////////////////////////////////////////////////////

class JSLint
{
public:
	void CheckScript(const string& strOptions, const string& strScript, list<JSLintReportItem>& items);

private:
	TempFile m_jsLintScriptFileName;

	void CreateJSLintFile();
	
	void LoadCustomDataResource(HMODULE hModule, 
		LPCTSTR lpName, LPCTSTR lpType, LPVOID* ppData, LPDWORD pdwSize);
	
	static void WriteString(HANDLE hFile, const string& strScript);
	
	void ParseOutput(HANDLE hProcess, HANDLE hPipe, 
		list<JSLintReportItem>& items);
	
	void ReadError(HANDLE hProcess, HANDLE hPipe, string& strError);
	
	bool ReadFromPipe(HANDLE hProcess, HANDLE hPipe, char *buffer, 
		DWORD dwBufferSize, DWORD& dwRead);
};

////////////////////////////////////////////////////////////////////////////////

class JSLintException : public exception 
{
};


class JSLintResourceException : public JSLintException 
{
};

class JSLintUnexpectedException : public JSLintException 
{
};