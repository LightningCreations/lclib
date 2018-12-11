package github.lightningcreations.lclib.primitive;

import java.util.function.Consumer;

@FunctionalInterface
public interface FloatConsumer extends Consumer<Float> {
	void accept(float f);
	default void accept(Float f) {
		accept(f.floatValue());
	}
	default FloatConsumer andThen(FloatConsumer next) {
		return val->{FloatConsumer.this.accept(val);next.accept(val);};
	}
}
