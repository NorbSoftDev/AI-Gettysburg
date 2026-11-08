///--------------

#pragma once

#include <iostream>     // std::cout
#include <sstream>      // std::ostringstream

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"





 class CXSowwl
 {
 public:

 static const int attivo = 3;

 static void LogWrite( std::string chiave, std::string chiave2, int numero); 

 static void LogWriteChar( std::string chiave, std::string chiave2, std::string chiave3 ); 

  static void OfficerLogWrite( std::string chiave, std::string chiave2, int numero); 

 static void OfficerLogWriteChar( std::string chiave, std::string chiave2, std::string chiave3 ); 

  static void SpecificLogWrite( std::string chiave, std::string chiave2, int numero); 

 static void SpecificLogWriteChar( std::string chiave, std::string chiave2, std::string chiave3 ); 

 };




//------------------

