package github.lightningcreations.lclib.security;
import static github.lightningcreations.lclib.security.nx.SecurityNatives.*;

import java.nio.Buffer;
import java.nio.ByteBuffer;

import javax.security.auth.DestroyFailedException;

import github.lightningcreations.lclib.jnix.NativeExtensionException;
import github.lightningcreations.lclib.security.nx.SecurityNatives;
/**
 * Interface with static methods which are useful in secure coding.
 * These methods are by default implemented in Java.
 * if the system property lclib.security.jnix.security_natives is true,
 * the first time that one of these methods is invoked, then these methods
 * will be implemented in native code, which is ultimately faster and more secure.
 * However these are library extensions and are not enabled by default.
 * @author connor
 *
 */
public interface SecurityUtils {
	/**
	 * Destroys a Byte array. If the property lclib.security.jnix.security_natives
	 * is true then this is implemented in terms of memset_s.
	 * Otherwise iterates over the array and sets each element to 0.
	 * If if native security extensions are used and an exception is thrown by the underlying c++ code the error is not propregated through.
	 * The array is still guaranteed to be cleared if this occurs
	 * @throws DestroyFailedException if native security extensions are used and some error occurs. Note that the array is still guarenteed to be destroy in this case
	 */
	public static void destroy(byte[] b)throws DestroyFailedException{
		try {
			if(SecurityNatives.USING_NATIVE_SECURITY_EXTENSIONS) {
				destroyNative(b);
				return;
			}
		}catch(DestroyFailedException e) {
			for(int i = 0;i<b.length;i++)
				b[i] = 0;
			throw e;
		}catch(NativeExtensionException e) {
			for(int i = 0;i<b.length;i++)
				b[i] = 0;
			return;
		}
		for(int i = 0;i<b.length;i++)
			b[i] = 0;
	}
	public static void destroy(ByteBuffer b)throws DestroyFailedException{
		try {
		if(b.isDirect()&&SecurityNatives.USING_NATIVE_SECURITY_EXTENSIONS)
			destroyNative(b);
		}catch(DestroyFailedException | NativeExtensionException e) {
			destroy(b.array());
		}
		destroy(b.array());
	}
}
