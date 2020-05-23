#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <DirectXMath.h>

#include <fbxsdk.h>

using namespace std;
using namespace DirectX;
using file_path_t = std::array<char, 260>; // Simple max size file path string

int numVertices = 0;
int* indices;
int numIndices = 0;
float scale = 1.0f;
struct SimpleVertex
{

	XMFLOAT3 Position;
	XMFLOAT3 Tex;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
};
SimpleVertex* vertices;


struct SimpleMesh
{
	vector<SimpleVertex> vertexList;
	vector<int> indicesList;
};
// Simple material definition
struct material_t
{
public:
	enum e_component { EMISSIVE = 0, DIFFUSE, NORMAL, SPECULAR, SHININESS, COUNT };
	struct component_t
	{
		float value[3] = { 0.0f, 0.0f, 0.0f };
		float factor = 0.0f;
		int64_t input = -1;
	};
	component_t& operator[](int i) { return components[i]; }
	const component_t& operator[](int i)const { return components[i]; }
private:
	component_t components[COUNT];
};



vector<XMFLOAT3> positionList;
vector<XMFLOAT3> normalList;
vector<XMFLOAT3> uvList;

vector< int > positionIndices, uvIndices, normalIndices;

struct SimpleVertexAnim
{
	XMFLOAT3 Position;
	XMFLOAT3 Tex;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;

	XMINT4 Joint;
	XMFLOAT4 Weight;

};
struct SimpleMeshAnim
{
	vector<SimpleVertexAnim> vertexList;
	vector<int> indicesList;
};


std::vector<file_path_t> paths;



void FBX_InitLoad(const char* fbxfile, const char* meshfile, const char* matPath = "", const char* matfile = "");

void ProcessFbxMesh(FbxNode* Node, const char* meshfile, const char* matpath = "", const char* matfile = "");
void Compactify(SimpleVertex* verticesCompact, const char* meshfile);
void ComputeTangent(SimpleMesh& simpleMesh);

void Load_Mesh(const char* meshFileName, SimpleMesh& mesh);

void  FBX_InitLoad(const char* fbxfile, const char* meshfile, const char* matPath, const char* matfile)
{

	// Change the following filename to a suitable filename value.
	const char* lFilename = fbxfile;

	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();


	// Process the scene and build DirectX Arrays
	ProcessFbxMesh(lScene->GetRootNode(), meshfile, matPath, matfile);

	int Done;
	Done = 0;


};
void  ProcessFbxMesh(FbxNode* Node, const char* meshfile, const char* matpath, const char* matfile)
{
	// set up output console
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	//FBX Mesh stuff
	int childrenCount = Node->GetChildCount();

	//std::cout << "\nName:" << Node->GetName();
	SimpleVertex* vertices2;
	for (int i = 0; i < childrenCount; i++)
	{
		FbxNode* childNode = Node->GetChild(i);
		FbxMesh* mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			//std::cout << "\nMesh:" << childNode->GetName();

			// Get index count from mesh
			numIndices = mesh->GetPolygonVertexCount();
			//std::cout << "\nIndice Count:" << numIndices;

			// No need to allocate int array, FBX does for us
			indices = mesh->GetPolygonVertices();

			// Get vertex count from mesh
			numVertices = mesh->GetControlPointsCount();
			//std::cout << "\nVertex Count:" << numVertices;

			// Create SimpleVertex array to size of this mesh
			vertices = new SimpleVertex[numVertices];




#pragma region ================= Process Vertices ===================
			for (int j = 0; j < numVertices; j++)
			{

				FbxVector4 vert = mesh->GetControlPointAt(j);

				vertices[j].Position.x = (float)vert.mData[0] / scale;
				vertices[j].Position.y = (float)vert.mData[1] / scale;
				vertices[j].Position.z = (float)vert.mData[2] / scale;

				// Get the Normals array from the mesh

			}


			FbxArray<FbxVector4> normalsVec;
			mesh->GetPolygonVertexNormals(normalsVec);
			//std::cout << "\nNormalVec Count:" << normalsVec.Size();
			// Declare a new array for the second vertex array
	// Note the size is numIndices not numVertices
			vertices2 = new SimpleVertex[numIndices];

			FbxStringList lUVSetNameList;
			mesh->GetUVSetNames(lUVSetNameList);
			const char* lUVSetName = lUVSetNameList.GetStringAt(0);
			const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;



			// align (expand) vertex array and set the normals and UVs
			for (int k = 0; k < numIndices; k++)
			{
				FbxVector2 lUVValue;

				vertices2[k] = vertices[indices[k]];
				vertices2[k].Normal.x = normalsVec[k].mData[0];
				vertices2[k].Normal.y = normalsVec[k].mData[1];
				vertices2[k].Normal.z = normalsVec[k].mData[2];

				int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(k) : k;
				lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

				vertices2[k].Tex.x = lUVValue[0];
				vertices2[k].Tex.y = lUVValue[1];
				vertices2[k].Tex.z = 0.0f;

			}
			// vertices is an "out" var so make sure it points to the new array
			// and clean up first array
			delete  vertices;
			vertices = vertices2;

			// make new indices to match the new vertex(2) array
			delete  indices;
			indices = new int[numIndices];
			for (int j = 0; j < numIndices; j++)
			{
				indices[j] = j;
			}
#pragma endregion

#pragma region =================== Texture ==================================

			//int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();
			int materialCount = childNode->GetMaterialCount();
			for (int index = 0; index < materialCount; index++)
			{
				material_t my_mat;
				FbxSurfaceMaterial* mat = childNode->GetMaterial(index);
				//Diffuse
				if (mat->Is<FbxSurfaceLambert>() == false)
					continue;

				FbxSurfaceLambert* lam = (FbxSurfaceLambert*)mat;
				FbxDouble3 diffuse_color = lam->Diffuse.Get();
				FbxDouble diffuse_factor = lam->DiffuseFactor.Get();

				my_mat[my_mat.DIFFUSE].value[0] = diffuse_color[0];
				my_mat[my_mat.DIFFUSE].value[1] = diffuse_color[1];
				my_mat[my_mat.DIFFUSE].value[2] = diffuse_color[2];

				my_mat[my_mat.DIFFUSE].factor = diffuse_factor;

				if (FbxFileTexture* file_texture = lam->Diffuse.GetSrcObject<FbxFileTexture>())
				{
					bool slash = false;
					string file_name = file_texture->GetRelativeFileName();
					file_path_t file_path;
					file_name.pop_back();
					file_name.pop_back();
					file_name.pop_back();
					file_name.append("dds");
					string path = matpath;
					string dds;
					for (int i = 0; i < file_name.size(); i++)
					{
						if (slash)
							dds.push_back(file_name[i]);
						if (file_name[i] == 92)
							slash = true;

					}

					file_name.clear();
					file_name.append(path);
					file_name.append(dds);
					strcpy(file_path.data(), file_name.c_str());

					my_mat[my_mat.DIFFUSE].input = paths.size();
					paths.push_back(file_path);


				}

#pragma region Emissive
				FbxDouble3 emissive_color = lam->Emissive.Get();
				FbxDouble emissive_factor = lam->EmissiveFactor.Get();

				my_mat[my_mat.EMISSIVE].value[0] = emissive_color[0];
				my_mat[my_mat.EMISSIVE].value[1] = emissive_color[1];
				my_mat[my_mat.EMISSIVE].value[2] = emissive_color[2];

				my_mat[my_mat.EMISSIVE].factor = emissive_factor;
				if (FbxFileTexture* file_texture = lam->Emissive.GetSrcObject<FbxFileTexture>())
				{
					bool slash = false;
					string file_name = file_texture->GetRelativeFileName();
					file_path_t file_path;
					file_name.pop_back();
					file_name.pop_back();
					file_name.pop_back();
					file_name.append("dds");
					string path = matpath;
					string dds;
					for (int i = 0; i < file_name.size(); i++)
					{
						if (slash)
							dds.push_back(file_name[i]);
						if (file_name[i] == 92)
							slash = true;

					}

					file_name.clear();
					file_name.append(path);
					file_name.append(dds);
					strcpy(file_path.data(), file_name.c_str());

					my_mat[my_mat.EMISSIVE].input = paths.size();
					paths.push_back(file_path);


				}
#pragma endregion

				//#pragma region Specular
				//
				//				if (mat->Is<FbxSurfacePhong>())
				//				{
				//					FbxSurfacePhong* spec = (FbxSurfacePhong*)mat;
				//					FbxDouble3 specular_color = spec->Specular.Get();
				//					FbxDouble specular_factor = spec->SpecularFactor.Get();
				//
				//					my_mat[my_mat.SPECULAR].value[0] = specular_color[0];
				//					my_mat[my_mat.SPECULAR].value[1] = specular_color[1];
				//					my_mat[my_mat.SPECULAR].value[2] = specular_color[2];
				//
				//					my_mat[my_mat.SPECULAR].factor = specular_factor;
				//					if (FbxFileTexture* file_texture = spec->Specular.GetSrcObject<FbxFileTexture>())
				//					{
				//						bool slash = false;
				//						string file_name = file_texture->GetRelativeFileName();
				//						file_path_t file_path;
				//						file_name.pop_back();
				//						file_name.pop_back();
				//						file_name.pop_back();
				//						file_name.append("dds");
				//						string path = matpath;
				//						string dds;
				//						for (int i = 0; i < file_name.size(); i++)
				//						{
				//							if (slash)
				//								dds.push_back(file_name[i]);
				//							if (file_name[i] == 92)
				//								slash = true;
				//
				//						}
				//
				//						file_name.clear();
				//						file_name.append(path);
				//						file_name.append(dds);
				//						strcpy(file_path.data(), file_name.c_str());
				//
				//						my_mat[my_mat.SPECULAR].input = paths.size();
				//						paths.push_back(file_path);
				//
				//
				//
				//
				//
				//
				//					}
				//
				//				}
				//#pragma endregion


			}
			if (matfile != "")
			{
				std::ofstream file(matfile, std::ios::trunc | std::ios::binary | std::ios::out);

				assert(file.is_open());

				uint32_t count = (uint32_t)paths.size();

				file.write((const char*)&count, sizeof(uint32_t));
				file.write((const char*)paths.data(), sizeof(file_path_t) * paths.size());

				file.close();
			}
#pragma endregion




			if (true)
			{
				Compactify(vertices, meshfile);

			}
			else
			{
				// numVertices is an "out" var so set to new size
				// this is used in the DrawIndexed functions to set the 
				// the right number of triangles
				numVertices = numIndices;

			}
		}
		ProcessFbxMesh(childNode, meshfile, matpath);
	}



};

