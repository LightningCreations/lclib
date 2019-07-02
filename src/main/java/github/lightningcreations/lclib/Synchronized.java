package github.lightningcreations.lclib;

import java.util.Collection;
import java.util.Iterator;
import java.util.Queue;

public interface Synchronized {

	public static <E> Queue<E> synchronizedQueue(final Queue<E> src){
		return new Queue<E>() {

			@Override
			public synchronized int size() {
				// TODO Auto-generated method stub
				return src.size();
			}

			@Override
			public synchronized boolean isEmpty() {
				// TODO Auto-generated method stub
				return src.isEmpty();
			}

			@Override
			public synchronized boolean contains(Object o) {
				// TODO Auto-generated method stub
				return src.contains(o);
			}

			@Override
			public synchronized Iterator<E> iterator() {
				// TODO Auto-generated method stub
				return src.iterator();
			}

			@Override
			public synchronized Object[] toArray() {
				// TODO Auto-generated method stub
				return src.toArray();
			}

			@Override
			public synchronized <T> T[] toArray(T[] a) {
				// TODO Auto-generated method stub
				return src.toArray(a);
			}

			@Override
			public synchronized boolean remove(Object o) {
				// TODO Auto-generated method stub
				return src.remove(o);
			}

			@Override
			public synchronized boolean containsAll(Collection<?> c) {
				// TODO Auto-generated method stub
				return src.containsAll(src);
			}

			@Override
			public synchronized boolean addAll(Collection<? extends E> c) {
				// TODO Auto-generated method stub
				return src.addAll(c);
			}

			@Override
			public synchronized boolean removeAll(Collection<?> c) {
				// TODO Auto-generated method stub
				return src.removeAll(c);
			}

			@Override
			public synchronized boolean retainAll(Collection<?> c) {
				// TODO Auto-generated method stub
				return src.retainAll(c);
			}

			@Override
			public synchronized void clear() {
				src.clear();
			}

			@Override
			public synchronized boolean add(E e) {
				// TODO Auto-generated method stub
				return src.add(e);
			}

			@Override
			public synchronized boolean offer(E e) {
				// TODO Auto-generated method stub
				return src.offer(e);
			}

			@Override
			public synchronized E remove() {
				// TODO Auto-generated method stub
				return src.remove();
			}

			@Override
			public synchronized E poll() {
				// TODO Auto-generated method stub
				return src.poll();
			}

			@Override
			public synchronized E element() {
				// TODO Auto-generated method stub
				return src.element();
			}

			@Override
			public synchronized E peek() {
				// TODO Auto-generated method stub
				return src.peek();
			}
		};
	}
	
}
