#include "stdafx.h"
#include "mxml.h"
#include "mxmlAnalyze.h"

CMXmlAnalyze::CMXmlAnalyze()
{

}

CMXmlAnalyze::~CMXmlAnalyze()
{

}


#if 1
////////////////////////////////////////////////////////////////////////////////////////////////
/*
xml参数说明：
	[c_name]     [采集器名称] 
	[c_addr]     [采集器编号]  
	[c_sample]   [采样周期]   
	
	[i_addr]     [总线接口编号]
	
	[d_start]    [设备起始序号]
	[d_num]      [设备数]
	[d_addr]     [设备地址]
	[d_type]     [设备类型]                  1:AM-3PHASE; 2:AM-1PHASE; 3:AM-DC; 4:AM-HARM;
	                                         11:AM-3PHASE-1; 12:AM-3PHASE-2; 13:AM-3PHASE-3;
	                                         21:AM-1PHASE-1; 22:AM-1PHASE-2; 23:AM-1PHASE-3;
	                                         31:AM-DC-1;     32:AM-DC-2;     33:AM-DC-3;
	                                         41:AM-HARM-1;   42:AM-HARM-2;   43:AM-HARM-3;  
	                                                
	[d_baud]     [波特率]                    1:2400; 2:4800; 3:9600; 4:115200;
	[d_pari]     [校验位]                    0:无; 1:奇; 2:偶;
	
	[w_mask]     [回路掩码]    
	[w_addr]     [回路编码]   
	
	[e_type]     [事件类型]                  1:配置改变;
*/
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////

static bool _parser_xml_4_read_col_conf_1(mxml_node_t* tree, mxml_node_t* node, st_col_conf* resp)
{
	bool ret = false;
	bool flag1 = false, flag2 = false, flag3 = false, flag4 = false;

	node = mxmlWalkNext(node, tree);
	while (node != NULL)
	{
		if (node->type == MXML_ELEMENT && node->value.element)
		{
			if (!strcmp(node->value.element, "c_name"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_TEXT && node->value.text)
					{
						strcpy(resp->name, node->value.text);
						flag1 = true;
					}
				}
            }
			else if (!strcmp(node->value.element, "c_type"))
			{
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_TEXT && node->value.text)
					{
						char type[256] = "";
						strcpy(type, node->value.text);
						flag1 = true;
					}
				}
			}
			else if (!strcmp(node->value.element, "c_addr"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_INTEGER)
					{
						resp->id =  node->value.integer;
						flag2 = true;
					}
				}
            }
			else if (!strcmp(node->value.element, "c_sample"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_INTEGER)
					{
						resp->sample_m =  node->value.integer;
						flag3 = true;
					}
				}
            }
			else if (!strcmp(node->value.element, "i_num"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_INTEGER)
					{
					//	resp->col_conf.i_num =  node->value.integer;
						flag4 = true;
					}
				}
            }
		}
		else
		{
			ret = false;
		}

		node = mxmlWalkNext(node, tree);
	}

	ret = flag1 && flag2 && flag3 && flag4;

	return ret;
}

static mxml_type_t _type_cb_read_col_conf (mxml_node_t *node)
{
    char* type = node->value.element;

    if (!strcmp(type, "c_name") || !strcmp(type, "c_type") )
        return (MXML_TEXT);
    else
        return (MXML_INTEGER);
}

