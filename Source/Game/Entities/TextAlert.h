#pragma once

#include <Engine/EMain.h>

// test entity class
class CTextAlert : public CEntityMain 
{
public:

	std::string strAlert;
	//int i_delay;

	bool bLoop;
	bool bOnce;

	void setText(const char* strText);

	CTextAlert(void);

	void Load(CFileStream& stream);

public:

	void Update(void) override;

	void Initialize(void) override;
};
