#pragma once
#include "hash.h"
#include "fileIO.h"
#include <vector>
#include <string>

struct string_setting_t
{
	char text[128];
};

class node_t
{
public:
	virtual ~node_t() = default;

	virtual void add(node_t* node_ptr) = 0;

	virtual void load_setting(std::string& config_str) = 0;
	virtual void save_setting(std::string& config_str) = 0;
	//virtual bool set_value(std::string& feature) = 0;
};

class holder_t : public node_t
{
public:
	holder_t(const char* name) : m_name(name)
	{
	}

	holder_t(holder_t* holder_ptr, const char* name) : m_name(name) { holder_ptr->add(this); }

	void load_setting(std::string& node) override
	{
	}

	void save_setting(std::string& node) override
	{
	}

	//bool set_value(std::string& feature) override { return false; }

	void add(node_t* node_ptr) override;

	void load(const char* path, const char* file);

	void load(std::string& config);

	void save(std::string& config);

	void save(const char* path, const char* file);

	const char* m_name;
	std::vector<node_t*> m_nodes;
	std::string tempConfig;
};

class ISetting : public node_t
{
	void add(node_t* node) override
	{
	}
};

template <typename T>
class setting_t : ISetting
{
public:
	__forceinline setting_t(holder_t* holder_ptr, uint64_t name) : m_name(name) { holder_ptr->add(this); }

	__forceinline setting_t(holder_t* holder_ptr, uint64_t name, const T& rhs) : m_old_value(rhs), m_value(rhs), m_default(rhs), m_name(name) { holder_ptr->add(this); }

	bool changed()
	{
		uint8_t* data1 = reinterpret_cast<uint8_t*>(&m_value);
		uint8_t* data2 = reinterpret_cast<uint8_t*>(&m_old_value);

		for (size_t i = 0; i < sizeof(m_value); i++)
		{
			if (data1[i] != data2[i])
			{
				m_old_value = m_value;
				return true; // Objects are not equal
			}
		}
		return false;
	}

	void load_setting(std::string& config_str) override
	{
		m_value = m_default;

		uint8_t* data = (uint8_t*)&m_value;

		auto pos = config_str.find(std::to_string(m_name));
		if (pos == std::string::npos)
			return;

		pos += std::to_string(m_name).length() + 1;
		const char* buffer = config_str.data() + pos;
		for (size_t i = 0; i < sizeof(m_value); i++)
		{
			unsigned temp;
			sscanf(&buffer[2 * i], "%02X", &temp);
			data[i] = temp; //Set Value
		}
	}

	void save_setting(std::string& config_str) override
	{
		config_str.append(std::to_string(m_name) + ":");

		uint8_t* data = (uint8_t*)&m_value;

		char* buffer = new char[sizeof(m_value) * 2 + 1];
		for (size_t i = 0; i < sizeof(m_value); i++)
			sprintf(&buffer[2 * i], "%02X", data[i]);

		config_str.append(std::string(buffer) + ";");
		delete[] buffer;
	}

	setting_t& operator=(const T& rhs)
	{
		return *this;
	}

	__forceinline operator T&()
	{
		return m_value;
	}

	__forceinline operator const T&() const
	{
		return m_value;
	}

	__forceinline T* operator &()
	{
		return &m_value;
	}

	__forceinline const T* operator &() const
	{
		return &m_value;
	}

	__forceinline T& operator ()()
	{
		return m_value;
	}

	__forceinline const T& operator ()() const
	{
		return m_value;
	}

	bool operator!=(const T& rhs) const
	{
		return m_value != rhs;
	}

private:
	T m_value;
	T m_old_value;
	T m_default;
	uint64_t m_name;
};

template <typename t>
class settingtype_t
{
public:
	settingtype_t(const t& value = { })
	{
		for (size_t i = 0; i < 1; ++i)
			m_data[i] = value;
	}

	t m_data[1];

	t& operator( )()
	{
		return m_data[0];
	}

	t& operator[](const size_t& iter)
	{
		return m_data[iter];
	}
};

// fuck it!
struct settingstring_t
{
	char buffer[128];

	settingstring_t()
	{
		clear();
	}

	settingstring_t(const char* text)
	{
		set(text);
	}

	__forceinline void set(std::string& str)
	{
		set(str.c_str());
	}

	__forceinline void set(const char* text)
	{
		clear();
		strncpy_s(buffer, 128, text, 128);
	}

	__forceinline std::string get() const
	{
		return buffer;
	}

	__forceinline bool empty() const
	{
		return buffer[0] == 0;
	}

	__forceinline void clear()
	{
		memset(buffer, 0, 128);
	}

	__forceinline const char* c_str()
	{
		return buffer;
	}
};

//Struct for example
struct placeholder_struct
{
	std::string name;
	float wooow;
};

class c_settings
{
public:
	bool tooltips = false;
	holder_t m_holder {""};

	setting_t<bool> test_boolean {&m_holder, fnvc("test_boolean"), true};

	setting_t<float> a_float_value {&m_holder, fnvc("a_float_value"), 100.f};
	setting_t<placeholder_struct> test_struct {&m_holder, fnvc("test_struct"), {"Example", 50.f}};

	void save(const char* name)
	{
		std::string path = std::filesystem::temp_directory_path().string();
		path += "\\Config";
		if (!DoesPathExist(path))
			CreateFolder(path);
		m_holder.save(path.c_str(), name);
	}

	bool exists(const char* name)
	{
		std::string path = std::filesystem::temp_directory_path().string();
		path += "\\Config";
		if (!DoesPathExist(path))
			CreateFolder(path);
		path += "\\" + std::string(name);
		if (std::filesystem::exists(path))
			return true;
		return false;
	}

	void load(const char* name)
	{
		std::string path = std::filesystem::temp_directory_path().string();
		path += "\\Config";
		if (!DoesPathExist(path))
			return;
		m_holder.load(path.c_str(), name);
	}

	void _delete(const char* name)
	{
		std::string path = std::filesystem::temp_directory_path().string();
		path += "\\Config";
		if (!DoesPathExist(path))
			return;
		path += "\\" + std::string(name);
		if (std::filesystem::exists(path))
			std::filesystem::remove(path);
	}

	std::vector<std::string> get_all_configs()
	{
		std::vector<std::string> configs;
		std::string path = std::filesystem::temp_directory_path().string();
		path += "\\Config";
		if (!DoesPathExist(path))
			return configs;
		//get all files in folder
		try
		{
			for (const auto& entry : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_regular_file(entry.path()))
					configs.push_back(entry.path().filename().string());
		}
		catch (const std::exception& e)
		{
		}
		return configs;
	}

	void save_conf(std::string& config) { m_holder.save(config); }
	void load_conf(std::string& config) { m_holder.load(config); }

	//Place holders that can come in handy
	template <typename T>
	void Set(T* setting, T value)
	{
		*setting = value;
	}

	template <typename T>
	void Toggle(T* setting)
	{
		*setting = !*setting;
	}
};

extern c_settings settings;
