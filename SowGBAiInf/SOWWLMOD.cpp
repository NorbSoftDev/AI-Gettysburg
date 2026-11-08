// SOWWLMOD
#include <stdafx.h>
#include "SOWWLMOD.h"
#include <WinBase.h>


void CXSowwl::LogWrite( std::string chiave, std::string chiave2, int numero )
{

#ifndef _DEBUG
	numero = 0;
#else
	std::string stringa;

	int flag = 1;

	//if  (chiave == "85e Ligne / 1e Batallion")
	//{
	if ( attivo == flag )
	{
		std::ostringstream convert;   // stream used for the conversion

		//
		convert << numero;      // insert the textual representation of 'Number' in the characters in the stream
		//

		stringa = chiave + chiave2 + ":" + convert.str()   + "\r\n"; // set 'Result' to the contents of the stream
		//
		const char *Result = stringa.c_str();
		//
		OutputDebugString(Result);
		CXUtil::AddLog(Result);
	}
	//}
#endif


};

void CXSowwl::LogWriteChar( std::string chiave, std::string chiave2, std::string chiave3 )
{

#ifdef _DEBUG
	std::string stringa;

	int flag = 1;

	//if  (chiave == "55e Ligne / 1e Batallion")
	//{
	if ( attivo == flag )
	{

		stringa = chiave + chiave2 + ":" + chiave3   + "\r\n" ; // set 'Result' to the contents of the stream
		//
		const char *Result = stringa.c_str();
		//
		OutputDebugString(Result);
		CXUtil::AddLog(Result);
	}
	//}
#endif

};

void CXSowwl::OfficerLogWrite( std::string chiave, std::string chiave2, int numero )
{
#ifndef _DEBUG
	numero = 0;
#else
	std::string stringa;

	int flag = 2;

	//if  (chiave == "Kapitain Carl von Rettberg")
	//{
	if ( attivo == flag )
	{
		std::ostringstream convert;   // stream used for the conversion

		//
		convert << numero;      // insert the textual representation of 'Number' in the characters in the stream
		//

		stringa = chiave + chiave2 + ":" + convert.str()   + "\r\n" ; // set 'Result' to the contents of the stream
		//
		const char *Result = stringa.c_str();
		//
		//

		OutputDebugString(Result);
		CXUtil::AddLog(Result);

	}
#endif


};

void CXSowwl::OfficerLogWriteChar( std::string chiave, std::string chiave2, std::string chiave3 )
{
#ifdef _DEBUG
	std::string stringa;

	int flag = 2;

	//if  (chiave == "55e Ligne / 1e Batallion")
	//{
	if ( attivo == flag )
	{

		stringa = chiave + chiave2 + ":" + chiave3   + "\r\n" ; // set 'Result' to the contents of the stream
		//
		const char *Result = stringa.c_str();
		//
		OutputDebugString(Result);
		CXUtil::AddLog(Result);
	}
	//}
#endif
};


void CXSowwl::SpecificLogWrite( std::string chiave, std::string chiave2, int numero )
{
#ifndef _DEBUG
	numero = 0;
#else
	std::string stringa;

	int flag = 3;

	if ( attivo == flag)
	{
		std::ostringstream convert;   // stream used for the conversion

		//
		convert << numero;      // insert the textual representation of 'Number' in the characters in the stream
		//

		stringa = chiave + chiave2 + ":" + convert.str()  + "\r\n"; // set 'Result' to the contents of the stream
		//
		const char *Result = stringa.c_str();
		//
		OutputDebugString(Result);
		CXUtil::AddLog(Result);

	}

#endif


};

void CXSowwl::SpecificLogWriteChar( std::string chiave, std::string chiave2, std::string chiave3 )
{
#ifdef _DEBUG
	std::string stringa;

	int flag = 3;

	//if  (chiave == "GÃ©nÃ©ral de Division Baron Jean-Baptiste Jeanin")
	//{
	if ( attivo == flag )
	{

		stringa = chiave + chiave2 + ":" + chiave3 + "\r\n"; // set 'Result' to the contents of the stream
		//
		const char *Result = stringa.c_str();
		//
		OutputDebugString(Result);
		CXUtil::AddLog(Result);
	}
	//}

#endif
};