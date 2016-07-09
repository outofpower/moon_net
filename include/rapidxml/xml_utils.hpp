#pragma once
#include <common/string_utils.hpp>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <memory>

namespace rapidxml
{
	/**
	* 根据名称查找子节点,查找失败返回 nullptr
	*
	* @parent 父节点.
	* @child 子节点名称
	*/
	inline xml_node<>*  find_child_node(xml_node<>* parent, const std::string& child)
	{
		if (parent == nullptr || parent->first_node() == nullptr || child.size() == 0)
			return nullptr;

		for (xml_node<>* nd = parent->first_node(); nd != nullptr; nd = nd->next_sibling())
		{
			if (child == nd->name())
			{
				return nd;
			}
		}

		return nullptr;
	}

	/**
	* 根据一个路径查找子节点，查找失败返回 nullptr
	*
	* @parent 父节点.
	* @child 子节点路径 ‘/’ 分隔开
	*/
	inline xml_node<>* get_child_node(xml_node<>* parent, const std::string& child)
	{
		if (parent == nullptr || parent->first_node() == nullptr || child.size() == 0)
			return nullptr;

		if (child.find("/") != std::string::npos)
		{
			std::vector<std::string> vec = string_utils::split<std::string>(child, "/");
			xml_node<>* node = parent;
			for(auto veciter = vec.begin(); veciter != vec.end(); veciter++)
			{
				node = find_child_node(node,*veciter);
				if (node == nullptr)
					break;
			}
			return node;
		}
		return find_child_node(parent, child);
	}

	inline xml_attribute<char>* get_child_attribute(xml_node<>* parent,const std::string& attribute_name, const std::string& child_path="")
	{
		if (parent == nullptr || attribute_name.size() == 0)
			return nullptr;
		
		auto child_node = parent;
		if (child_path.size() != 0)
		{
			child_node = get_child_node(parent, child_path);
		}

		if (child_node != nullptr)
		{
			auto child_attribute = child_node->first_attribute();
			for (;child_attribute != nullptr; child_attribute = child_attribute->next_attribute())
			{
				if (child_attribute->name() == attribute_name)
				{
					return child_attribute;
				}
			}
		}

		return nullptr;
	}

	/**
	* 根据一个路径查找子节点的值,查找失败返回default
	*
	* @parent 父节点.
	* @child 子节点路径 ‘/’ 分隔开
	* @default 默认值
	* e. <a>
			<b>
				<c>
				  <d>100</d>
				</c>
			</b>
		  </a>
		  parent 是节点a,  get_child_node_value<int>(parent,"b/c/d") == 100
	*/
	template<class T>
	inline T get_child_node_value(xml_node<>* parent, const std::string& child, const T& defalut =T())
	{
		auto nd = get_child_node(parent, child);
		if (nd != nullptr && nd->value() != nullptr && nd->value_size() >0)
		{
			return  string_utils::string_convert<T>(nd->value());
		}
		return defalut;
	}

	template<class T>
	inline T get_child_attribute_value(xml_node<>* parent, const std::string& attribute_name,const T& defalut =T(), const std::string& child_path="")
	{
		auto atr = get_child_attribute(parent, attribute_name,child_path);
		if (atr != nullptr && atr->value() != nullptr && atr->value_size() >0)
		{
			return  string_utils::string_convert<T>(atr->value());
		}
		return defalut;
	}


	//rapidxml::read_helper rh("HeroData.xml");
	//auto root = rh.root_node();
	//if(root.first != nullptr)// root.second 描述错误信息
	//{
	//	int no =  rapidxml::get_child_node_value<int>(root.first,"HeroData/ID");
	//	auto str  =  rapidxml::get_child_node_value<std::string>(root.first,"HeroData/ID");
	//}
	class read_helper
	{
	public:
		read_helper(const std::string& file)
			:m_path(file),f(nullptr)
		{

		}

		~read_helper()
		{
			if(f)
			{
				delete f;
				f = nullptr;
			}
		}

