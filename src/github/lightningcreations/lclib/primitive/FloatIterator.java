package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;

public interface FloatIterator {
	public boolean hasNext();
	public float next()throws NoSuchElementException;
	default float remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(FloatConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
