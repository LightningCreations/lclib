package github.lightningcreations.lclib.primitive;

public interface ShortIterable {
	ShortIterator iterator();
	default void forEach(ShortConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
