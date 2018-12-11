package github.lightningcreations.lclib.sync;

import java.util.Deque;
import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Implements an Atomic Flag that can load and store values
 * If available and enabled, then uses Native Extensions.
 * Otherwise, uses some java bytecode hacks to achieve atomacy.
 * Unlike the types in java.util.concurrent.atomic, AtomicFlag is guaranteed to be lockfree
 * @author chorm
 *
 */
public final class AtomicFlag {
	private static final class AtomicFlagImpl{
		private AtomicBoolean underlying;
		static boolean IS_USING_NATIVE_ATOMICFLAG = Boolean.getBoolean("lclib.native.atomic");
		private static native void registerNatives();
		static {
			if(IS_USING_NATIVE_ATOMICFLAG) {
				System.loadLibrary("lcjni");
				registerNatives();
			}
		}
		private AtomicFlagImpl() {
			if(IS_USING_NATIVE_ATOMICFLAG)
				init0();
			else
				underlying = new AtomicBoolean();
		}
		native void cleanup0();
		private native void init0();
		void clear() {
			underlying.set(false);
		}
		native void clear0();
		boolean testAndSet() {
			return underlying.getAndSet(true);
		}
		native boolean testAndSet0();
	}
	private AtomicFlagImpl impl;
	public AtomicFlag() {
		// TODO Auto-generated constructor stub
	}
	
	public boolean testAndSet(boolean b) {
		if(AtomicFlagImpl.IS_USING_NATIVE_ATOMICFLAG)
			return impl.testAndSet0();
		else
			return impl.testAndSet();
	}
	public void clear() {
		if(AtomicFlagImpl.IS_USING_NATIVE_ATOMICFLAG)
			impl.clear0();
		else
			impl.clear();
	}
	protected void finalize() {
		if(AtomicFlagImpl.IS_USING_NATIVE_ATOMICFLAG)
			impl.cleanup0();//Call destructor
	}

}
