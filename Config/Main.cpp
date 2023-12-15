#include "include/config.h"
#include <iostream>

void modify_struct(placeholder_struct* holder)
{
	std::cout << "name: " << holder->name << std::endl;
	std::cout << "wooow: " << holder->wooow << std::endl;
	(*holder).name = "new name";
	(*holder).wooow = 1337;
	std::cout << "Display edited data: " << std::endl;
	std::cout << "name: " << holder->name << std::endl;
	std::cout << "wooow: " << holder->wooow << std::endl;
}

void Test(float* value)
{
	*value = 69.f;
}

int main()
{
	//create default config
	if (!settings.exists("Default.conf"))
		settings.save("Default.conf");
	//Load config
	settings.load("Default.conf");

	std::cout << "Float: " << settings.a_float_value << std::endl;
	//To edit the config you must edit it through a pointer.
	modify_struct(&settings.test_struct);

	//this isn't possible
	settings.a_float_value = 1337.f;
	std::cout << "Attempting to edit the value wrong" << std::endl;
	std::cout << "Float: " << settings.a_float_value << std::endl;
	std::cout << "Value as you can see is still the same, and hasn't received the edited value as it's not edited through a pointer." << std::endl;

	//Additionally if you don't use it through a function you can do this
	settings.Set(&settings.a_float_value, 1337.f);
	std::cout << "Now it will have a new value" << std::endl;
	std::cout << "Float: " << settings.a_float_value << std::endl;

	//we can also use a function to edit the value
	Test(&settings.a_float_value);
	std::cout << "Float: " << settings.a_float_value << std::endl;

	//save config
	//After running this program all previous saved data will be kept.
	settings.save("Default.conf");

	std::cout << "Hello World!\n";
}
