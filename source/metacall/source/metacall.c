/*
 *	MetaCall Library by Parra Studios
 *	Copyright (C) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A library for providing a foreign function interface calls.
 *
 */

#include <metacall/metacall-version.h>

#include <metacall/metacall.h>

#include <loader/loader.h>

#include <reflect/function.h>

#include <stdio.h>

#define METACALL_ARGS_SIZE 0x20

typedef union metacall_args_type
{
	char c;
	int i;
	long l;
	double d;
	void * ptr;

} * metacall_args;

int metacall_initialize(void)
{
	/* TODO: load a full path */

	loader_naming_name name = "example.py";

	return loader_load(name);
}

void * metacall(const char * name, ...)
{
	va_list va;

	function f = (function)loader_get(name);

	signature s = function_signature(f);

	union metacall_args_type m_args[METACALL_ARGS_SIZE];

	void * f_args[METACALL_ARGS_SIZE];

	size_t iterator;

	va_start(va, name);

	for (iterator = 0; iterator < signature_count(s); ++iterator)
	{
		type t = signature_get_type(s, iterator);

		type_id id = type_index(t);

		if (id == TYPE_CHAR)
		{
			m_args[iterator].c = (char)va_arg(va, int);
			f_args[iterator] = (void *)&m_args[iterator].c;
		}
		else if (id == TYPE_INT)
		{
			m_args[iterator].i = va_arg(va, int);
			f_args[iterator] = (void *)&m_args[iterator].i;
		}
		else if (id == TYPE_LONG)
		{
			m_args[iterator].l = va_arg(va, long);
			f_args[iterator] = (void *)&m_args[iterator].l;
		}
		else if (id == TYPE_DOUBLE)
		{
			m_args[iterator].d = va_arg(va, double);
			f_args[iterator] = (void *)&m_args[iterator].d;
		}
		else if (id == TYPE_PTR)
		{
			m_args[iterator].ptr = va_arg(va, void *);
			f_args[iterator] = (void *)&m_args[iterator].ptr;
		}
		else
		{
			m_args[iterator].ptr = NULL;
			f_args[iterator] = (void *)&m_args[iterator].ptr;
		}
	}

	va_end(va);

	function_call(f, f_args);

	return NULL;
}

int metacall_destroy(void)
{
	return loader_unload();
}

void metacall_print_info(void)
{
	printf("MetaCall Library " METACALL_VERSION "\n");
	printf("Copyright (c) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>\n");

	#ifdef METACALL_STATIC_DEFINE
		printf("Compiled as static library type\n");
	#else
		printf("Compiled as shared library type\n");
	#endif
}