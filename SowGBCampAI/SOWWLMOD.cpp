// SOWWLMOD
#include <stdafx.h>
#include "SOWWLMOD.h"
#include <WinBase.h>


void CXSowwl::LogWrite( std::string chiave, std::string chiave2, int numero )
 {
#ifdef _DEBUG
std::string stringa;

//if  (chiave == "85e Ligne / 1e Batallion")
//{
if ( attivo == 1 )
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

//if  (chiave == "55e Ligne / 1e Batallion")
//{
if ( attivo == 1 )
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
#ifdef _DEBUG

std::string stringa;

//if  (chiave == "1st/95th Regiment of Foot (Riflemen) The Rifles")
//{
if ( attivo == 2 )
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

//if  (chiave == "55e Ligne / 1e Batallion")
//{
if ( attivo == 2 )
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
#ifdef _DEBUG

std::string stringa;

//if  (chiave == "1st/95th Regiment of Foot (Riflemen) The Rifles")
//{
if ( attivo == 3 )
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

//if  (chiave == "55e Ligne / 1e Batallion")
//{
if ( attivo == 3 )
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