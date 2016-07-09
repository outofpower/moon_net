#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include <rapidxml\xml_utils.hpp>

using namespace rapidxml;

class ModuleConfig
{
public:
	void add(const std::string& name, const std::string& value)
	{
		if (value.size() > 0)
		{
			config[name] = value;
		}
	}

	std::unordered_map<std::string, std::string> config;
};


class server_config
{
public:
	bool load(const std::string& config_file)
	{
		file<> f(config_file.c_str());
		xml_document<> doc;
		try
		{
			do 
			{
				doc.parse<0>(f.data());
				//! 获取根节点  
				xml_node<>* root = doc.first_node();
				if (root->name() != std::string("config"))
				{
					printf("[%s] config file is not stander!\r\n", config_file.c_str());
					break;
				}

				for (xml_node<>* node = root->first_node(); node != nullptr; node = node->next_sibling())
				{
					std::string type = get_child_node_value<std::string>(node, "type");

					std::string name = get_child_node_value<std::string>(node, "name");

					std::string ip = get_child_node_value<std::string>(node, "network/ip");

					std::string port = get_child_node_value<std::string>(node, "network/port");

					std::string thread_num = get_child_node_value<std::string>(root, "network/net_thread_num", "1");

					ModuleConfig af;
					af.add("type", type);
					af.add("name", name);
					af.add("ip", ip);
					af.add("port", port);
					af.add("net_thread_num", thread_num);
					configs.push_back(af);
				}

				return true;
			} while (0);
		
		}
		catch (rapidxml::parse_error e)
		{
			printf("gate way config parse error\r\n");
		}

		return false;
	}

	std::vector<ModuleConfig> configs;
};
