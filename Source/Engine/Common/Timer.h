#pragma once

#include <chrono>
//#include <Engine/EMain.h>
/*
class ENGINE_API CTimer 
{
public:
	CTimer();
	CTimer(const float time);
	~CTimer(void);
	double getTick(void);
	float setTick(const float time);
	double getDeltaTime(void);
	void setDeltaTime(const double deltatime);
	std::string getDataTime(void);
private:
//	float m_fTimeValue;
	double m_fDeltaTime;
};

extern ENGINE_API CTimer* timerMain;
*/

ENGINE_API char* getTimeDate(void);

// gets system time. Needed for seting packets.
// @returns time in miliseconds, 64 bit
inline unsigned long long getSystemTime() 
{
	unsigned long long __ullTime;

	auto now = std::chrono::system_clock::now();

	// Convert the current time to time since epoch
	auto duration = now.time_since_epoch();

	// Convert duration to milliseconds
	__ullTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	return __ullTime;
}