void  Compactify(SimpleVertex* verticesCompact, const char* meshfile)
{

	float epsilon = 0.0001;
	bool Duplicate = false;
	int pos = 0;
	vector<SimpleVertex> vertexList;
	vector<int> indicesList;



	for (int i = 0; i < numIndices; i++)
	{



		Duplicate = false;

		for (int j = 0; j < vertexList.size(); j++)
		{


			if ((abs(verticesCompact[i].Position.x - vertexList[j].Position.x) < epsilon) &&
				(abs(verticesCompact[i].Position.y - vertexList[j].Position.y) < epsilon) &&
				(abs(verticesCompact[i].Position.z - vertexList[j].Position.z) < epsilon) &&
				(abs(verticesCompact[i].Normal.x - vertexList[j].Normal.x) < epsilon) &&
				(abs(verticesCompact[i].Normal.y - vertexList[j].Normal.y) < epsilon) &&
				(abs(verticesCompact[i].Normal.z - vertexList[j].Normal.z) < epsilon) &&
				(abs(verticesCompact[i].Tex.x - vertexList[j].Tex.x) < epsilon) &&
				(abs(verticesCompact[i].Tex.y - vertexList[j].Tex.y) < epsilon) &&
				(abs(verticesCompact[i].Tex.z - vertexList[j].Tex.z) < epsilon)) {
				Duplicate = true;
				indices[i] = j;
				break;
			}
		}



		if (Duplicate == false)
		{
			vertexList.push_back(verticesCompact[i]);
			indices[i] = vertexList.size() - 1;


		}



	}
	for (int i = 0; i < vertexList.size(); i++)
	{
		verticesCompact[i] = vertexList[i];



	}
	for (int i = 0; i < numIndices; i++)
	{
		indicesList.push_back(indices[i]);
	}

	SimpleMesh GenerateTangents;
	GenerateTangents.vertexList = vertexList;
	GenerateTangents.indicesList = indicesList;
	ComputeTangent(GenerateTangents);
	vertexList = GenerateTangents.vertexList;
	indicesList = GenerateTangents.indicesList;

	// print out some stats
	//std::cout << "\nindex count BEFORE/AFTER compaction " << numIndices;
	//std::cout << "\nvertex count ORIGINAL (FBX source): " << numVertices;
	//std::cout << "\nvertex count AFTER expansion: " << numIndices;
	//std::cout << "\nvertex count AFTER compaction: " << vertexList.size();
	//std::cout << "\nSize reduction: " << ((numVertices - vertexList.size()) / (float)numVertices) * 100.00f << "%";
	//std::cout << "\nor " << (vertexList.size() / (float)numVertices) << " of the expanded size";

	//write out Mesh
	const char* path = meshfile;
	std::ofstream file(path, std::ios::trunc | std::ios::binary | std::ios::out);

	assert(file.is_open());


	uint32_t index_count = (uint32_t)indicesList.size();
	uint32_t vert_count = (uint32_t)vertexList.size();

	file.write((const char*)&index_count, sizeof(uint32_t));
	file.write((const char*)indicesList.data(), sizeof(uint32_t) * indicesList.size());
	file.write((const char*)&vert_count, sizeof(uint32_t));
	file.write((const char*)vertexList.data(), sizeof(SimpleVertex) * vertexList.size());

	file.close();


};
void  ComputeTangent(SimpleMesh& simpleMesh)
{
	//////////////////////Compute Normals///////////////////////////
	//If computeNormals was set to true then we will create our own
	//normals, if it was set to false we will use the obj files normals
	std::vector<XMFLOAT3> tempNormal;

	//normalized and unnormalized normals
	XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

	///////////////**************new**************////////////////////
	//tangent stuff
	std::vector<XMFLOAT3> tempTangent;
	XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float tcU1, tcV1, tcU2, tcV2;
	///////////////**************new**************////////////////////

	//Used to get vectors (sides) from the position of the verts
	float vecX, vecY, vecZ;

	//Two edges of our triangle
	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//Compute face normals
	//And Tangents
	for (int i = 0; i < simpleMesh.indicesList.size() / 3; ++i)
	{
		//Get the vector describing one edge of our triangle (edge 0,2)
		vecX = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Position.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.x;
		vecY = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Position.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.y;
		vecZ = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Position.z - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.z;
		edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

		//Get the vector describing another edge of our triangle (edge 2,1)
		vecX = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Position.x;
		vecY = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Position.y;
		vecZ = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.z - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Position.z;
		edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge

		//Cross multiply the two edge vectors to get the un-normalized face normal
		XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

		tempNormal.push_back(unnormalized);

		///////////////**************new**************////////////////////
		//Find first texture coordinate edge 2d vector
		tcU1 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Tex.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.x;
		tcV1 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Tex.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.y;

		//Find second texture coordinate edge 2d vector
		tcU2 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Tex.x;
		tcV2 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Tex.y;

		//Find tangent using both tex coord edges and position edges
		tangent.x = (tcV1 * XMVectorGetX(edge1) - tcV2 * XMVectorGetX(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
		tangent.y = (tcV1 * XMVectorGetY(edge1) - tcV2 * XMVectorGetY(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
		tangent.z = (tcV1 * XMVectorGetZ(edge1) - tcV2 * XMVectorGetZ(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));

		tempTangent.push_back(tangent);
		///////////////**************new**************////////////////////
	}

	//Compute vertex normals (normal Averaging)
	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int facesUsing = 0;
	float tX, tY, tZ;    //temp axis variables

	//Go through each vertex
	for (int i = 0; i < simpleMesh.vertexList.size(); ++i)
	{
		//Check which triangles use this vertex
		for (int j = 0; j < simpleMesh.indicesList.size() / 3; ++j)
		{
			if (simpleMesh.indicesList[j * 3] == i ||
				simpleMesh.indicesList[(j * 3) + 1] == i ||
				simpleMesh.indicesList[(j * 3) + 2] == i)
			{
				tX = XMVectorGetX(normalSum) + tempNormal[j].x;
				tY = XMVectorGetY(normalSum) + tempNormal[j].y;
				tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

				normalSum = XMVectorSet(tX, tY, tZ, 0.0f);    //If a face is using the vertex, add the unormalized face normal to the normalSum

				///////////////**************new**************////////////////////        
				//We can reuse tX, tY, tZ to sum up tangents
				tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
				tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
				tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

				tangentSum = XMVectorSet(tX, tY, tZ, 0.0f); //sum up face tangents using this vertex
				///////////////**************new**************////////////////////

				facesUsing++;
			}
		}

		//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
		normalSum = normalSum / facesUsing;
		///////////////**************new**************////////////////////
		tangentSum = tangentSum / facesUsing;
		///////////////**************new**************////////////////////

		//Normalize the normalSum vector and tangent
		normalSum = XMVector3Normalize(normalSum);
		///////////////**************new**************////////////////////
		tangentSum = XMVector3Normalize(tangentSum);
		///////////////**************new**************////////////////////

		//Store the normal and tangent in our current vertex
		simpleMesh.vertexList[i].Normal.x = XMVectorGetX(normalSum);
		simpleMesh.vertexList[i].Normal.y = XMVectorGetY(normalSum);
		simpleMesh.vertexList[i].Normal.z = XMVectorGetZ(normalSum);

		///////////////**************new**************////////////////////
		simpleMesh.vertexList[i].Tangent.x = XMVectorGetX(tangentSum);
		simpleMesh.vertexList[i].Tangent.y = XMVectorGetY(tangentSum);
		simpleMesh.vertexList[i].Tangent.z = XMVectorGetZ(tangentSum);
		///////////////**************new**************////////////////////

		//Clear normalSum, tangentSum and facesUsing for next vertex
		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		///////////////**************new**************////////////////////
		tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		///////////////**************new**************////////////////////
		facesUsing = 0;

	}
};
void  ComputeTangent(SimpleMeshAnim& simpleMesh)
{
	//////////////////////Compute Normals///////////////////////////
	//If computeNormals was set to true then we will create our own
	//normals, if it was set to false we will use the obj files normals
	std::vector<XMFLOAT3> tempNormal;

	//normalized and unnormalized normals
	XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

	///////////////**************new**************////////////////////
	//tangent stuff
	std::vector<XMFLOAT3> tempTangent;
	XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float tcU1, tcV1, tcU2, tcV2;
	///////////////**************new**************////////////////////

	//Used to get vectors (sides) from the position of the verts
	float vecX, vecY, vecZ;

	//Two edges of our triangle
	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//Compute face normals
	//And Tangents
	for (int i = 0; i < simpleMesh.indicesList.size() / 3; ++i)
	{
		//Get the vector describing one edge of our triangle (edge 0,2)
		vecX = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Position.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.x;
		vecY = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Position.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.y;
		vecZ = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Position.z - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.z;
		edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

		//Get the vector describing another edge of our triangle (edge 2,1)
		vecX = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Position.x;
		vecY = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Position.y;
		vecZ = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Position.z - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Position.z;
		edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge

		//Cross multiply the two edge vectors to get the un-normalized face normal
		XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

		tempNormal.push_back(unnormalized);

		///////////////**************new**************////////////////////
		//Find first texture coordinate edge 2d vector
		tcU1 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Tex.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.x;
		tcV1 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3)]].Tex.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.y;

		//Find second texture coordinate edge 2d vector
		tcU2 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.x - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Tex.x;
		tcV2 = simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 2]].Tex.y - simpleMesh.vertexList[simpleMesh.indicesList[(i * 3) + 1]].Tex.y;

		//Find tangent using both tex coord edges and position edges
		tangent.x = (tcV1 * XMVectorGetX(edge1) - tcV2 * XMVectorGetX(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
		tangent.y = (tcV1 * XMVectorGetY(edge1) - tcV2 * XMVectorGetY(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
		tangent.z = (tcV1 * XMVectorGetZ(edge1) - tcV2 * XMVectorGetZ(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));

		tempTangent.push_back(tangent);
		///////////////**************new**************////////////////////
	}

	//Compute vertex normals (normal Averaging)
	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int facesUsing = 0;
	float tX, tY, tZ;    //temp axis variables

	//Go through each vertex
	for (int i = 0; i < simpleMesh.vertexList.size(); ++i)
	{
		//Check which triangles use this vertex
		for (int j = 0; j < simpleMesh.indicesList.size() / 3; ++j)
		{
			if (simpleMesh.indicesList[j * 3] == i ||
				simpleMesh.indicesList[(j * 3) + 1] == i ||
				simpleMesh.indicesList[(j * 3) + 2] == i)
			{
				tX = XMVectorGetX(normalSum) + tempNormal[j].x;
				tY = XMVectorGetY(normalSum) + tempNormal[j].y;
				tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

				normalSum = XMVectorSet(tX, tY, tZ, 0.0f);    //If a face is using the vertex, add the unormalized face normal to the normalSum

				///////////////**************new**************////////////////////        
				//We can reuse tX, tY, tZ to sum up tangents
				tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
				tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
				tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

				tangentSum = XMVectorSet(tX, tY, tZ, 0.0f); //sum up face tangents using this vertex
				///////////////**************new**************////////////////////

				facesUsing++;
			}
		}

		//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
		normalSum = normalSum / facesUsing;
		///////////////**************new**************////////////////////
		tangentSum = tangentSum / facesUsing;
		///////////////**************new**************////////////////////

		//Normalize the normalSum vector and tangent
		normalSum = XMVector3Normalize(normalSum);
		///////////////**************new**************////////////////////
		tangentSum = XMVector3Normalize(tangentSum);
		///////////////**************new**************////////////////////

		//Store the normal and tangent in our current vertex
		simpleMesh.vertexList[i].Normal.x = XMVectorGetX(normalSum);
		simpleMesh.vertexList[i].Normal.y = XMVectorGetY(normalSum);
		simpleMesh.vertexList[i].Normal.z = XMVectorGetZ(normalSum);

		///////////////**************new**************////////////////////
		simpleMesh.vertexList[i].Tangent.x = XMVectorGetX(tangentSum);
		simpleMesh.vertexList[i].Tangent.y = XMVectorGetY(tangentSum);
		simpleMesh.vertexList[i].Tangent.z = XMVectorGetZ(tangentSum);
		///////////////**************new**************////////////////////

		//Clear normalSum, tangentSum and facesUsing for next vertex
		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		///////////////**************new**************////////////////////
		tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		///////////////**************new**************////////////////////
		facesUsing = 0;

	}
};

void  Load_Mesh(const char* meshFileName, SimpleMesh& mesh)
{

	std::fstream file{ meshFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t player_index_count;
	file.read((char*)&player_index_count, sizeof(uint32_t));

	mesh.indicesList.resize(player_index_count);

	file.read((char*)mesh.indicesList.data(), sizeof(uint32_t) * player_index_count);

	uint32_t player_vertex_count;
	file.read((char*)&player_vertex_count, sizeof(uint32_t));

	mesh.vertexList.resize(player_vertex_count);

	file.read((char*)mesh.vertexList.data(), sizeof(SimpleVertex) * player_vertex_count);

	//Example mesh conditioning if needed - this flips handedness
	for (auto& v : mesh.vertexList)
	{
		v.Position.x = -v.Position.x;
		v.Normal.x = -v.Normal.x;
		v.Tex.y = 1.0f - v.Tex.y;

	}

	int tri_count = (int)(mesh.indicesList.size() / 3);

	for (int i = 0; i < tri_count; ++i)
	{
		int* tri = mesh.indicesList.data() + i * 3;

		int temp = tri[0];
		tri[0] = tri[2];
		tri[2] = temp;
	}
	file.close();
};

//Animation
struct fbx_joint
{
	FbxNode* node; XMMATRIX global_xform; int parent_index; int childCount;

};
struct bones
{
	 XMMATRIX global_xform; int parent_index; int childCount;

};
vector<bones> jointData;


struct keyframe
{
	double time; vector<XMMATRIX> jointsMatrix; vector<int> parents;
};
struct anim_clip
{
	double duration;
	vector<keyframe> frames;
};
struct vert_pos_skinned
{
	XMVECTOR pos;
	int joints[4];
	XMVECTOR weights;


};
vector<XMMATRIX> InverseJoints;
const int MAX_INFLUENCES = 4;
struct influence
{
	int joint; float weight;
};
void ProcessFbxMeshAnim(FbxNode* Node, const char* meshfile, const char* matpath = "", const char* matfile = "");
void GetClusters(FbxSkin* skin);
void Anim_FBX_InitLoad(const char* fbxfile, const char* meshfile, const char* animFile, const char* matPath = "", const char* matfile = "");
void CompactifyAnim(SimpleVertexAnim* verticesCompact, const char* meshfile);
void WriteOutAnimationData(const char* AnimFile);

vector<fbx_joint> joints;
SimpleVertexAnim* verticesAnim;

using influence_set = array<influence, MAX_INFLUENCES>;
vector<influence_set> control_point_influences;

anim_clip out_clip;
//void ProcessFbxSkeleton(FbxNode* Node, int parentindex)
//{
//	int childCount = Node->GetChildCount();
//	XMMATRIX m = FBXAMatrix_To_XMMATRIX((Node->EvaluateGlobalTransform()));
//	fbx_joint joint = { Node, m, parentindex,  childCount };
//	joints.push_back(joint);
//	XMFLOAT4X4 mat;
//	FbxAMatrix matrix = Node->EvaluateGlobalTransform().Inverse();
//	for (unsigned int k = 0; k < 4; k++)
//	{
//		for (unsigned int l = 0; l < 4; l++)
//		{
//			mat.m[k][l] = matrix.mData[k].Buffer()[l];
//		}
//	}
//
//	XMMATRIX preComp = XMLoadFloat4x4(&mat);
//	InverseJoints.push_back(preComp);
//	int jointCount = joints.size();
//	for (int i = 0; i < childCount; i++)
//	{
//		FbxNode* childNode = Node->GetChild(i);
//		FbxSkeleton* skeleton = childNode->GetSkeleton();
//		if (skeleton != nullptr)
//		{
//
//			ProcessFbxSkeleton(childNode, jointCount - 1);
//
//		}
//	}
//
//
//}
void GetChildJoints()
{
	for (int i = 0; i < joints.size(); ++i)
	{
		for (int j = 0; j < joints[i].node->GetChildCount(); ++j)
		{
			FbxNode* child = joints[i].node->GetChild(j);
			FbxSkeleton* childSkeleton = child->GetSkeleton();
			if (childSkeleton != nullptr)
			{
				fbx_joint childJoint;
				childJoint.node = child;
				FbxAMatrix matrix = child->EvaluateGlobalTransform();
				XMFLOAT4X4 mat;
				for (unsigned int k = 0; k < 4; k++)
				{
					for (unsigned int l = 0; l < 4; l++)
					{
						mat.m[k][l] = matrix.mData[k].Buffer()[l];
					}
				}
				childJoint.global_xform = XMLoadFloat4x4(&mat);

				childJoint.parent_index = i;
				joints.push_back(childJoint);

				XMFLOAT4X4 inv_mat;
				FbxAMatrix inverse_matrix = child->EvaluateGlobalTransform().Inverse();
				
				for (unsigned int k = 0; k < 4; k++)
				{
					for (unsigned int l = 0; l < 4; l++)
					{
						inv_mat.m[k][l] = matrix.mData[k].Buffer()[l];
					}
				}
				XMMATRIX preComp = XMLoadFloat4x4(&inv_mat);
				InverseJoints.push_back(preComp);
			}
		}
	}
}

void Anim_FBX_InitLoad(const char* fbxfile, const char* meshfile, const char* animFile, const char* matPath, const char* matfile)
{
	// Change the following filename to a suitable filename value.
	const char* lFilename = fbxfile;

	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();

	int childrenCount = lScene->GetRootNode()->GetChildCount();
	FbxNode* Node = lScene->GetRootNode();
	for (int i = 0; i < childrenCount; i++)
	{
		FbxNode* childNode = Node->GetChild(i);
		FbxMesh* mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			// Get vertex count from mesh
			numVertices = mesh->GetControlPointsCount();
			control_point_influences.resize(numVertices);
		}
	}

	//Skeleton
	for (unsigned int j = 0; j < lScene->GetPoseCount(); j++)
	{


		FbxPose* bindPose = lScene->GetPose(j);
		if (bindPose->IsBindPose())
		{
			for (unsigned int i = 0; i < bindPose->GetCount(); i++)
			{


				FbxNode* skeletonNode = bindPose->GetNode(i);

				FbxSkeleton* skeleton = skeletonNode->GetSkeleton();
				if (skeleton != NULL)
				{
					if (skeleton->IsSkeletonRoot())
					{
						fbx_joint rootJoint;
						rootJoint.node = skeletonNode;
						FbxAMatrix matrix = skeletonNode->EvaluateGlobalTransform();
						XMFLOAT4X4 mat;
						for (unsigned int k = 0; k < 4; k++)
						{
							for (unsigned int l = 0; l < 4; l++)
							{
								mat.m[k][l] = matrix.mData[k].Buffer()[l];
							}
						}

						rootJoint.global_xform = XMLoadFloat4x4(&mat);
						rootJoint.parent_index = -1;
						joints.push_back(rootJoint);
						XMFLOAT4X4 inv_mat;
						FbxAMatrix inv_matrix = skeletonNode->EvaluateGlobalTransform().Inverse();
						for (unsigned int k = 0; k < 4; k++)
						{
							for (unsigned int l = 0; l < 4; l++)
							{
								inv_mat.m[k][l] = inv_matrix.mData[k].Buffer()[l];
							}
						}

						XMMATRIX preComp = XMLoadFloat4x4(&inv_mat);
						InverseJoints.push_back(preComp);
						GetChildJoints();
					}
				}
			}
		}
	}
	//Animate Model
	int num_of_control_points = 0;
	// Weights
	for (int i = 0; i < lScene->GetPoseCount(); i++)
	{
		FbxPose* pose = lScene->GetPose(i);

		if (pose->IsBindPose())
		{
			int node_count = pose->GetCount();

			FbxNode* node = nullptr;
			FbxMesh* mesh = nullptr;
			for (int j = 0; j < node_count; j++)
			{
				node = pose->GetNode(j);

				if (node)
				{
					mesh = node->GetMesh();

					if (mesh)
					{
						int deformer_count = mesh->GetDeformerCount();

						for (int k = 0; k < deformer_count; k++)
						{
							FbxDeformer* deformer = mesh->GetDeformer(k);
							if (deformer->Is<FbxSkin>())
							{
								FbxSkin* skin = FbxCast<FbxSkin>(deformer);
								GetClusters(skin);
							}
						}
					}
				}
			}
		}
	}	FbxAnimStack* stack = lScene->GetCurrentAnimationStack();
	FbxTimeSpan timeSpan = stack->GetLocalTimeSpan();
	FbxTime duration = timeSpan.GetDuration();
	out_clip.duration = duration.Get();
	FbxTime temp = duration;
	FbxLongLong frames = duration.GetFrameCount(duration.eFrames24);
	for (unsigned int i = 0; i < frames; i++)
	{
		temp.SetFrame(i, duration.eFrames24);

		keyframe key;
		key.time = temp.Get();
		for (unsigned int j = 0; j < joints.size(); j++)
		{
			XMFLOAT4X4 mat;
			FbxAMatrix matrix = joints[j].node->EvaluateGlobalTransform(key.time);
			for (unsigned int k = 0; k < 4; k++)
			{
				for (unsigned int l = 0; l < 4; l++)
				{
					mat.m[k][l] = matrix.mData[k].Buffer()[l];
				}
			}
		
			key.jointsMatrix.push_back(XMLoadFloat4x4(&mat));
			key.parents.push_back(joints[j].parent_index);
		}
		out_clip.frames.push_back(key);

	}
	for (int i = 0; i < InverseJoints.size(); i++)
	{
		InverseJoints[i] = XMMatrixInverse(nullptr, InverseJoints[i]);
	}

	ProcessFbxMeshAnim(lScene->GetRootNode(), meshfile, matPath, matfile);

	WriteOutAnimationData(animFile);
}
void GetClusters(FbxSkin* skin)
{

	int c = skin->GetClusterCount();
	for (unsigned int i = 0; i < skin->GetClusterCount(); i++)
	{
		int joint_index = -1;
		FbxCluster* cluster = skin->GetCluster(i);
		FbxNode* node = cluster->GetLink();

		for (unsigned int j = 0; j < joints.size(); j++)
		{
			if (node == joints[j].node)
			{
				joint_index = j;
				break;
			}
		}

		assert(joint_index != -1);

		int cpi_count = cluster->GetControlPointIndicesCount();

		auto cpi = cluster->GetControlPointIndices();

		auto weights = cluster->GetControlPointWeights();

		for (unsigned int k = 0; k < cpi_count; k++)
		{
			int index = cpi[k];

			influence_set* influenceSet = &control_point_influences[index];
			double weight = weights[k];
			influence temp = influence({ joint_index, (float)weight });

			for (auto inf = influenceSet->begin(); inf != influenceSet->end(); inf++)
			{
				if (inf->weight < temp.weight)
				{
					std::swap(temp, *inf);
				}
			}
		}

	}
};
void ProcessFbxMeshAnim(FbxNode* Node, const char* meshfile, const char* matpath, const char* matfile)
{
	// set up output console
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	//FBX Mesh stuff
	int childrenCount = Node->GetChildCount();

	//std::cout << "\nName:" << Node->GetName();
	SimpleVertexAnim* vertices2;
	for (int i = 0; i < childrenCount; i++)
	{
		FbxNode* childNode = Node->GetChild(i);
		FbxMesh* mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			//std::cout << "\nMesh:" << childNode->GetName();

			// Get index count from mesh
			numIndices = mesh->GetPolygonVertexCount();
			//std::cout << "\nIndice Count:" << numIndices;

			// No need to allocate int array, FBX does for us
			indices = mesh->GetPolygonVertices();

			// Get vertex count from mesh
			numVertices = mesh->GetControlPointsCount();
			//std::cout << "\nVertex Count:" << numVertices;

			// Create SimpleVertex array to size of this mesh
			verticesAnim = new SimpleVertexAnim[numVertices];

			control_point_influences.resize(numVertices);



#pragma region ================= Process Vertices ===================
			for (int j = 0; j < numVertices; j++)
			{

				FbxVector4 vert = mesh->GetControlPointAt(j);
				verticesAnim[j].Position.x = (float)vert.mData[0];
				verticesAnim[j].Position.y = (float)vert.mData[1];
				verticesAnim[j].Position.z = (float)vert.mData[2];
				//vertices[j].Color = RAND_COLOR;
				// Generate random normal for first attempt at getting to render
				verticesAnim[j].Tex.x = verticesAnim[j].Position.x;
				verticesAnim[j].Tex.y = verticesAnim[j].Position.y;

				verticesAnim[j].Joint.x = control_point_influences[j][0].joint;
				verticesAnim[j].Joint.y = control_point_influences[j][1].joint;
				verticesAnim[j].Joint.z = control_point_influences[j][2].joint;
				verticesAnim[j].Joint.w = control_point_influences[j][3].joint;

				verticesAnim[j].Weight.x = control_point_influences[j][0].weight;
				verticesAnim[j].Weight.y = control_point_influences[j][1].weight;
				verticesAnim[j].Weight.z = control_point_influences[j][2].weight;
				verticesAnim[j].Weight.w = control_point_influences[j][3].weight;

				float sum = verticesAnim[j].Weight.x + verticesAnim[j].Weight.y + verticesAnim[j].Weight.z + verticesAnim[j].Weight.w;

				verticesAnim[j].Weight.x /= sum;
				verticesAnim[j].Weight.y /= sum;
				verticesAnim[j].Weight.z /= sum;
				verticesAnim[j].Weight.w /= sum;
			}


			FbxArray<FbxVector4> normalsVec;
			mesh->GetPolygonVertexNormals(normalsVec);
			//std::cout << "\nNormalVec Count:" << normalsVec.Size();
			// Declare a new array for the second vertex array
	// Note the size is numIndices not numVertices
			vertices2 = new SimpleVertexAnim[numIndices];

			FbxStringList lUVSetNameList;
			mesh->GetUVSetNames(lUVSetNameList);
			const char* lUVSetName = lUVSetNameList.GetStringAt(0);
			const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;



			// align (expand) vertex array and set the normals and UVs
			for (int k = 0; k < numIndices; k++)
			{
				FbxVector2 lUVValue;

				vertices2[k] = verticesAnim[indices[k]];
				vertices2[k].Normal.x = normalsVec[k].mData[0];
				vertices2[k].Normal.y = normalsVec[k].mData[1];
				vertices2[k].Normal.z = normalsVec[k].mData[2];

				vertices2[k].Joint.x = verticesAnim[indices[k]].Joint.x;
				vertices2[k].Joint.y = verticesAnim[indices[k]].Joint.y;
				vertices2[k].Joint.z = verticesAnim[indices[k]].Joint.z;
				vertices2[k].Joint.w = verticesAnim[indices[k]].Joint.w;

				vertices2[k].Weight.x = verticesAnim[indices[k]].Weight.x;
				vertices2[k].Weight.y = verticesAnim[indices[k]].Weight.y;
				vertices2[k].Weight.z = verticesAnim[indices[k]].Weight.z;
				vertices2[k].Weight.w = verticesAnim[indices[k]].Weight.w;

				int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(k) : k;
				lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

				vertices2[k].Tex.x = lUVValue[0];
				vertices2[k].Tex.y = lUVValue[1];
				vertices2[k].Tex.z = 0.0f;





			}
			// vertices is an "out" var so make sure it points to the new array
			// and clean up first array
			delete verticesAnim;
			verticesAnim = vertices2;

			// make new indices to match the new vertex(2) array
			delete indices;
			indices = new int[numIndices];
			for (int j = 0; j < numIndices; j++)
			{
				indices[j] = j;
			}
#pragma endregion

#pragma region =================== Texture ==================================

			//int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();
			int materialCount = childNode->GetMaterialCount();
			for (int index = 0; index < materialCount; index++)
			{
				material_t my_mat;
				FbxSurfaceMaterial* mat = childNode->GetMaterial(index);
				//Diffuse
				if (mat->Is<FbxSurfaceLambert>() == false)
					continue;

				FbxSurfaceLambert* lam = (FbxSurfaceLambert*)mat;
				FbxDouble3 diffuse_color = lam->Diffuse.Get();
				FbxDouble diffuse_factor = lam->DiffuseFactor.Get();

				my_mat[my_mat.DIFFUSE].value[0] = diffuse_color[0];
				my_mat[my_mat.DIFFUSE].value[1] = diffuse_color[1];
				my_mat[my_mat.DIFFUSE].value[2] = diffuse_color[2];

				my_mat[my_mat.DIFFUSE].factor = diffuse_factor;

				if (FbxFileTexture* file_texture = lam->Diffuse.GetSrcObject<FbxFileTexture>())
				{
					bool slash = false;
					string file_name = file_texture->GetRelativeFileName();
					file_path_t file_path;
					file_name.pop_back();
					file_name.pop_back();
					file_name.pop_back();
					file_name.append("dds");
					string path = matpath;
					string dds;
					for (int i = 0; i < file_name.size(); i++)
					{
						if (slash)
							dds.push_back(file_name[i]);
						if (file_name[i] == 92)
							slash = true;

					}

					file_name.clear();
					file_name.append(path);
					file_name.append(dds);
					strcpy(file_path.data(), file_name.c_str());

					my_mat[my_mat.DIFFUSE].input = paths.size();
					paths.push_back(file_path);


				}

#pragma region Emissive
				FbxDouble3 emissive_color = lam->Emissive.Get();
				FbxDouble emissive_factor = lam->EmissiveFactor.Get();

				my_mat[my_mat.EMISSIVE].value[0] = emissive_color[0];
				my_mat[my_mat.EMISSIVE].value[1] = emissive_color[1];
				my_mat[my_mat.EMISSIVE].value[2] = emissive_color[2];

				my_mat[my_mat.EMISSIVE].factor = emissive_factor;
				if (FbxFileTexture* file_texture = lam->Emissive.GetSrcObject<FbxFileTexture>())
				{
					bool slash = false;
					string file_name = file_texture->GetRelativeFileName();
					file_path_t file_path;
					file_name.pop_back();
					file_name.pop_back();
					file_name.pop_back();
					file_name.append("dds");
					string path = matpath;
					string dds;
					for (int i = 0; i < file_name.size(); i++)
					{
						if (slash)
							dds.push_back(file_name[i]);
						if (file_name[i] == 92)
							slash = true;

					}

					file_name.clear();
					file_name.append(path);
					file_name.append(dds);
					strcpy(file_path.data(), file_name.c_str());

					my_mat[my_mat.EMISSIVE].input = paths.size();
					paths.push_back(file_path);


				}
#pragma endregion

				//#pragma region Specular
				//
				//				if (mat->Is<FbxSurfacePhong>())
				//				{
				//					FbxSurfacePhong* spec = (FbxSurfacePhong*)mat;
				//					FbxDouble3 specular_color = spec->Specular.Get();
				//					FbxDouble specular_factor = spec->SpecularFactor.Get();
				//
				//					my_mat[my_mat.SPECULAR].value[0] = specular_color[0];
				//					my_mat[my_mat.SPECULAR].value[1] = specular_color[1];
				//					my_mat[my_mat.SPECULAR].value[2] = specular_color[2];
				//
				//					my_mat[my_mat.SPECULAR].factor = specular_factor;
				//					if (FbxFileTexture* file_texture = spec->Specular.GetSrcObject<FbxFileTexture>())
				//					{
				//						bool slash = false;
				//						string file_name = file_texture->GetRelativeFileName();
				//						file_path_t file_path;
				//						file_name.pop_back();
				//						file_name.pop_back();
				//						file_name.pop_back();
				//						file_name.append("dds");
				//						string path = matpath;
				//						string dds;
				//						for (int i = 0; i < file_name.size(); i++)
				//						{
				//							if (slash)
				//								dds.push_back(file_name[i]);
				//							if (file_name[i] == 92)
				//								slash = true;
				//
				//						}
				//
				//						file_name.clear();
				//						file_name.append(path);
				//						file_name.append(dds);
				//						strcpy(file_path.data(), file_name.c_str());
				//
				//						my_mat[my_mat.SPECULAR].input = paths.size();
				//						paths.push_back(file_path);
				//
				//
				//
				//
				//
				//
				//					}
				//
				//				}
				//#pragma endregion


			}
			if (matfile != "")
			{
				std::ofstream file(matfile, std::ios::trunc | std::ios::binary | std::ios::out);

				assert(file.is_open());

				uint32_t count = (uint32_t)paths.size();

				file.write((const char*)&count, sizeof(uint32_t));
				file.write((const char*)paths.data(), sizeof(file_path_t) * paths.size());

				file.close();
			}
#pragma endregion




			if (true)
			{
				CompactifyAnim(verticesAnim, meshfile);

			}
			else
			{
				// numVertices is an "out" var so set to new size
				// this is used in the DrawIndexed functions to set the 
				// the right number of triangles
				numVertices = numIndices;

			}
		}
		ProcessFbxMeshAnim(childNode, meshfile, matpath);
	}



}

void Load_AnimMesh(const char* meshFileName, SimpleMeshAnim& mesh)
{

	std::fstream file{ meshFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t player_index_count;
	file.read((char*)&player_index_count, sizeof(uint32_t));

	mesh.indicesList.resize(player_index_count);

	file.read((char*)mesh.indicesList.data(), sizeof(uint32_t) * player_index_count);

	uint32_t player_vertex_count;
	file.read((char*)&player_vertex_count, sizeof(uint32_t));

	mesh.vertexList.resize(player_vertex_count);

	file.read((char*)mesh.vertexList.data(), sizeof(SimpleVertexAnim) * player_vertex_count);

	//Example mesh conditioning if needed - this flips handedness
	for (auto& v : mesh.vertexList)
	{
		v.Position.x = -v.Position.x;
		v.Normal.x = -v.Normal.x;
		v.Tex.y = 1.0f - v.Tex.y;

	}

	int tri_count = (int)(mesh.indicesList.size() / 3);

	for (int i = 0; i < tri_count; ++i)
	{
		int* tri = mesh.indicesList.data() + i * 3;

		int temp = tri[0];
		tri[0] = tri[2];
		tri[2] = temp;
	}
	file.close();
};

void WriteOutAnimationData(const char* AnimFile)
{
	const char* path = AnimFile;
	std::ofstream file(path, std::ios::trunc | std::ios::binary | std::ios::out);

	assert(file.is_open());
	jointData.resize(joints.size());
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		jointData[i].childCount = joints[i].childCount;
		jointData[i].global_xform = joints[i].global_xform;
		jointData[i].parent_index = joints[i].parent_index;

	
	}
	
	uint32_t joint_count = (uint32_t)jointData.size();
	file.write((const char*)&joint_count, sizeof(uint32_t));
	uint32_t inv_joint_count = (uint32_t)InverseJoints.size();
	file.write((const char*)&inv_joint_count, sizeof(uint32_t));


	double clip_duration = out_clip.duration;
	file.write((const char*)&clip_duration, sizeof(double));

	uint32_t key_joint_count = (uint32_t)out_clip.frames.size();
	file.write((const char*)&key_joint_count, sizeof(uint32_t));

	

	for (unsigned int i = 0; i < key_joint_count; i++)
	{
		uint32_t matrix_joint_count = (uint32_t)out_clip.frames[i].jointsMatrix.size();
		file.write((const char*)&matrix_joint_count, sizeof(uint32_t));
		file.write((const char*)&out_clip.frames[i].time, sizeof(double));
		uint32_t parents_joint_count = (uint32_t)out_clip.frames[i].parents.size();
		file.write((const char*)&parents_joint_count, sizeof(uint32_t));

		file.write((const char*)out_clip.frames[i].parents.data(), sizeof(uint32_t) * out_clip.frames[i].parents.size());

		file.write((const char*)out_clip.frames[i].jointsMatrix.data(), sizeof(XMMATRIX) * out_clip.frames[i].jointsMatrix.size());

	}

	file.write((const char*)jointData.data(), sizeof(bones) * jointData.size());


	file.write((const char*)InverseJoints.data(), sizeof(XMMATRIX) * InverseJoints.size());







	file.close();



}
void Load_AnimFile(const char* animFileName, vector<bones>& skeleton, vector<XMMATRIX>& inverse, anim_clip& clip)
{

	std::fstream file{ animFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t joint_Count;
	file.read((char*)&joint_Count, sizeof(uint32_t));
	uint32_t inv_joint_count;
	file.read((char*)&inv_joint_count, sizeof(uint32_t));

	double duration;
	file.read((char*)&duration, sizeof(double));
	clip.duration = duration;
	uint32_t key_joint_count;
	file.read((char*)&key_joint_count, sizeof(uint32_t));

	clip.frames.resize(key_joint_count);


	for (unsigned int i = 0; i < key_joint_count; i++)
	{

		uint32_t matrix_joint_count;
		file.read((char*)&matrix_joint_count, sizeof(uint32_t));
		clip.frames[i].jointsMatrix.resize(matrix_joint_count);
		file.read((char*)&clip.frames[i].time, sizeof(double));
		uint32_t parents_joint_count;
		file.read((char*)&parents_joint_count, sizeof(uint32_t));
		clip.frames[i].parents.resize(parents_joint_count);


		file.read((char*)clip.frames[i].parents.data(), sizeof(int) * parents_joint_count);

		file.read((char*)clip.frames[i].jointsMatrix.data(), sizeof(XMMATRIX) * matrix_joint_count);


	}
	skeleton.resize(joint_Count);
	file.read((char*)skeleton.data(), sizeof(bones) * joint_Count);

	inverse.resize(inv_joint_count);
	file.read((char*)inverse.data(), sizeof(XMMATRIX) * inv_joint_count);



	file.close();
};
void CompactifyAnim(SimpleVertexAnim* verticesCompact, const char* meshfile)
{

	float epsilon = 0.0001;
	bool Duplicate = false;
	int pos = 0;
	vector<SimpleVertexAnim> vertexList;
	vector<int> indicesList;



	for (int i = 0; i < numIndices; i++)
	{



		Duplicate = false;

		for (int j = 0; j < vertexList.size(); j++)
		{


			if ((abs(verticesCompact[i].Position.x - vertexList[j].Position.x) < epsilon) &&
				(abs(verticesCompact[i].Position.y - vertexList[j].Position.y) < epsilon) &&
				(abs(verticesCompact[i].Position.z - vertexList[j].Position.z) < epsilon) &&
				(abs(verticesCompact[i].Normal.x - vertexList[j].Normal.x) < epsilon) &&
				(abs(verticesCompact[i].Normal.y - vertexList[j].Normal.y) < epsilon) &&
				(abs(verticesCompact[i].Normal.z - vertexList[j].Normal.z) < epsilon) &&
				(abs(verticesCompact[i].Tex.x - vertexList[j].Tex.x) < epsilon) &&
				(abs(verticesCompact[i].Tex.y - vertexList[j].Tex.y) < epsilon) &&
				(abs(verticesCompact[i].Tex.z - vertexList[j].Tex.z) < epsilon) &&
				(abs(verticesCompact[i].Joint.x - vertexList[j].Joint.x) < epsilon) &&
				(abs(verticesCompact[i].Joint.y - vertexList[j].Joint.y) < epsilon) &&
				(abs(verticesCompact[i].Joint.z - vertexList[j].Joint.z) < epsilon) &&
				(abs(verticesCompact[i].Joint.w - vertexList[j].Joint.w) < epsilon) &&
				(abs(verticesCompact[i].Weight.x - vertexList[j].Weight.x) < epsilon) &&
				(abs(verticesCompact[i].Weight.y - vertexList[j].Weight.y) < epsilon) &&
				(abs(verticesCompact[i].Weight.z - vertexList[j].Weight.z) < epsilon) &&
				(abs(verticesCompact[i].Weight.w - vertexList[j].Weight.w) < epsilon))
			{
				Duplicate = true;
				indices[i] = j;
				break;
			}
		}



		if (Duplicate == false)
		{
			vertexList.push_back(verticesCompact[i]);
			indices[i] = vertexList.size() - 1;


		}



	}
	for (int i = 0; i < vertexList.size(); i++)
	{
		verticesCompact[i] = vertexList[i];



	}
	for (int i = 0; i < numIndices; i++)
	{
		indicesList.push_back(indices[i]);
	}
	SimpleMeshAnim GenerateTangents;
	GenerateTangents.vertexList = vertexList;
	GenerateTangents.indicesList = indicesList;
	ComputeTangent(GenerateTangents);
	vertexList = GenerateTangents.vertexList;
	indicesList = GenerateTangents.indicesList;


	// print out some stats
	//std::cout << "\nindex count BEFORE/AFTER compaction " << numIndices;
	//std::cout << "\nvertex count ORIGINAL (FBX source): " << numVertices;
	//std::cout << "\nvertex count AFTER expansion: " << numIndices;
	//std::cout << "\nvertex count AFTER compaction: " << vertexList.size();
	//std::cout << "\nSize reduction: " << ((numVertices - vertexList.size()) / (float)numVertices) * 100.00f << "%";
	//std::cout << "\nor " << (vertexList.size() / (float)numVertices) << " of the expanded size";

	//write out Mesh
	const char* path = meshfile;
	std::ofstream file(path, std::ios::trunc | std::ios::binary | std::ios::out);

	assert(file.is_open());

	uint32_t index_count = (uint32_t)indicesList.size();
	uint32_t vert_count = (uint32_t)vertexList.size();

	file.write((const char*)&index_count, sizeof(uint32_t));
	file.write((const char*)indicesList.data(), sizeof(uint32_t) * indicesList.size());
	file.write((const char*)&vert_count, sizeof(uint32_t));
	file.write((const char*)vertexList.data(), sizeof(SimpleVertexAnim) * vertexList.size());

	file.close();


}





