package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;

public interface BooleanIterator {
	public boolean hasNext();
	public boolean next()throws NoSuchElementException;
	default boolean remove()throws NoSuchElementException,UnsupportedOperationException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(BooleanConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
