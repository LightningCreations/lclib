package github.lightningcreations.lclib;

import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.time.Instant;
import java.util.UUID;

import github.lightningcreations.lclib.primitive.*;


/**
 * Compatibility class with hashcode methods provided for consistency with lclib-c++<br/>
 * This class contains a bunch of specializations of hashcode to make them consistent for things that depend on consistent hashcode implementations (Similar to Hash.hpp)
 * @author connor
 *
 */
public interface Hash {
	
	public static int hashcode(int i) {
		return i;
	}
	public static int hashcode(long l) {
		return (int)l^((int)l>>>32);
	}
	public static int hashcode(short s) {
		return s;
	}
	public static int hashcode(byte b) {
		return b&0xff;
	}
	public static int hashcode(boolean b) {
		return b?1331:1337;
	}
	public static int hashcode(/*const*/ Version v) {
		return v.hashCode();
	}
	public static int hashcode(/*const*/ UUID u) {
		return hashcode(u.getMostSignificantBits())*31+hashcode(u.getLeastSignificantBits());
	}
	public static <E extends Enum<E>> int hashcode(/*const*/ E e) {
		return e.ordinal();
	}
	public static int hashcode(/*const*/ String s) {
		int result= 0;
		final int prime = 31;
		for(byte b:s.getBytes(StandardCharsets.UTF_8)){
			result *= prime;
			result += hashcode(b);
		}
		return result;
	}
	public static int hashcode(Object o) {
		if(o instanceof Integer)
			return hashcode(((Integer)o).intValue());
		else if(o instanceof Byte)
			return hashcode(((Byte) o).byteValue());
		else if(o instanceof Short)
			return hashcode(((Short) o).shortValue());
		else if(o instanceof Long)
			return hashcode(((Long) o).longValue());
		else if(o instanceof Float)
			return hashcode(((Float) o).floatValue());
		else if(o instanceof Double)
			return hashcode(((Double) o).doubleValue());
		else if(o instanceof String)
			return hashcode((String)o);
		else if(o instanceof UUID)
			return hashcode((UUID)o);
		else if(o instanceof Version)
			return hashcode((Version)o);
		else if(o instanceof Enum<?>)
			return hashcode(((Enum<?>) o).ordinal());
		else if(o instanceof Instant)
			return hashcode((Instant)o);
		else if(o instanceof Duration)
			return hashcode((Duration)o);
		else if(o instanceof Object[])
			return hashcode((Object[])o);
		else if(o instanceof byte[])
			return hashcode((byte[])o);
		else if(o instanceof short[])
			return hashcode((short[])o);
		else if(o instanceof char[])
			return hashcode((char[])o);
		else if(o instanceof boolean[])
			return hashcode((boolean[])o);
		else if(o instanceof int[])
			return hashcode((int[])o);
		else if(o instanceof long[])
			return hashcode((long[])o);
		else if(o instanceof float[])
			return hashcode((float[])o);
		else if(o instanceof double[])
			return hashcode((double[])o);
		else if(o instanceof BooleanIterable)
			return hashcode((BooleanIterable)o);
		else if(o instanceof Iterable<?>)
			return hashcode((Iterable<?>)o);
		else if(o instanceof ByteIterable)
			return hashcode((ByteIterable)o);
		else if(o instanceof ShortIterable)
			return hashcode((ShortIterable)o);
		else if(o instanceof IntIterable)
			return hashcode((IntIterable)o);
		else if(o instanceof FloatIterable)
			return hashcode((FloatIterable)o);
		else if(o instanceof DoubleIterable)
			return hashcode((DoubleIterable)o);
		else if(o instanceof LongIterable)
			return hashcode((LongIterable)o);
		else if(o instanceof Class<?>)
			return hashcode((Class<?>)o);
		return o.hashCode();
	}
	
	public static int hashcode(Instant i) {
		return hashcode(i.getEpochSecond())*31+i.getNano();
	}
	public static int hashcode(Duration d) {
		return hashcode(d.getSeconds())*31+d.getNano();
	}
	public static int hashcode(Object[] oa) {
		int hash = 0;
		final int prime = 31;
		for(var o:oa) {
			hash *= prime;
			hash += hashcode(o);
		}
		return hash;
	}
	public static int hashcode(int[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(byte[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(short[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(char[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(long[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(float[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(double[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(boolean[] ia) {
		int hash = 0;
		final int prime = 31;
		for(var i:ia) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static <T> int hashcode(Iterable<T> itr) {
		int hash = 0;
		final int prime = 31;
		for(var i:itr) {
			hash *= prime;
			hash += hashcode(i);
		}
		return hash;
	}
	public static int hashcode(BooleanIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(ByteIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(ShortIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(CharIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(IntIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(FloatIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(DoubleIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(LongIterable itb) {
		int hash = 0;
		final int prime = 31;
		for(var itr = itb.iterator();itr.hasNext();) {
			hash *= prime;
			hash += hashcode(itr.next());
		}
		return hash;
	}
	public static int hashcode(Class<?> cl) {
		int hash = 0xf194be82;
		final int p1 = 736223333;
		final int p2 = 606598079;
		for(byte c:cl.getName().getBytes(StandardCharsets.UTF_8)){
			hash ^= c*p1;
			hash <<= 1;
			hash += c*p2;
		}
		return hash;
	}
}
