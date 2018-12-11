package github.lightningcreations.lclib.invoke.internal;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;

public final class JavaOpsImpl {

	private JavaOpsImpl() {
		// TODO Auto-generated constructor stub
	}
	
	private static class LoadDynamicInt{
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
		public int getValue() throws Throwable {
			if(!hasLoaded)
				synchronized(this) {
					if(!hasLoaded) {
						MethodHandle bootstrap = MethodHandles.filterArguments(this.bootstrap, 2, rest);
						value = (int)bootstrap.invokeExact(lookup,name,int.class);
						hasLoaded = true;
					}
				}
			return value;
		}
	}

}
