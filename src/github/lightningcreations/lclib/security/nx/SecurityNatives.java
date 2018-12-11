package github.lightningcreations.lclib.security.nx;

import java.nio.Buffer;

import javax.security.auth.DestroyFailedException;

import github.lightningcreations.lclib.jnix.NativeExtensionException;

public abstract class SecurityNatives {
	public static final boolean USING_NATIVE_SECURITY_EXTENSIONS = Boolean.getBoolean("lclib.security.jnix.security_natives");
	static {
		if(USING_NATIVE_SECURITY_EXTENSIONS) {
			System.loadLibrary("lc-jni");
			registerNatives();
		}
	}
	private static native void registerNatives();
	private SecurityNatives() {
		
	}
	public static native void destroyNative(byte[] b)throws DestroyFailedException,NativeExtensionException;
	public static native void destroyNative(Buffer b)throws DestroyFailedException,NativeExtensionException;
}
