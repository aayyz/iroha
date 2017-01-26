/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class repository_AssetRepository */

#ifndef _Included_repository_AssetRepository
#define _Included_repository_AssetRepository
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     repository_AssetRepository
 * Method:    add
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_repository_AssetRepository_add
  (JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     repository_AssetRepository
 * Method:    findOne
 * Signature: (Ljava/lang/String;)Ljava/util/HashMap;
 */
JNIEXPORT jobject JNICALL Java_repository_AssetRepository_findOne
  (JNIEnv *, jclass, jstring);

/*
 * Class:     repository_AssetRepository
 * Method:    update
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_repository_AssetRepository_update
  (JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     repository_AssetRepository
 * Method:    remove
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_repository_AssetRepository_remove
  (JNIEnv *, jclass, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif