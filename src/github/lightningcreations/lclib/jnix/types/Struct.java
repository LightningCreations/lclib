package github.lightningcreations.lclib.jnix.types;

import java.lang.ref.WeakReference;
import java.lang.reflect.InvocationTargetException;

import github.lightningcreations.lclib.Callable;
import github.lightningcreations.lclib.jnix.annotations.StructDecl;

public abstract class Struct {
	private static native void registerNatives();
	static {
		System.loadLibrary("lc-jni");
		registerNatives();
	}
	private long obj;
	protected Struct() {
		try {
			init(getClass().getAnnotation(StructDecl.class));
			final WeakReference<Struct> ref = new WeakReference<>(this);
			Runtime.getRuntime().addShutdownHook(new Thread() {
				public void run() {
					if(ref.get()!=null&&obj!=0)
						_cxx_destroy();
				}
			});
		}catch(Exception e) {
			if(obj!=0)
				_cxx_destroy();
		}
	}
	private native void init(StructDecl decl)throws Exception;
	private native void _cxx_destroy();
	protected final void finalize() {
		if(obj!=0)
			_cxx_destroy();
	}
	protected native Object getField(String name);
	protected native Callable<Struct> getMethod(String name);
	private static class StructMethod implements Callable<Struct>{
		private long targetAddress;
		private StructMethod(long address) {
			targetAddress = address;
		}
		@Override
		public native Object call(Struct obj, Object... args) throws InvocationTargetException;
	}
}
