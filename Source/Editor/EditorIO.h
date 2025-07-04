#pragma once

namespace EditorModel
{
	CFileName openFile(void);
	void saveFile(void);
	CFileName newFile(void);
};

namespace EditorWorld
{
	CFileName openFile(void);
	CFileName saveFile(void);
	void newFile(void);
};

CFileName openTextureFile(void);
CFileName saveTextureFile(void);
CFileName createTextureFile(void);
