/*
==============================================

			loader .obj file

==============================================
*/

#pragma once


#define MAX_VERTEX_COUNT 3
#define MAX_STRING_FILE_OBJ 8
#define MATERIAL_KEY "newmtl"
#define FACE_KEY "usemtl"

/*
simple obj face for In the Engine data
*/
struct objFace_t
{
	int		    VertexIndex[MAX_VERTEX_COUNT];	// Indices of the position vertices
	int		    NormalIndex[MAX_VERTEX_COUNT];	// Indices of each vertex normal
	int         TextureIndex[MAX_VERTEX_COUNT];	// Indices of each vertex texture coordinate
	std::string	Name;							// String name of the face for material
};

/*
obj material data to import In the Engine model, contains name, diffuse color and tag. 
*/
struct objMaterial_t
{
	std::string	Name;			// String name of the material
	double		diff[3];        // Diffuse reflectance values
	int		    Tag;			// Material Tag
};

class CFileOBJ
{
public:
	
	std::vector<glm::vec3>	vertexList;			// Array of vertex vector pointers
	//std::vector<glm::vec3>	vertexNormalList;	// Array of normal vector pointers
	std::vector<glm::vec2>	vertexTextureList;	// Array of texture vector pointers

	std::vector<objFace_t>	   faceList;		// Array of face pointers
	std::vector<objMaterial_t> materialList;	// Array of material pointers

public:
	ENGINE_API CFileOBJ(void) = default;
	ENGINE_API ~CFileOBJ();								// destructor
	ENGINE_API void Load(const CFileName &StrFileName); 	// load data


public:
	ENGINE_API void GetRGB(byte &r, byte &g, byte &b, const double diffColor[3]);         // get color
	ENGINE_API objMaterial_t GetMaterialFromFace(const std::string &strName);                   // get material from face
private:
	ENGINE_API void GetFaceFromOBJ(char *str, int *arr);
};
