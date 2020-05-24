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
//	FBX_InitLoad("Assets/Malbers Animations/Monsters/Forest Golem/Models/Forest_Golem_Test.fbx", "ObjectData/ForestGolem_1.mesh");

//We Now Can load meshes with Animations 
	//The following Function works the same as the one from before however now it generates meshes with joint and weight data
	//Also generates a Animation file with base joint data, inverse joint data and animation clips
	Anim_FBX_InitLoad("Assets/Golem_Idle.fbx", "ObjectData/TestAnim.AnimMesh", "ObjectData/Test.animfile", "Assets/Run.fbm", "ObjectData/BattleMage.mat");

	//Custom Mesh Files
	std::vector<std::string> materials;
	materials.push_back("Golem_Lava_Realistic_02_AlbedoTransparency.png");
	materials.push_back("Golem_Lava_Realistic_02_Emission.png");
	materials.push_back("Golem_Lava_Realistic_02_Normal.png");
	CreateCustomFiles(materials, "ObjectData/Golem_2.mat");

	SimpleMeshAnim test;
	//The Following function  loads in the mesh
	Load_AnimMesh("ObjectData/TestAnim.AnimMesh", test);
	vector<bones> skeleton;
	vector<XMMATRIX> inverse_transforms;
	anim_clip clip;
	//This one loads the animation file
	Load_AnimFile("ObjectData/Test.animfile", skeleton, inverse_transforms, clip);
	//FBX_InitLoad("Assets/cube.fbx", "ObjectData/cube.mesh");

 
 	 return 1;
}

