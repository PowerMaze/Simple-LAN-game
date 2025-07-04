#include <stdAfx.h>
#include "stb_image.h"
#include "SkyBox.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/FileName.h>
#include <Engine/Common/FileIO.h>
#include <Engine/Graphics/Shader.h>
//#include <Engine/Graphics/View.h>

CSkyBox::CSkyBox()
{
	cubeMap = 0;
	VAO = 0;
	VBO = 0;
	EBO = 0;
	prepareForRender = false;
    loadSuccess = true;

    float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    unsigned int length = sizeof(skyboxVertices) / sizeof(skyboxVertices[0]);

    FOREACH_ARRAY(length) 
    {
        verticies.push_back(skyboxVertices[i]);
    }


}
CSkyBox::~CSkyBox()
{
    Clear();
}

void CSkyBox::makeSkyBox(std::vector<std::string>& textures)
{
	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < textures.size(); i++)
    {

        CFileName strFile(textures[i]);

        FileSetGlobalPatch(strFile);

        unsigned char* data = stbi_load((const char*)strFile, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            PrintF("Cubemap tex failed to load at path: %s", textures[i].c_str());
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CSkyBox::Load(CFileStream& file)
{
    // clear all data
    Clear();

    // check main ID in world file, if we have skybox
    if (!file.checkID_Safe("SKYB"))
    {
        loadSuccess = false;
        return;
    }
    // info about loaded data from file. 0 - 5 for length strings, 6 for length verticies of skybox
    unsigned int numData[7] = {};
    // read array contains size of datas
    file.ReadFromFile(numData, sizeof(unsigned int) * 7);
    // alocate paths of sky box textures
    char** strFileNames = (char**)malloc(sizeof(char*) * 6);
    // for each path strings
    FOREACH_ARRAY(6) 
    {
        // alocate path
        strFileNames[i] = (char*)malloc(sizeof(char) * numData[i] + 1);
        // read string from file
        file.ReadFromFile(strFileNames[i], sizeof(char) * numData[i]);
        // set zero-terminal
        strFileNames[i][numData[i]] = '\0';
        // add to container of std::strings
        strTexturesNameSkyBox.push_back(std::string(strFileNames[i]));
    }

    // for each verticies
    FOREACH_ARRAY(numData[6])
    {
        float data;
        file.ReadFromFile(&data, sizeof(float));
        // add to container of verticies
        verticies.push_back(data);
    }

    // release memory of strings
    FOREACH_ARRAY(6)
    {
        free(strFileNames[i]);
    }
    free(strFileNames);
    // create skebox from textures
    makeSkyBox(strTexturesNameSkyBox);
    // check flag is loads a successs
    loadSuccess = true;
}


void CSkyBox::Save(CFileStream& file)
{

    unsigned int numData[7];

    file.WriteToFile("SKYB", sizeof(char) * 4);
    

    FOREACH_ARRAY(6)
        numData[i] = strTexturesNameSkyBox[i].length();

    numData[6] = verticies.size();
    file.WriteToFile(numData, sizeof(unsigned int) * 7);


    FOREACH_ARRAY(6)
        file.WriteToFile(strTexturesNameSkyBox[i].c_str(), sizeof(char) * numData[i]);  

    FOREACH_ARRAY(numData[6])
    {
        file.WriteToFile(&verticies[i], sizeof(float));
    }
}
void CSkyBox::Prepare(void)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
//  glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Загружаем данные в вершинный буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);


    // Координаты вершин
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(0 * sizeof(float)));

    glBindVertexArray(0);

    prepareForRender = true;
}

void CSkyBox::Render(CShader& sha)
{
    glDepthMask(GL_FALSE);

    sha.use();

    sha.shaderSetMatrix("projection", pViewPort->getProjection());

    glm::mat4 view = glm::mat4(glm::mat3(pViewPort->m_viewMatrix));

    sha.shaderSetMatrix("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    sha.shaderSetMatrix("model", model);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glBindVertexArray(0);
}

void CSkyBox::Clear()
{
    verticies.clear();
    strTexturesNameSkyBox.clear();
    if (prepareForRender)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
     /* glDeleteBuffers(1, &EBO); */
        prepareForRender = false;
    }
}