// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// TODO replace all Nullcheck regions with this macro
// TODO try make it with template
#if !UE_BUILD_SHIPPING
/**
 * Checks the validity of a UObject pointer, logs a warning if it is invalid to help prevent editor crashes, and then returns.
 * This has no effect on shipping builds.
 */
#define UVALID_LOG_DEBUG(PointerParam)																					\
{																														\
	if (!IsValid(PointerParam))																							\
	{																													\
		UE_LOG(LogTemp, Warning, TEXT("%s is nullptr at \"%s#L%d\""), TEXT(#PointerParam),  TEXT(__FILE__), __LINE__)	\
		return;																											\
	}																													\
}
#define UVALID_LOG_DEBUGF(PointerParam)																					\
{																														\
	if (!IsValid(PointerParam))																							\
	{																													\
		UE_LOG(LogTemp, Warning, TEXT("%s is nullptr at \"%s#L%d\""), TEXT(#PointerParam),  TEXT(__FILE__), __LINE__)	\
		return 0.f;																										\
	}																													\
}
#define UVALID_LOG_DEBUGB(PointerParam)																					\
{																														\
	if (!IsValid(PointerParam))																							\
	{																													\
		UE_LOG(LogTemp, Warning, TEXT("%s is nullptr at \"%s#L%d\""), TEXT(#PointerParam),  TEXT(__FILE__), __LINE__)	\
		return false;																									\
	}																													\
}
#else
#define UVALID_LOG_DEBUG(PointerParam)  /* No-op in shipping builds */
#define UVALID_LOG_DEBUGF(PointerParam) /* No-op in shipping builds */
#define UVALID_LOG_DEBUGB(PointerParam) /* No-op in shipping builds */
#endif
