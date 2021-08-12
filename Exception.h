#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H


#define THROW_ERROR(msg) THROW_ERROR_BASE(__LINE__, __FILE__, msg);

void THROW_ERROR_BASE(int line, const char* file, const char* msg);


#endif