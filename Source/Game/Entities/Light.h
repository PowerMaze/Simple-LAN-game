#pragma once

class CLight: public CEntityMain
{
public:

	Vector4D lg_colAmbient;
	Vector4D lg_colDiffuse;
	Vector4D lg_colSpecular;

public:

	CLight();

	~CLight();

	void End();

	void Initialize();

	void SetAmbient(HEXColor color);
	void SetDiffuse(HEXColor color);
	void SetSpecular(HEXColor color);

	void Save(CFileStream& stream);

	void Load(CFileStream& stream);

	void Update(void) override;

};