package github.lightningcreations.lclib.jnix;

public class NativeExtensionException extends Exception {
	private long ptr;
	NativeExtensionException(long exception) {
		ptr = exception;
	}
	private native String getWhat();
	@Override
	public String getMessage() {
		// TODO Auto-generated method stub
		return getWhat();
	}
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return getWhat();
	}
	


}
