package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;
import java.util.function.IntConsumer;

public interface IntIterator {
	public boolean hasNext();
	public int next()throws NoSuchElementException;
	default int remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(IntConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
