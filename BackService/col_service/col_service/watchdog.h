
#define FIRST_CHECK_DELAY_S  (5)
#define NORMAL_CHECK_DELAY_S (10)
#define RESTART_DELAY_S      (10*24*60*60)

class CWatchDog
{
private:
	int m_cnt;
	DWORD m_ProcID;
	bool m_is_first;

private:
	bool GetProcIDByExeName(DWORD &ProcessID, char* ExeName);
	bool ProcIsExist(void);
	bool ProcIsOK(void);
	void StartProc(void);

public:

	CWatchDog();
	virtual ~CWatchDog();
	void Init();
	void Task(void);
};


