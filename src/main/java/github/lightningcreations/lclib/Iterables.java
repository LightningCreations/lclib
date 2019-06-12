package github.lightningcreations.lclib;

import java.util.Enumeration;
import java.util.Iterator;
import java.util.NoSuchElementException;

public interface Iterables {
	public static <T> Iterable<T> fromEnumeration(final Enumeration<T> e){
		return ()->e.asIterator();
	}
	@SafeVarargs
	public static <T> Iterable<T> join(final Iterable<? extends T> a,final Iterable<? extends T>... b){

		class JoinedIterator implements Iterator<T>{
			final Iterator<? extends T> ai = a.iterator();
			@SuppressWarnings("unchecked")
			final Iterator<? extends T>[] bi = (Iterator<? extends T>[]) new Iterator[b.length];
			JoinedIterator() {
				for(int i =0;i<b.length;i++)
					bi[i] = b[i].iterator();
			}
			@Override
			public boolean hasNext() {
				if(ai.hasNext())
					return true;
				else for(Iterator<? extends T> i:bi)
					if(i.hasNext())
						return true;
				return false;
			}

			@Override
			public T next() {
				if(ai.hasNext())
					return ai.next();
				else for(Iterator<? extends T> i:bi)
					if(i.hasNext())
						return i.next();
				throw new NoSuchElementException();
			}
		}
			
		return ()->new JoinedIterator();
	}
}
