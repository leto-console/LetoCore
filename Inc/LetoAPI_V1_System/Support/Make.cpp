#include "Make.hpp"

// ====================================================================================================

static int import_status = LETO_API_IMPORT_UNKNOWN;

#include <cstring>

#include "Available.hpp"

static void Import(const char* import)
{
	int check_res = LETO_API_IMPORT_UNKNOWN;

	for (const char* function : available_functions)
	{
		if (strcmp(import, function) == 0)
		{
			check_res = LETO_API_IMPORT_SUCCESS;
			break;
		}
	}

	if (check_res == LETO_API_IMPORT_UNKNOWN)
		import_status = LETO_API_IMPORT_ERROR;
	else if (import_status == LETO_API_IMPORT_UNKNOWN)
		import_status = LETO_API_IMPORT_SUCCESS;
}

static int CheckImport() { return import_status; }

static void ResetCheckResult() { import_status = LETO_API_IMPORT_UNKNOWN; }

// ====================================================================================================

const SupportAPI_V1* Make_SupportAPI()
{
	static const SupportAPI_V1 api
	{
		&Import, 
		&CheckImport,
		&ResetCheckResult
	};

	return &api;
}
