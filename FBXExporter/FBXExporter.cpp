// FBXExporter.cpp : This file contains the 'main' function. Program execution begins and ends there.
#pragma once

#include "FBXExport.h"
#include <iostream>

int main()
{

	//Example
					 //Fbx File             //Mesh File     //new material file filepath  //new material file
	//	FBX_InitLoad("Assets/Water.fbx", "ObjectData/Water.mFbx", "Assets/water.fbm", "water.materials");
	
	//P.S. material file is simply a file containing the file paths of the actual textures
	// If you want to work in another project you'll need to store the textures as dds afterward in a similar path to the one you create  here
	//Also it wont matter if the model is not textured by default
	// so do something like I did in the following line
	SimpleMesh mesh;
	FBX_InitLoad("Assets/Golem.fbx", "ObjectData/ForestGolem_2.mesh");
	Load_FBX("ObjectData/ForestGolem_2.mesh", mesh);
	//FBX_InitLoad("Assets/cube.fbx", "ObjectData/cube.mesh");
	return 1;
}

