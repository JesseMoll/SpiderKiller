#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <string>
#include <exception>

#define DEFINE_MESSAGE_EXCEPTION(exception_name, message_text) \
class exception_name : public std::exception \
{ \
private: \
	std::string message; \
public: \
	exception_name(const std::string& name) \
	{ \
		message = message_text + name + "\n"; \
	} \
	virtual const char* what() const \
	{ \
		return message.c_str(); \
	} \
};

DEFINE_MESSAGE_EXCEPTION(file_open_error_exception, "Could not open file: ");
DEFINE_MESSAGE_EXCEPTION(file_read_error_exception, "Error reading file: ");
DEFINE_MESSAGE_EXCEPTION(texture_not_loaded_exception, "Texture not loaded: ");
DEFINE_MESSAGE_EXCEPTION(texture_not_found, "Texture not found: ");
DEFINE_MESSAGE_EXCEPTION(creep_not_found, "Creep not found: ");
DEFINE_MESSAGE_EXCEPTION(projectile_not_found, "Projectile not found: ");


#endif