bool net_pro_parser_resp_4_read_col_conf(char* buf, st_col_conf* resp)
{
	bool ret = false;

	mxml_node_t *tree = mxmlLoadString(buf, _type_cb_read_col_conf);
	if (tree)
	{
		ret = _parser_xml_4_read_col_conf_1(tree, tree, resp);
		mxmlRelease(tree);
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
int net_pro_compose_req_4_read_if_conf(char* buf, u_req* req)
{
	char req_str[] = 
	{
		"<req>"
			"<i_addr>%d</i_addr>"
		"</req>"
	};

	sprintf(buf, req_str, req->i_addr);

	return strlen(buf);
}	

static bool _parser_xml_4_read_if_conf_2(mxml_node_t* tree, mxml_node_t** p_node, u_resp* resp)
{
	bool ret = false;
	bool flag1 = false, flag2 = false, flag3 = false, flag4 = false;
	int dev_order;
	mxml_node_t* node = *p_node;

	dev_order = resp->if_conf.d_num;

	node = mxmlWalkNext(node, tree);
	while (node != NULL)
	{
		if (node->type == MXML_ELEMENT && node->value.element)
		{
			if (!strcmp(node->value.element, "n"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_TEXT && node->value.text)
					{
						strcpy(resp->if_conf.dev_conf[dev_order].d_name, node->value.text);
						flag1 = true;
					}
				}
            }
			else if (!strcmp(node->value.element, "a"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_INTEGER)
					{
						resp->if_conf.dev_conf[dev_order].d_addr =  node->value.integer;
						flag2 = true;
					}
				}
            }
			else if (!strcmp(node->value.element, "t"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_INTEGER)
					{
						resp->if_conf.dev_conf[dev_order].dev_type = (e_dev_type)(node->value.integer >> 4); 
						flag3 = true;
					}
				}
            }
			else if (!strcmp(node->value.element, "o"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_TEXT && node->value.text)
					{
						sscanf(node->value.text, "%d,%d,%d", 
							&resp->if_conf.dev_conf[dev_order].w_mask,
							&resp->if_conf.dev_conf[dev_order].d_baud,
							&resp->if_conf.dev_conf[dev_order].d_pari);

						switch(resp->if_conf.dev_conf[dev_order].d_baud)
						{
							case 1:
								resp->if_conf.dev_conf[dev_order].d_baud = 2400;
								break;

							case 2:
								resp->if_conf.dev_conf[dev_order].d_baud = 4800;
								break;

							case 3:
								resp->if_conf.dev_conf[dev_order].d_baud = 9600;
								break;

							case 4:
								resp->if_conf.dev_conf[dev_order].d_baud = 115200;
								break;

							default:
								resp->if_conf.dev_conf[dev_order].d_baud = 115200;
								break;
						}

						flag4 = true;
					}
				}
            }
		}

		node = mxmlWalkNext(node, tree);
	}

	*p_node = node;

	ret = flag1 && flag2 && flag3 && flag4;

	return ret;
}

static bool _parser_xml_4_read_if_conf_1(mxml_node_t* tree, mxml_node_t* node, u_resp* resp)
{
	bool ret = false;

	node = mxmlWalkNext(node, tree);
	while (node != NULL)
	{
		if (node->type == MXML_ELEMENT && node->value.element)
		{
			if (!strcmp(node->value.element, "d"))
            {
				ret = _parser_xml_4_read_if_conf_2(tree, &node, resp);
				if (!ret)
				{
					break;
				}
				else
				{
					resp->if_conf.d_num++;
				}
            }
		}

		node = mxmlWalkNext(node, tree);
	}

	return ret;
}

static mxml_type_t _type_cb_read_if_conf (mxml_node_t *node)
{
    char* type = node->value.element;

    if (!strcmp(type, "n") || !strcmp(type, "o"))
        return (MXML_TEXT);
    else
        return (MXML_INTEGER);
}

