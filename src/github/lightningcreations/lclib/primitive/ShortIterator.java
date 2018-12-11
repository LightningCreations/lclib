package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;

public interface ShortIterator {
	public boolean hasNext();
	public short next()throws NoSuchElementException;
	default short remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(ShortConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
