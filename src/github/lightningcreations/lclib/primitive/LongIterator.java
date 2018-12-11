package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;
import java.util.function.LongConsumer;

public interface LongIterator {
	public boolean hasNext();
	public long next()throws NoSuchElementException;
	default long remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(LongConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
