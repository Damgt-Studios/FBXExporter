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
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Attack2.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Attack2_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Attack2.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Attack4.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Attack4_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Attack4.animfile");

	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Attack5.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Attack5_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Attack5.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Born.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Born_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Born.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Die2.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Die2_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Die2.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Die3.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Die3_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Die3.animfile");
	
	/*Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_HeavyHit_From_Back.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_HeavyHit_From_Back_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_HeavyHit_From_Back.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_HeavyHit_From_Front.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_HeavyHit_From_Front_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_HeavyHit_From_Front.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_HeavyHit_From_Left.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_HeavyHit_From_Left_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_HeavyHit_From_Left.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_HeavyHit_From_Right.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_HeavyHit_From_Right_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_HeavyHit_From_Right.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Idle.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Idle_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Idle.animfile");*/
	
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Idle_Stretch.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Idle_Stretch_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Idle_Stretch.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Look.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Look_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Look.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Look_Opp.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Look_Opp_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Look_Opp.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_3/Golem_3_Run.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Run_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Run.animfile");


	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Attack2.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Attack2_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Attack2.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Attack4.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Attack4_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Attack4.animfile");

	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Attack5.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Attack5_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Attack5.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Born.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Born_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Born.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Die2.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Die2_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Die2.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Die3.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Die3_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Die3.animfile");

	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_HeavyHit_From_Back.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_HeavyHit_From_Back_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_HeavyHit_From_Back.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_HeavyHit_From_Front.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_HeavyHit_From_Front_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_HeavyHit_From_Front.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_HeavyHit_From_Left.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_HeavyHit_From_Left_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_HeavyHit_From_Left.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_HeavyHit_From_Right.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_HeavyHit_From_Right_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_HeavyHit_From_Right.animfile");
	//Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Idle.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Idle_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Idle.animfile");

	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Idle_Stretch.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Idle_Stretch_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Idle_Stretch.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Look.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Look_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Look.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Look_Opp.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Look_Opp_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Look_Opp.animfile");
	Anim_FBX_InitLoad("Assets/Completed_Animated_Fbx/Golem_4/Golem_4_Run.fbx", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimMesh/Golem_4_Run_Mesh.AnimMesh", "ObjectData/Completed_Animated_Mesh_Files/Golem_4/AnimFile/Golem_4_Run.animfile");



	////Custom Mesh Files
	//std::vector<std::string> materials;
	//materials.push_back("Golem_Lava_Realistic_02_AlbedoTransparency.png");
	//materials.push_back("Golem_Lava_Realistic_02_Emission.png");
	//materials.push_back("Golem_Lava_Realistic_02_Normal.png");
	//CreateCustomFiles(materials, "ObjectData/Golem_2.mat");

	//SimpleMeshAnim test;
	//////The Following function  loads in the mesh
	//Load_AnimMesh("ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimMesh/Golem_3_Attack2_Mesh.AnimMesh", test);
	//vector<bones> skeleton;
	//vector<XMMATRIX> inverse_transforms;
	//anim_clip clip;
	////This one loads the animation file
	//Load_AnimFile("ObjectData/Completed_Animated_Mesh_Files/Golem_3/AnimFile/Golem_3_Attack2.animfile", skeleton, inverse_transforms, clip);
	//////FBX_InitLoad("Assets/cube.fbx", "ObjectData/cube.mesh");

 
 	 return 1;
}

