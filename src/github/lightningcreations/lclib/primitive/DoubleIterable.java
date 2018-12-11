package github.lightningcreations.lclib.primitive;

import java.util.function.DoubleConsumer;

public interface DoubleIterable {
	DoubleIterator iterator();
	default void forEach(DoubleConsumer cons) {
		for(var itr=iterator();itr.hasNext();)
			cons.accept(itr.next());
	} 
}
