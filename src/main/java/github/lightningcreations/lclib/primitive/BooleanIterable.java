package github.lightningcreations.lclib.primitive;

public interface BooleanIterable {
	BooleanIterator iterator();
	default void forEach(BooleanConsumer cons) {
		for(BooleanIterator itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
