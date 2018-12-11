package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;

public interface ByteIterator {
	public boolean hasNext();
	public byte next()throws NoSuchElementException;
	default byte remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
	default void forEachRemaining(ByteConsumer cons) {
		while(hasNext())
			cons.accept(next());
	}
}
