// pos.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pos.h"

/////////////////////////////////////////////////////////////////////////////////////////
CPosition::CPosition()
{
	pNation = new st_nation;
	memset(pNation , 0, sizeof(*pNation));
}

CPosition::~CPosition()
{
	delete pNation;
}

void CPosition::parser_db()
{
	int i,j;
	int pro_idx, city_idx, county_idx;

	for(i = 0; NULL != db_arr[i].code_id; i++)
	{
		if(!strcmp(db_arr[i].parent_id, "0     "))
		{
			//province
			pro_idx = pNation->num_province;

			pNation->province[pro_idx].code_id = db_arr[i].code_id;
			pNation->province[pro_idx].name = db_arr[i].name;

			pNation->num_province = pro_idx + 1;
		}
		else if(!strcmp(db_arr[i].parent_id, pNation->province[pro_idx].code_id))
		{
			//city
			city_idx = pNation->province[pro_idx].num_city;

			pNation->province[pro_idx].city[city_idx].code_id = db_arr[i].code_id;
			pNation->province[pro_idx].city[city_idx].name = db_arr[i].name;
			
			pNation->province[pro_idx].num_city = city_idx + 1;
		}
		else 
		{	
			for (j=0; j< pNation->province[pro_idx].num_city; j++)
			{
				if(!strcmp(db_arr[i].parent_id, pNation->province[pro_idx].city[j].code_id))
				{
					//county
					county_idx = pNation->province[pro_idx].city[j].num_county;

					pNation->province[pro_idx].city[j].county[county_idx].code_id = db_arr[i].code_id;
					pNation->province[pro_idx].city[j].county[county_idx].name = db_arr[i].name;

					pNation->province[pro_idx].city[j].num_county = county_idx + 1;

					break;
				}
			}
		}
	}
}

CString CPosition::GetNameById( int id )
{
	CString str = "";
	int i = 0;
	for(i = 0; NULL != db_arr[i].code_id; i++)
	{
		if (atoi(db_arr[i].code_id) == id)
		{
			str = db_arr[i].name;
			return  str;
		}
	}
	return  str;
}



