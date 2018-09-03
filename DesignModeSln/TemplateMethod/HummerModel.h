#pragma once
class CHummerModel
{
public:
	CHummerModel(void);
	virtual ~CHummerModel(void);
	void Run();
protected:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Alarm() = 0;
	virtual void EngineBoom() = 0;
	virtual bool IsAlarm();
};