bool net_pro_parser_resp_4_read_if_conf(char* buf, u_resp* resp)
{
	bool ret = false;

	mxml_node_t *tree = mxmlLoadString(buf, _type_cb_read_if_conf);
	if (tree)
	{
		ret = _parser_xml_4_read_if_conf_1(tree, tree, resp);
		mxmlRelease(tree);
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int net_pro_compose_req_4_heart_beat(char* buf, u_req* req)
{
	return 0;
}

bool net_pro_parser_resp_4_heart_beat(char* buf, u_resp* resp)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int net_pro_compose_req_4_read_rt_event(char* buf, u_req* req)
{
	return 0;
}

static bool net_pro_parser_resp_4_read_rt_event_1(mxml_node_t* tree, mxml_node_t* node, u_resp* resp)
{
	node = mxmlWalkNext(node, tree);
	while (node != NULL)
	{
		if (node->type == MXML_ELEMENT && node->value.element)
		{
			if (!strcmp(node->value.element, "item"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_TEXT)
					{
						sscanf(node->value.text, "%d,%d", &resp->event_arr.event_data[resp->event_arr.event_num].event_type, 
							                              &resp->event_arr.event_data[resp->event_arr.event_num].event_code);
						resp->event_arr.event_num++;
					}
				}	
            }
		}

		node = mxmlWalkNext(node, tree);
	}

	return true;
}

static mxml_type_t _type_cb_read_rt_event(mxml_node_t *node)
{
	char* type = node->value.element;

    if (!strcmp(type, "item"))
        return (MXML_TEXT);
    else
        return (MXML_INTEGER);
}

bool net_pro_parser_resp_4_read_rt_event(char* buf, u_resp* resp)
{
	bool ret = false;

	mxml_node_t *tree = mxmlLoadString(buf, _type_cb_read_rt_event);
	if (tree)
	{
		ret = net_pro_parser_resp_4_read_rt_event_1(tree, tree, resp);
		mxmlRelease(tree);
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int net_pro_compose_req_4_write_time(char* buf, u_req* req)
{
	char req_str[] = 
	{
		"<req>"
			"<time>%s</time>"
		"</req>"
	};

	sprintf(buf, req_str, req->time);

	return strlen(buf);
}

bool net_pro_parser_resp_4_write_time(char* buf, u_resp* resp)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

int net_pro_compose_req_4_read_his_data(char* buf, u_req* req)
{
	char req_str[] = 
	{
		"<req>"
			"<pre_result>%d</pre_result>"
		"</req>"
	};

	if (req->pre_result)
	{
		sprintf(buf, req_str, 1);
	}
	else
	{
		sprintf(buf, req_str, 0);
	}

	return strlen(buf);
}

static bool net_pro_parser_resp_4_read_his_data_2(mxml_node_t* tree, mxml_node_t** p_node, u_resp* resp)
{
	bool ret = false;
	bool flag1 = false, flag2 = false, flag3 = false;
	st_way_data *way_data;
	char data[128];
	mxml_node_t* node = *p_node;

	way_data = &resp->way_arr.way_data[resp->way_arr.way_num];

	node = mxmlWalkNext(node, tree);
	while (node != NULL)
	{
		if (node->type == MXML_ELEMENT && node->value.element)
		{
			if (!strcmp(node->value.element, "d_type"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_INTEGER)
					{
						way_data->dev_type = (e_dev_type)(node->value.integer >> 4);
						flag1 = true;
					}
				}	
            }
			else if (!strcmp(node->value.element, "addr"))
            {
				if (node->child)
				{
					node = mxmlWalkNext(node, tree);
					if (node->type == MXML_TEXT)
					{
						sscanf(node->value.text, "%d,%d,%d", &way_data->i_addr, &way_data->d_addr, &way_data->w_addr);
						flag2 = true;
					}
				}	
            }
			else if (strstr(node->value.element, "CDATA"))
            {
				if (0 != sscanf(node->value.element, "![CDATA[%[^]]]]", data))
				{
					switch(way_data->dev_type)
					{
						case DEV_TYPE_AM_3PHASE:
							strcpy(way_data->way_measure.am_3phase.data, data);
							break;

						case DEV_TYPE_AM_1PHASE:
							strcpy(way_data->way_measure.am_1phase.data, data);
							break;

						case DEV_TYPE_AM_DC:
							strcpy(way_data->way_measure.am_dc.data, data);
							break;

						case DEV_TYPE_AM_HARM:
							strcpy(way_data->way_measure.am_harm.data, data);
							break;

						default:
							break;
					}
						
					flag3 = true;
				}
				break;
            }
		}

		node = mxmlWalkNext(node, tree);
	}

	*p_node = node;

	ret = flag1 && flag2 && flag3;

	return ret;
}

static bool net_pro_parser_resp_4_read_his_data_1(mxml_node_t* tree, mxml_node_t* node, u_resp* resp)
{
	node = mxmlWalkNext(node, tree);
	while (node != NULL)
	{
		if (node->type == MXML_ELEMENT && node->value.element)
		{
			if (!strcmp(node->value.element, "item"))
            {
				if (node->child)
				{
					if (net_pro_parser_resp_4_read_his_data_2(tree, &node, resp))
					{
						resp->way_arr.way_num++;
					}
				}	
            }
		}

		node = mxmlWalkNext(node, tree);
	}

	return true;
}

static mxml_type_t _type_cb_read_his_data(mxml_node_t *node)
{
	char* type = node->value.element;

    if (!strcmp(type, "addr"))
        return (MXML_TEXT);
    else
        return (MXML_INTEGER);
}

bool net_pro_parser_resp_4_read_his_data(char* buf, u_resp* resp)
{
	bool ret = false;

	mxml_node_t *tree = mxmlLoadString(buf, _type_cb_read_his_data);
	if (tree)
	{
		ret = net_pro_parser_resp_4_read_his_data_1(tree, tree, resp);
		mxmlRelease(tree);
	}

	return ret;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
#endif