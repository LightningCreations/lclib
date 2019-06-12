package github.lightningcreations.lclib.invoke.internal;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.util.function.IntSupplier;
import java.util.function.Supplier;

public final class JavaOpsImpl {

	private JavaOpsImpl() {
		// TODO Auto-generated constructor stub
	}
	
	private static class LoadDynamicInt implements IntSupplier{
		private boolean hasLoaded;
		private int value;
		private final MethodHandle bootstrap;
		private final MethodHandle[] rest;
		private final String name;
		private final MethodHandles.Lookup lookup;
		private LoadDynamicInt(MethodHandle bootstrap,MethodHandles.Lookup lookup,String name,MethodHandle[] constants) {
			this.bootstrap = bootstrap;
			this.rest = constants;
			this.name = name;
			this.lookup = lookup;
		}
		public int getAsInt() {
			try {
			if(!hasLoaded)
				synchronized(this) {
					if(!hasLoaded) {
						MethodHandle bootstrap = MethodHandles.filterArguments(this.bootstrap, 2, rest);
						value = (int)bootstrap.invokeExact(lookup,name,int.class);
						hasLoaded = true;
					}
				}
			}catch(Error|RuntimeException e) {
				throw e;
			}catch(Throwable t) {
				throw new RuntimeException(t);
			}
			return value;
		}
	}

}
