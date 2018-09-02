#pragma once
namespace NSP_STP_CM
{
	class CMString
	{
	public:
		CMString(void);
		CMString(const CMString& other_obj);
		CMString(const char* other_str);
		~CMString(void);
	public:
		CMString operator=(const CMString& other_obj);
		CMString operator=(const char* other_str);
		CMString operator+(const CMString& other_obj);
		CMString operator+(const char* other_str);
		bool operator==(const CMString& other_obj);
		bool empty();
		const char* c_str();
		void swap(CMString& other_obj);

		void test();

	private:
		char* m_pStr;
	};

}
