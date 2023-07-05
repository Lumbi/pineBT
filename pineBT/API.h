#pragma once

#define EXPORT __declspec(dllexport)

extern "C"
{
	using BehaviorTreeHandle = int;

	EXPORT void pineBT_schemas(char*);

	EXPORT BehaviorTreeHandle pineBT_create(const char* json);

	EXPORT void pineBT_destroy(BehaviorTreeHandle);

	EXPORT void pineBT_run(BehaviorTreeHandle);

	EXPORT void pineBT_status(BehaviorTreeHandle, char*);
}

#undef EXPORT
