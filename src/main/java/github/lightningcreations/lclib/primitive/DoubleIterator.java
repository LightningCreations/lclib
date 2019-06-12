package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;
import java.util.function.DoubleConsumer;

public interface DoubleIterator {
	public boolean hasNext();
	public double next()throws NoSuchElementException;
	default double remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(DoubleConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
