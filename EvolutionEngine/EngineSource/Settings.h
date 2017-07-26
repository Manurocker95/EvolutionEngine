/*
This file is part of Evolution Engine

Started 21 / 05 / 2017

Copyright (C) 2017 Manuel Rodríguez Matesanz
>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.

> email: manuelrodriguezmatesanz@gmail.com

*/

#pragma once

//Context settings:

static const size_t window_width = 800;												// Window Width
static const size_t window_height = 600;											// Window Height
static const unsigned int default_buffer_bit_depth = 24;
static const unsigned int default_stencil = 0;
static const unsigned int default_antialising = 0;
static const unsigned int major_context_version = 3;
static const unsigned int minor_context_version = 2;
static const unsigned int number_of_keys = 5;

// Default paths and file names
static const char * default_obj_path = "Assets\\Models\\";							// Default path for objs. So you can pass only the name of the obj
static const char * default_skybox = "Assets\\textures\\sky-cube-map-";				// Default path for Skybox in case the xml doesn't contain one
static const char * errorLog_Folder = "Errorlogs\\";								// Folder for saving errorlogs
static const char * errorlog = "ErrorLog.txt";										// Errorlog file name
static const char * scene_file_extension = "evolscene";
// Other Settings
static const bool save_errorlog = true;												// Boolean that checks if we save or not the error log after showing an error
static const bool close_if_loading_error = false;									// Boolean that checks if we have to close the window when showing an error or load a default object
static const bool showing_middleware_demo = true;