		std::pair<xml_node<>*,std::string> root_node()
		{
			try
			{
				if(f == nullptr)
				{
					f = new rapidxml::file<>(m_path.c_str());
				}

				do
				{
					doc.parse<0>(f->data());
					xml_node<>* root = doc.first_node();
					if (root == nullptr)
					{
						break;
					}
					return std::make_pair(root,string_utils::format("XML file [%s] parse success.",m_path.c_str()));
				}while(0);
			}
			catch(std::runtime_error e)
			{
				return std::make_pair(nullptr,string_utils::format("XML file [%s] open failed. %s",m_path.c_str(),e.what()));
			}
			catch (rapidxml::parse_error e)
			{
				return std::make_pair(nullptr,string_utils::format("XML file [%s] parse failed.",m_path.c_str(),e.what()));	
			}
			return std::make_pair(nullptr,string_utils::format("XML file [%s] root node is null.",m_path.c_str()));	
		}
	private:
		rapidxml::file<>* f;
		rapidxml::xml_document<> doc;
		std::string   m_path;
	};

	class xml_helper
	{
	public:
		xml_helper(const std::string& file)
			:m_path(file),f(nullptr)
		{

		}

		~xml_helper()
		{
			std::ofstream out(m_path);  
			out << doc;

			if(f)
			{
				delete f;
				f = nullptr;
			}
		}

		std::pair<xml_node<>*,std::string> open()
		{
			try
			{
				if(f == nullptr)
				{
					f = new rapidxml::file<>(m_path.c_str());
				}
				do
				{
					doc.parse<parse_declaration_node>(f->data());
					auto root = doc.first_node();
					if (root == nullptr)
					{
						return std::make_pair(nullptr, string_utils::format("XML file [%s] parse error.", m_path.c_str()));
					}

					if (root->type() != node_declaration)
					{
						return std::make_pair(nullptr, string_utils::format("XML file [%s] can not find declaration.", m_path.c_str()));
					}

					if (root->next_sibling() == nullptr)
					{
						break;
					}

					root = root->next_sibling();

					return std::make_pair(root,string_utils::format("XML file [%s] parse success.",m_path.c_str()));
				}while(0);
			}
			catch(std::runtime_error e)
			{
				return std::make_pair(nullptr,string_utils::format("XML file [%s] open failed. %s",m_path.c_str(),e.what()));
			}
			catch (rapidxml::parse_error e)
			{
				return std::make_pair(nullptr,string_utils::format("XML file [%s] parse failed.",m_path.c_str(),e.what()));	
			}
			return std::make_pair(nullptr,string_utils::format("XML file [%s] root is null.",m_path.c_str()));	
		}

		bool add_node(xml_node<>* node, const std::string& child_path,const std::string& value)
		{
			if(nullptr == node || child_path.size() == 0 || value.size() == 0)
				return false;

			std::vector<std::string> vec = string_utils::split<std::string>(child_path,"/");
			if(vec.size() ==0)
			{
				return  false;
			}

			for(auto& iter = vec.begin(); iter != vec.end(); iter++)
			{
				auto child_node = find_child_node(node, *iter);
				auto next_iter = iter;
				next_iter++;
				if (next_iter == vec.end())
				{
					if (child_node != nullptr)
					{
						node->remove_node(child_node);
					}
					child_node = doc.allocate_node(node_element, doc.allocate_string(iter->c_str()), doc.allocate_string(value.c_str()));
					assert(nullptr != child_node);
					node->append_node(child_node);
					return true;
				}
				if (child_node == nullptr)
				{
					child_node = doc.allocate_node(node_element, doc.allocate_string(iter->c_str()));
					node->append_node(child_node);
				}
				node = child_node;
			}
			return true;
		}

		bool add_attribute(xml_node<>* node, const std::string& attribute_name, const std::string& attribute_value, const std::string& child_path = "")
		{
			if(nullptr == node || attribute_name.size() == 0 || attribute_value.size() == 0)
				return false;

			if(child_path.size() != 0)
			{
				std::vector<std::string> vec = string_utils::split<std::string>(child_path, "/");
				if (vec.size() == 0)
				{
					return  false;
				}

				for (auto& iter = vec.begin(); iter != vec.end(); iter++)
				{
					auto child_node = find_child_node(node, *iter);
					if (child_node == nullptr)
					{
						child_node = doc.allocate_node(node_element, doc.allocate_string(iter->c_str()));
						assert(nullptr != child_node);
						node->append_node(child_node);
					}
					node = child_node;
				}
			}
			auto attr = get_child_attribute(node, attribute_name);
			if (attr != nullptr)
			{
				node->remove_attribute(attr);
			}
			attr = doc.allocate_attribute(doc.allocate_string(attribute_name.c_str()), doc.allocate_string(attribute_value.c_str()));
			node->append_attribute(attr);
			return true;
		}

	public:
		rapidxml::file<>* f;
		rapidxml::xml_document<> doc;
		std::string   m_path;
	};
}

