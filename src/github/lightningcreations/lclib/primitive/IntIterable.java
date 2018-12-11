package github.lightningcreations.lclib.primitive;

import java.util.function.IntConsumer;

public interface IntIterable {
	IntIterator iterator();
	default void forEach(IntConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
