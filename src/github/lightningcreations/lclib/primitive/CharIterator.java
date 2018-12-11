package github.lightningcreations.lclib.primitive;

import java.util.NoSuchElementException;

public interface CharIterator {
	public boolean hasNext();
	public char next()throws NoSuchElementException;
	default char remove()throws UnsupportedOperationException,NoSuchElementException{
		throw new UnsupportedOperationException("remove() is not supported");
	}
}
