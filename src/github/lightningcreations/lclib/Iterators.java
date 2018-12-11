package github.lightningcreations.lclib;

import java.util.Iterator;
import java.util.NoSuchElementException;
import github.lightningcreations.lclib.primitive.*;
public interface Iterators {
	static <T> Iterator<T> over(final T[] t){
		return new Iterator<T>() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<t.length;
			}

			@Override
			public T next() {
				if(hasNext())
					throw new NoSuchElementException("End of the Array");
				else
					return t[idx++];
			}
		};
	}
	static ByteIterator over(final byte[] b) {
		return new ByteIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public byte next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static ShortIterator over(final short[] b) {
		return new ShortIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public short next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static CharIterator over(final char[] b) {
		return new CharIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public char next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static IntIterator over(final int[] b) {
		return new IntIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public int next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static FloatIterator over(final float[] b) {
		return new FloatIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public float next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static DoubleIterator over(final double[] b) {
		return new DoubleIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public double next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static BooleanIterator over(final boolean[] b) {
		return new BooleanIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<b.length;
			}

			@Override
			public boolean next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return b[idx++];
			}
		};
	}
	static LongIterator over(long[] array) {
		return new LongIterator() {
			int idx = 0;

			@Override
			public boolean hasNext() {
				// TODO Auto-generated method stub
				return idx<array.length;
			}

			@Override
			public long next() throws NoSuchElementException {
				if(!hasNext())
					throw new NoSuchElementException("End of the Array");
				return array[idx++];
			}
		};
	}
	static <T> Iterator<T> empty(){
		return new Iterator<T>() {
			public boolean hasNext() {
				return false;
			}
			public T next() {
				throw new NoSuchElementException("next() on empty iterator");
			}
		};
	}
	
}
