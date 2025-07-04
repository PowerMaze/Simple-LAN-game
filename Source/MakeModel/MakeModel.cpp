#include <stdio.h>
#include <Engine\EMain.h>

void createModel_t(void);
void createTexture_t(void);
void createFont_t(void);

struct data 
{
    int value;
    float dst[3];
};

int main(int argcc, char** argv)
{
    //createModel_t();
    //createTexture_t();
 //  createFont_t();

    Network::CNetMSG msg;

    Vector3D vPos = Vector3D(1.6f);

    msg.InitMessage(NET_MAX_BUFFER);

    data d = { 5, {3.14f, -56.f, 0.f} };

    msg.writeBlock((byte*)&d, sizeof(struct data));

    printf("data - %x\n", d.value);

    memset(&d, 0, sizeof(struct data));

    FOREACH_ARRAY(msg.getLength())
    {
        printf("%X", msg.msg_data[i]);
    }

    msg.readBlock((byte*)&d, sizeof(struct data));



    printf("\n%d {%f %f %f}\n", d.value, d.dst[0], d.dst[1], d.dst[2]);

    return 0;    
}

void createTexture_t() 
{
    try {
        CFileName strFile = "C:\\Games\\In-the-Power\\Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.png";
        CTextureObject texture;
        texture.loadFromImage(strFile);
        strFile.ChangeEXT(".te");
        texture.Save(strFile);
    }
    catch (void* err) 
    {
        throw (const char*)err;
    }
}

void createFont_t()
{
    CFileName strFileName = "C:\\Games\\In-the-Power\\Data\\Font\\Display1.tga";
    try {
        CTextureObject texture;
        texture.loadFromImage(strFileName);
        strFileName.ChangeEXT(".te");
        texture.Save(strFileName);
    }
    catch (char* err) 
    {
        printf("cannot create texture for font: %s\n", err);
        return;
    }

    CFileStream file;

    if (!file.open("C:\\Games\\In-the-Power\\Data\\Font\\Display1.txt", 1, 1)) {
        printf("Error loading ASCII text file\n");
        return;
    }  

    unsigned int bytes;
    
    char* data = (char*)file.ReadFullFileData(&bytes);
    data[bytes] = 0;  

    file.closeFile();

    int w = 26, h = 35;

    CFont font;

    if (!font.createFont(data, w, h, strFileName))
    {
        printf("Error create font!\n");
       // file.closeFile();
        free(data);
    }

    strFileName.ChangeEXT(".fo");

    font.Save(strFileName);
    font.Clear();

    free(data);
}


void createModel_t()
{
    CModelObject* _mo = nullptr;
   /// COBJDecoder obj;

    CFileName strFile = "C:\\Games\\In-the-Power\\Data\\Models\\Editor\\Floor.obj";

    //_mo = CreateModel(obj, strFile);

    if (_mo == nullptr) 
    {
        PrintF("_mo == nullptr\n");
        return;
    }

    //_mo = new CModelObject;
    //CFileName strFile = "Data\\Models\\Misc\\TestAnimations\\Cat.mo";
    //_mo->Load(CFileName("C:\\Games\\In-the-Power\\Data\\Models\\Misc\\TestAnimations\\MorphCube.mo"));

    //addAnimations(_mo, "C:\\Games\\In-the-Power\\Data\\Models\\Misc\\TestAnimations\\MorphCube.txt");

    reportModelInfo(_mo);

   // strFile.ChangeEXT(".mo");
    _mo->Save(CFileName("C:\\Games\\In-the-Power\\Data\\Models\\Editor\\Floor.mo"));

    /*
    strFile.ChangeEXT(".txt");
    addAnimations(_mo, (const char*)strFile);
    strFile.ChangeEXT(".mo");
    _mo->Save(strFile);
    */

    delete _mo;
    _mo = NULL;

}


/*
class CEditorDocument
{
public:
    const char* strDocName;
    unsigned long imemoryUsed;

    bool bNew;
    bool bActive;

    CFileName strFileLocation;

    unsigned long lFlags;

public:

    CEditorDocument();
    CEditorDocument(CFileName strFile);
};

class CWorldDocument : public CEditorDocument
{
public:
    CGameWorld* gaWorld;
    HEXColor colWorld;


};

class CModelDocument : public CEditorDocument
{
public:
    CModelObject* moMainModel;
    CModelObject* moFloor;

    CTextureObject* teTextureModel;
    CTextureObject* teNormalTexture;
    CTextureObject* teFloor;

public:
};


class CEditorDocumentHolder
{
public:
    unsigned int iCount;
    CEditorDocument** edf_Documents;
    bool isUsed;
public:
    CEditorDocumentHolder(void)
    {
        iCount = 0;
        edf_Documents = NULL;
        isUsed = false;
    }
    void* getDocument(int i)
    {
        return edf_Documents[i];
    }
    void addDocument(CEditorDocument* eDoc)
    {
        if (eDoc == NULL)
            return;
        if (iCount == 0)
        {
            edf_Documents = (CEditorDocument**)malloc(sizeof(class CEditorDocument) * 1);
            iCount++;
            edf_Documents[iCount] = (CEditorDocument*)malloc(sizeof(class CEditorDocument));
        }
        else
        {

        }
    }
    void removeDocument(CEditorDocument* eDoc)
    {

    }
    void clearDoc()
    {

    }

};
*/
