package github.lightningcreations.lclib.primitive;

import java.util.function.Consumer;

@FunctionalInterface
public interface CharConsumer extends Consumer<Character> {
	void accept(char c);
	default void accept(Character c) {
		accept(c.charValue());
	}
	default CharConsumer andThen(CharConsumer next) {
		return val->{CharConsumer.this.accept(val);next.accept(val);};
	}
	
	static CharConsumer select(CharPredicate selector,CharConsumer act1,CharConsumer act2) {
		return c->{
			if(selector.test(c))
				act1.accept(c);
			else
				act2.accept(c);
		};
	}
	static CharConsumer nop() {
		return c->{};
	}
}
