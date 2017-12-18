#pragma once
#ifndef __TEXTURE_LOADER_H
#define __TEXTURE_LOADER_H
//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#include "glut.h"

void loadTexture(GLuint* texID, const char * imagepath);

#endif