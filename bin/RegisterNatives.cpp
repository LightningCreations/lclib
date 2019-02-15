/*
 * RegisterNatives.c
 *
 *  Created on: Jan 16, 2019
 *      Author: chorm
 */

#include "NativeRoot.h"
#include <array>

#define fn_ptr(fn) reinterpret_cast<void*>(&(fn))

static const JNINativeMethod Security_natives[] = {
		{"destroyNative","[B",fn_ptr(Security_destroyNativeBarr)},
		{"destroyNative","Ljava/nio/Buffer;",fn_ptr(Security_destroyNativeBuffer)}
};

void Java_github_lightningcreations_lclib_security_nx_SecurityNatives_registerNatives(JNIEnv* env,jclass cl){
	env->RegisterNatives(cl, Security_natives, (jlong) sizeof(Security_natives)/sizeof(JNINativeMethod));
}

