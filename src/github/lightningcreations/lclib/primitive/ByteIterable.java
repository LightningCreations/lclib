package github.lightningcreations.lclib.primitive;

public interface ByteIterable {
	ByteIterator iterator();
	default void forEach(ByteConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
