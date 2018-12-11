package github.lightningcreations.lclib.primitive;

public interface FloatIterable {
	FloatIterator iterator();
	default void forEach(FloatConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
