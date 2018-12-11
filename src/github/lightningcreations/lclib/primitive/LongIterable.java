package github.lightningcreations.lclib.primitive;

import java.util.function.LongConsumer;

public interface LongIterable {
	LongIterator iterator();
	default void forEach(LongConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	}
}
