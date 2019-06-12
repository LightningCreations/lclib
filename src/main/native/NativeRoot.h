/*
 * NativeRoot.h
 *
 *  Created on: Jan 16, 2019
 *      Author: chorm
 */

#ifndef NATIVE_NATIVEROOT_H__2019_01_16_10_45_55
#define NATIVE_NATIVEROOT_H__2019_01_16_10_45_55

#include <jni.h>

#ifdef __cplusplus
extern"C"{
#endif

void Security_destroyNativeBarr(JNIEnv*,jclass,jbyteArray);
void Security_destroyNativeBuffer(JNIEnv*,jclass,jobject);

JNIEXPORT void Java_github_lightningcreations_lclib_security_nx_SecurityNatives_registerNatives(JNIEnv*,jclass);

#ifdef __cplusplus
};
#endif

#endif /* NATIVE_NATIVEROOT_H__2019_01_16_10_45_55 */
