package github.lightningcreations.lclib.primitive;

public interface CharIterable {
	CharIterator iterator();

	default void forEach(CharConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
