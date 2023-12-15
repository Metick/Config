# Config System
## Introduction
Welcome to the Config System repository! This lightweight configuration system is designed to provide a simple and flexible way to manage configuration settings for your applications. It allows you to create, load, modify, delete and save configuration files with ease.

## Getting Started
To use the Config System in your project, follow these steps:

Include the Include folder in your project:

#include "Include/config.h"

There is already a instance created inside config.h which you can access as `settings`.

## Create a default configuration file and load it:

```cpp
if (!settings.exists("Default.conf"))
    settings.save("Default.conf");
settings.load("Default.conf");
```
## Usage Examples

### Accessing Config Values
```cpp
std::cout << "Float: " << settings.a_float_value << std::endl;
```

### Modifying Config Values
To edit the config, You have to pass through the value as 'pointer'
For example in my Main.cpp:
```cpp
modify_struct(&settings.test_struct);
```

### Incorrect Way to Edit Config Values
```cpp
// This won't work as intended
settings.a_float_value = 1337.f;
std::cout << "Attempting to edit the value wrong" << std::endl;
std::cout << "Float: " << settings.a_float_value << std::endl;
std::cout << "Value remains the same, as it's not edited through a pointer." << std::endl;
```

### Correct Ways to Edit Config Values
```cpp
// Use Set function to edit the value
settings.Set(&settings.a_float_value, 1337.f);
std::cout << "Now it will have a new value" << std::endl;
std::cout << "Float: " << settings.a_float_value << std::endl;
```

### Using Custom Function
```cpp
void Test(float* value)
{
	*value = 69.f;
}

// Use a custom function to edit the value
Test(&settings.a_float_value);
std::cout << "Float: " << settings.a_float_value << std::endl;
```

## Saving Config
After making changes, save the configuration:
```cpp
// Save the modified config
settings.save("Default.conf");
```

License
This Config System is open-source and licensed under the MIT License. Feel free to use, modify, and distribute it in your projects.
