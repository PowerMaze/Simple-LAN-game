#pragma once

enum EButtonPositionType
{
	BPT_LEFT = -1,
	BPT_CENTER = 0,
	BPT_RIGHT = 1
};

class CMenuButton 
{
public:
	const char* m_strName;
	const char* m_strCaption;
	const char* m_strDescription;
	float m_fCorners[4];
	unsigned short m_uiXpos;
	unsigned short m_uiYpos;
	unsigned char m_iPosType;
public:
	CMenuButton(void);
	CMenuButton(const char* name, const char* caption, const char* description, unsigned short X, unsigned short Y);
	~CMenuButton(void);
	void Clear(void);
	void DrawButton(HEXColor color);
};